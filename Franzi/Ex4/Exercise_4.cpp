#include<opencv2/opencv.hpp>
#include<iostream>


using namespace cv;
using namespace std;


#define EX5 1
#define EX5_RAW 0
#define DRAW_CONTOUR 0
#define DRAW_RECTANGLE 0

#define THICKNESS_VALUE 1


struct MyStrip {
	int stripeLength;
	int nStop;
	int nStart;
	Point2f stripeVecX;
	Point2f stripeVecY;
};


typedef vector<Point> contour_t;
typedef vector<contour_t> contour_vector_t;


const int threshold_slider_max = 255;
int threshold_slider = 0;

const int fps = 30;

Mat videoStreamFrameGray;
Mat videoStreamFrameOutput;

const string stripWindow = "Strip Window";

// Pos is from UI, dereferencing of the pointer
static void on_trackbar(int pos, void* slider_value) {
	*((int*)slider_value) = pos;
	// C++ >= 11 -> Standard
	//*(static_cast<int*>(slider_value)) = pos;
}

int subpixSampleSafe(const Mat& pSrc, const Point2f& p) {
	int fx = int(floorf(p.x));
	int fy = int(floorf(p.y));

	if (fx < 0 || fx >= pSrc.cols - 1 ||
		fy < 0 || fy >= pSrc.rows - 1)
		return 127;

	int px = int(256 * (p.x - floorf(p.x)));
	int py = int(256 * (p.y - floorf(p.y)));

	unsigned char* i = (unsigned char*)((pSrc.data + fy * pSrc.step) + fx);

	int a = i[0] + ((px * (i[1] - i[0])) >> 8);
	i += pSrc.step;
	int b = i[0] + ((px * (i[1] - i[0])) >> 8);

	return a + ((py * (b - a)) >> 8);
}


Mat calculate_Stripe(double dx, double dy, MyStrip & st) {
	// Norm (euclidean distance)
	double diffLength = sqrt(dx * dx + dy * dy);

	st.stripeLength = (int)(0.8 * diffLength);

	if (st.stripeLength < 5)
		st.stripeLength = 5;

	st.stripeLength |= 1;

	st.nStop = st.stripeLength >> 1;
	st.nStart = -st.nStop;

	Size stripeSize;

	stripeSize.width = 3;
	stripeSize.height = st.stripeLength;

	// Normalized direction vector
	st.stripeVecX.x = dx / diffLength;
	st.stripeVecX.y = dy / diffLength;

	// Normalized perpendicular vector
	st.stripeVecY.x = st.stripeVecX.y;
	st.stripeVecY.y = -st.stripeVecX.x;

	return Mat(stripeSize, CV_8UC1);
}

Mat drawLines(Mat image, Point2f points[6], int i , Point &pt1, Point &pt2){
	float lineParams[16];
	Mat lineParamsMat(Size(4, 4), CV_32F, lineParams);
	Mat point_mat(Size(1, 6), CV_32FC2, points);


	// returns (vx, vy, x0, y0): normalized vector (vx,vy) and point (x0,y0)
	fitLine( point_mat, lineParamsMat.col(i), CV_DIST_L2, 0, 0.01, 0.01 );
	
	float d = sqrt((double)lineParamsMat.at<float>(0,i) * lineParamsMat.at<float>(0,i) + (double)lineParamsMat.at<float>(1,i) * lineParamsMat.at<float>(1,i));
    float t = (float)(image.cols + image.rows);

	//(x,y) = (x0,y0) + t*(vx,vy)
	pt1.x = cvRound(lineParamsMat.at<float>(2,i) - (lineParamsMat.at<float>(0,i) / d) * t);
    pt1.y = cvRound(lineParamsMat.at<float>(3,i) - (lineParamsMat.at<float>(1,i) / d) * t);
    pt2.x = cvRound(lineParamsMat.at<float>(2,i) + (lineParamsMat.at<float>(0,i) / d) * t);
    pt2.y = cvRound(lineParamsMat.at<float>(3,i) + (lineParamsMat.at<float>(1,i) / d) * t);

    line(image, pt1, pt2, Scalar(255, 0, 0), 1, LINE_4); //BGR
	// cout << "Angekommen1" << endl;
	// cout << pt2 << endl;

	return image;

}

inline double Det(double a, double b, double c, double d)
{
	return a*d - b*c;
}

///Calculate intersection of two lines.
///\return true if found, false if not found or error
bool LineLineIntersect(double x1, double y1, //Line 1 start
    double x2, double y2, //Line 1 end
    double x3, double y3, //Line 2 start
    double x4, double y4, //Line 2 end
    double &ixOut, double &iyOut) //Output 
{

    double detL1 = Det(x1, y1, x2, y2);
    double detL2 = Det(x3, y3, x4, y4);
    double x1mx2 = x1 - x2;
    double x3mx4 = x3 - x4;
    double y1my2 = y1 - y2;
    double y3my4 = y3 - y4;

    double xnom = Det(detL1, x1mx2, detL2, x3mx4);
    double ynom = Det(detL1, y1my2, detL2, y3my4);
    double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
    if(denom == 0.0)//Lines don't seem to cross
    {
        ixOut = NAN;
        iyOut = NAN;
        return false;
    }

    ixOut = xnom / denom;   
    iyOut = ynom / denom;
    if(!isfinite(ixOut) || !isfinite(iyOut)) //Probably a numerical issue
        return false;

    return true; //All OK
}

