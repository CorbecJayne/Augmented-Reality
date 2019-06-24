#include "UI_Manager.h"
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// like this
UI_Manager::UI_Manager(VideoCapture &cap) {
	camera = cap;
    Wname = "Frame Capture";
	namedWindow(Wname, CV_WINDOW_AUTOSIZE);
    
}

// display on console
void UI_Manager::display(string text) {
    cout<<text<<endl;
}

// redundant?
void UI_Manager::set_areas(){

}

// test where the point is based on center
int UI_Manager::get_area_of_point(Point p){
    if(p.x>=center.x){
        if(p.y>=center.y){
            return 3;
        }else{
            return 1;
        }
    }else{
        if(p.y>=center.y){
            return 2;
        }else{
            return 0;
        }
    }
}

const Point &UI_Manager::getCenter() const {
    return center;
}

void UI_Manager::setCenter(const Point &center) {
    UI_Manager::center = center;
}

// get camera image, use code from ARExercise
Mat UI_Manager::get_next_image(Mat &frame){
    if (!camera.isOpened()) {
		cout << "No webcam, using video file" << endl;
		camera.open("MarkerMovie.MP4");
        Mat empty;
        return empty;
	}
    camera.read(frame);
	imshow(Wname, frame);
    return frame;
}