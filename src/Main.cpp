#include <Db_Api.h>
#include <Player_Manager.h>
#include <Marker_Tracking.h>
#include <Player.h>

#include <GLFW/glfw3.h>

#include "Draw_Primitives.h"
#include <iostream>
#include <iomanip>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Pose_Estimation.h"

using namespace std;
using namespace cv;


#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


cv::VideoCapture cap;

// Camera settings
const int camera_width  = 640;
const int camera_height = 480;
const int virtual_camera_angle = 30;
unsigned char bkgnd[camera_width * camera_height * 3];


/* Program & OpenGL initialization */
void initGL(int argc, char *argv[]) {

// Added in Exercise 8 - End *****************************************************************

    // For our connection between OpenCV/OpenGL
    // Pixel storage/packing stuff -> how to handle the pixel on the graphics card
    // For glReadPixels​ -> Pixel representation in the frame buffer
    glPixelStorei(GL_PACK_ALIGNMENT,   1);
    // For glTexImage2D​ -> Define the texture image representation
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Turn the texture coordinates from OpenCV to the texture coordinates OpenGL
    glPixelZoom(1.0, -1.0);

// Added in Exercise 8 - End *****************************************************************

    // Enable and set colors
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0, 0, 0, 1.0);

    // Enable and set depth parameters
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);

    // Light parameters
    GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_dif[] = {0.7, 0.7, 0.7, 1.0};

    // Enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_dif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


void display(GLFWwindow* window, const cv::Mat &img_bgr, float resultMatrix[16]) {

// Added in Exercise 8 - Start *****************************************************************

    // Copy picture data into bkgnd array
    memcpy(bkgnd, img_bgr.data, sizeof(bkgnd));

// Added in Exercise 8 - End *****************************************************************

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Needed for rendering the real camera image
    glMatrixMode(GL_MODELVIEW);
    // No position changes
    glLoadIdentity();

// Added in Exercise 8 - Start *****************************************************************

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    // Push the projection matrix (frustum) -> frustum will be saved on the stack
    glPushMatrix();
    glLoadIdentity();
    // In the ortho view all objects stay the same size at every distance
    glOrtho(0.0, camera_width, 0.0, camera_height,-1,1);

    // -> Render the camera picture as background texture
    // Making a raster of the image -> -1 otherwise overflow
    glRasterPos2i(0, camera_height-1);
    // Load and render the camera image -> unsigned byte because of bkgnd as unsigned char array
    // bkgnd 3 channels -> pixelwise rendering
    glDrawPixels(camera_width, camera_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, bkgnd);

    // Go back to the previous projection -> frustum
    glPopMatrix();

    // Activate depth -> that snowman can be scaled with depth
    glEnable(GL_DEPTH_TEST);

    // Move to marker-position
    glMatrixMode(GL_MODELVIEW);

    // Sadly doesn't work for Windows -> so we made own solution!
    //glLoadTransposeMatrixf(resultMatrix);

    // -> Transpose the Modelview Matrix
    float resultTransposedMatrix[16];
    for (int x=0; x<4; ++x) {
        for (int y=0; y<4; ++y) {
            // Change columns to rows
            resultTransposedMatrix[x*4+y] = resultMatrix[y*4+x];
        }
    }

    // Load the transpose matrix
    glLoadMatrixf(resultTransposedMatrix);

    // Rotate 90 desgress in x-direction
    glRotatef(-90, 1, 0, 0);
    // Scale down!
    glScalef(0.03, 0.03, 0.03);

// Added in Exercise 8 - End *****************************************************************

    // Draw 3 white spheres
    glColor4f(1.0, 1.0, 1.0, 1.0);
    drawSphere(0.8, 10, 10);
    glTranslatef(0.0, 0.8, 0.0);
    drawSphere(0.6, 10, 10);
    glTranslatef(0.0, 0.6, 0.0);
    drawSphere(0.4, 10, 10);

    // Draw the eyes
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.2, 0.2);
    drawSphere(0.066, 10, 10);
    glTranslatef(0, 0, -0.4);
    drawSphere(0.066, 10, 10);
    glPopMatrix();

    // Draw a nose
    glColor4f(1.0, 0.5, 0.0, 1.0);
    glTranslatef(0.3, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    drawCone(0.1, 0.3, 10, 10);
}


void reshape( GLFWwindow* window, int width, int height ) {
    // Set a whole-window viewport
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );

    // Create a perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // The camera should be calibrated -> a calibration results in the projection matrix -> then load the matrix
    // -> into GL_PROJECTION
    // -> adjustment of FOV is needed for each camera
    float ratio = (GLfloat)width / (GLfloat)height;

    float near = 0.01f, far = 100.f;
    float top = tan((double)(virtual_camera_angle*M_PI / 360.0f)) * near;
    float bottom = -top;
    float left = ratio * bottom;
    float right = ratio * top;
    glFrustum(left, right, bottom, top, near, far);
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Initialize the window system
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(camera_width, camera_height, "Quizzar", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    reshape(window, window_width, window_height);

    // Initialize the GL library
    initGL(argc, argv);

    // Setup OpenCV
    cv::Mat img_bgr;
    // Get video stream
    //initVideoStream(cap);
    // [m]
    const double kMarkerSize = 0.045;
    // Constructor with the marker size (similar to Exercise 5)
    //MarkerTracker markerTracker(kMarkerSize);

    float resultMatrix[16];
	Mat frame;
	VideoCapture cap(0);
	
	//instantiate db & retrieve questions
	Db_Api db;

	db.retrieveQuestions();
	
	if (!cap.isOpened())
	{
		cout << "NO capture" << endl;
		return -1;
	}
	//string Wname = "Frame Capture";
	//namedWindow(Wname);

	//instantiate Player Manager 
	Player_Manager p_manager = Player_Manager();

	//instantiate Marker_Tracking & calibrate
	Marker_Tracking tracking;
	Point2f center;
	center.x=0;
	center.y=0;
	//center = tracking.calibrate();

    Question question = db.getNextQuestion();
    cout<<question.to_string()<<endl;

    int i=1;

    while (true){
        //only 10 questions
        if(i>=10){
            break;
        }
        //if all locked in
        if(p_manager.all_locked()){
            i++;
            cout<<"correct answer : "<<question.getCorrectAnswer()<<endl;
            //retrieve question
            question = db.getNextQuestion();
            cout<<question.to_string()<<endl;
            //mark correct answer
            //	ui_manager.display(question.getCorrectAnswer());

            //add points depending on if correct & order
            p_manager.give_score(question.getCorrectPosition());

            //reset players
            p_manager.reset_players();



        }


        //std::cout << question.to_string() << endl;

        //detect markers

        cap >> frame;
        vector<Player> new_infos = tracking.detect_markers(frame,resultMatrix);
        //imshow(Wname, frame);

        //compare
        p_manager.update_player_info(center,new_infos);

        for(Player p:p_manager.get_players()){
            //cout<<p.get_points()<<endl;
        }

        // Track a marker and get the pose of the marker
        //markerTracker.findMarker(img_bgr, resultMatrix);

        // Render here
        display(window, frame, resultMatrix);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        int key = waitKey(10);
        if (key == 27) {
            break;
        }
    }

    // Important -> Avoid memory leaks!
    glfwTerminate();

	
	//destroyWindow(Wname);
	return 0;
}