vector<Point> drawEdges(Mat img, vector<Point> collectLinePoints){
	vector<Point> edges;

	for(int i = 0; i < collectLinePoints.size(); i=i+2){

		Point l11 = collectLinePoints.at(i);
		Point l12 = collectLinePoints.at(i+1);

		for(int k = i+2; k < collectLinePoints.size(); k=k+2){

			Point l21 = collectLinePoints.at(k);
			Point l22 = collectLinePoints.at(k+1);

			double edgex;
			double edgey;
			if(LineLineIntersect(l11.x, l11.y, l12.x, l12.y, l21.x, l21.y, l22.x, l22.y, edgex, edgey)){
				if((edgex > 0) && (edgey > 0)){
					Point center;
					center.x = edgex;
					center.y = edgey;
					circle(img, center, 3, CV_RGB(255, 0, 0), 2);
					edges.push_back(center);
				}
			}
		}
	}
	return edges;
}


Mat perspective_transformation(Mat image, vector<Point> edges)
{
	vector<Point> goalpoints;
	goalpoints.push_back(Point(-0.5, -0.5));
	goalpoints.push_back(Point(-0.5, 5.5));
	goalpoints.push_back(Point(5.5, 5.5));
	goalpoints.push_back(Point(5.5, -0.5));

	cout << edges << endl;

	Mat transformed = getPerspectiveTransform(goalpoints, edges);
	Mat output(Size(6,6), CV_32FC2);
	// warpPerspective(image, output, transformed, Size(6,6));
	return output;
}

bool isMarker(Mat marker){
	for(int i = 0; i< 6; i++){
		// cout << marker.at<double>(0,0) << endl;
		// left, top, right, bottom
		// if( (marker[i][1] != CV_RGB(255, 0, 0)) || (marker[1][i] != CV_RGB(255, 0, 0)) || (marker[i][4] != CV_RGB(255, 0, 0)) || (marker[4][i] != CV_RGB(255, 0, 0))){
		// 	return false ;
		// } 
	}
	return true;
}

bool is_near_black(int value){
	return true;
}

void print_identifier(Mat marker){
	cout << "identifier" << endl;
}

int main() {
	Mat frame;
	VideoCapture cap(0);

	const string streamWindow = "Stream";

	if (!cap.isOpened()) {
		cout << "No webcam, using video file" << endl;
		cap.open("MarkerMovie.MP4");
		if (cap.isOpened() == false) {
			cout << "No video!" << endl;
			exit(0);
			return -1;
		}
	}

	bool isFirstStripe = true;

	const string contoursWindow = "Contours";
	const string UI = "Threshold";
	namedWindow(contoursWindow, CV_WINDOW_AUTOSIZE);

	int slider_value = 100;
	createTrackbar(UI, contoursWindow, &slider_value, 255, on_trackbar, &slider_value);

	Mat imgFiltered;

	while (cap.read(frame)) {

		// --- Process Frame ---

		Mat grayScale;
		imgFiltered = frame.clone();
		cvtColor(imgFiltered, grayScale, COLOR_BGR2GRAY);

		// Threshold to reduce the noise
		threshold(grayScale, grayScale, slider_value, 255, THRESH_BINARY);

		contour_vector_t contours;

		findContours(grayScale, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		// over all found contours
		for (size_t k = 0; k < contours.size(); k++) {

			contour_t approx_contour;

			// Simplifying of the contour with the Ramer-Douglas-Peuker Algorithm
			approxPolyDP(contours[k], approx_contour, arcLength(contours[k], true) * 0.02, true);

			Scalar QUADRILATERAL_COLOR(0, 0, 255);
			Scalar colour;
			// Convert to a usable rectangle
			Rect r = boundingRect(approx_contour);

			if (approx_contour.size() == 4) {
				colour = QUADRILATERAL_COLOR;
			}
			else {
				continue;
			}

			// --- Filter rectangles ones ---
			if (r.height < 20 || r.width < 20 || r.width > imgFiltered.cols - 10 || r.height > imgFiltered.rows - 10) {
				continue;
			}

			polylines(imgFiltered, approx_contour, true, colour, THICKNESS_VALUE);

			vector<Point> collectLinePoints;

			// over all found  egdes
			for (size_t i = 0; i < approx_contour.size(); ++i) {
				circle(imgFiltered, approx_contour[i], 3, CV_RGB(0, 255, 0), -1);

				// Euclidic distance
				double dx = ((double)approx_contour[(i + 1) % 4].x - (double)approx_contour[i].x) / 7.0;
				double dy = ((double)approx_contour[(i + 1) % 4].y - (double)approx_contour[i].y) / 7.0;

				MyStrip strip;

				Mat imagePixelStripe = calculate_Stripe(dx, dy, strip);

				Point2f collectRealPoints[6]; 

				// over all 6 points per Edge
				for (int j = 1; j < 7; ++j) {
					// Position calculation
					double px = (double)approx_contour[i].x + (double)j * dx;
					double py = (double)approx_contour[i].y + (double)j * dy;

					Point p;
					p.x = (int)px;
					p.y = (int)py;
					circle(imgFiltered, p, 2, CV_RGB(0, 0, 255), -1);

					// Columns
					for (int m = -1; m <= 1; ++m) {
						// Rows
						for (int n = strip.nStart; n <= strip.nStop; ++n) {
							Point2f subPixel;

							subPixel.x = (double)p.x + ((double)m * strip.stripeVecX.x) + ((double)n * strip.stripeVecY.x);
							subPixel.y = (double)p.y + ((double)m * strip.stripeVecX.y) + ((double)n * strip.stripeVecY.y);

							Point p2;
							p2.x = (int)subPixel.x;
							p2.y = (int)subPixel.y;

							if (isFirstStripe)
								circle(imgFiltered, p2, 1, CV_RGB(255, 0, 255), -1);
							else
								circle(imgFiltered, p2, 1, CV_RGB(0, 255, 255), -1);

							// Combined Intensity of the subpixel
							int pixelIntensity = subpixSampleSafe(imgFiltered, subPixel);

							// Converte from index to pixel coordinate
							int w = m + 1;

							// n (Row, real)
							int h = n + (strip.stripeLength >> 1);

							imagePixelStripe.at<uchar>(h, w) = (uchar)pixelIntensity;

						}
					}

					// The first and last row must be excluded
					vector<double> sobelValues(strip.stripeLength - 2.);

					for (int n = 1; n < (strip.stripeLength - 1); n++) {
						// Take the intensity value from the stripe 
						unsigned char* stripePtr = &(imagePixelStripe.at<uchar>(n - 1, 0));

						// Calculation of the gradient with the sobel for the first row
						double r1 = -stripePtr[0] - 2. * stripePtr[1] - stripePtr[2];

						// r2 -> Is equal to 0

						// Jump two lines
						stripePtr += 2 * imagePixelStripe.step;

						double r3 = stripePtr[0] + 2. * stripePtr[1] + stripePtr[2];

						unsigned int ti = n - 1;
						sobelValues[ti] = r1 + r3;
					}

					double maxIntensity = -1;
					int maxIntensityIndex = 0;

					// Finding the max value
					for (int n = 0; n < strip.stripeLength - 2; ++n) {
						if (sobelValues[n] > maxIntensity) {
							maxIntensity = sobelValues[n];
							maxIntensityIndex = n;
						}
					}

					// f(x) slide 7 -> y0 .. y1 .. y2
					double y0, y1, y2;

					// Point before and after
					unsigned int max1 = maxIntensityIndex - 1, max2 = maxIntensityIndex + 1;

					// If the index is at the border we are out of the stripe, than we will take 0
					y0 = (maxIntensityIndex <= 0) ? 0 : sobelValues[max1];
					y1 = sobelValues[maxIntensityIndex];
					// If we are going out of the array of the sobel values
					y2 = (maxIntensityIndex >= strip.stripeLength - 3) ? 0 : sobelValues[max2];

					// d = 1 because of the normalization
					double pos = (y2 - y0) / (4 * y1 - 2 * y0 - 2 * y2);

					// Check if there is a solution to the calculation
					if (pos != pos) {
						// Value is not a number -> NAN
						continue;
					}

					// Exact point with subpixel accuracy
					Point2d edgeCenter;

					int maxIndexShift = maxIntensityIndex - (strip.stripeLength >> 1);

					// Shift the original edgepoint accordingly
					edgeCenter.x = (double)p.x + (((double)maxIndexShift + pos) * strip.stripeVecY.x);
					edgeCenter.y = (double)p.y + (((double)maxIndexShift + pos) * strip.stripeVecY.y);

					collectRealPoints[j-1] = edgeCenter;

					// Highlight the subpixel with blue color
					circle(imgFiltered, edgeCenter, 2, CV_RGB(0, 0, 255), -1);

				}

				Point p1, p2;
				imgFiltered = drawLines(imgFiltered, collectRealPoints, i, p1, p2);
				
				collectLinePoints.push_back(p1);
				collectLinePoints.push_back(p2);

			}

			vector<Point> edges = drawEdges(imgFiltered, collectLinePoints);
			if(edges.size() == 4)
			{
				Mat marker = perspective_transformation(imgFiltered, edges);
				if(isMarker(marker)){
					print_identifier(marker);
				}
			}

		}

		imshow(contoursWindow, imgFiltered);
		isFirstStripe = true;

		if (waitKey(10) == 27) {
			break;
		}
	}

	destroyWindow(contoursWindow);

	return 0;
}