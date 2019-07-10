#include <Db_Api.h>
#include <Player_Manager.h>
#include <Marker_Tracking.h>
#include <Player.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Draw_Primitives.h"
#include "Pose_Estimation.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace cv;


// Camera settings
const int camera_width  = 1280;         // change to change height and width, eg from 640 to 1280
const int camera_height = 720;          // change to change height and width, eg from 480 to 720
const int virtual_camera_angle = 60;    // change to change height and width, eg from 30 to 60
unsigned char bkgnd[camera_width * camera_height * 3]; 

const double timeForQuestion = 15.0;


/* Program & OpenGL initialization */
void initGL(int argc, char *argv[]); 

void display(GLFWwindow* window, const cv::Mat &img_bgr, const vector<vector<float>>&results,vector<Player> players);

void reshape( GLFWwindow* window, int width, int height );

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
    Mat img_bgr;
    // Get video stream
    //initVideoStream(cap);
    // [m]

	Mat frame;
	VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, camera_width); //changed for different width and height
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, camera_height);
	
	//instantiate db & retrieve questions
	Db_Api db;
	db.retrieveQuestions();
	
	if (!cap.isOpened())
	{
		cout << "NO capture" << endl;
		return -1;
	}

	//instantiate Player Manager 
	Player_Manager p_manager = Player_Manager();

	//instantiate Marker_Tracking & calibrate
	Marker_Tracking tracking{};
	Point2f center;
	center.x=0;
	center.y=0;

    Question question = db.getNextQuestion();
    cout<<question.to_string()<<endl;

    int i=0;

    double duration = 0;

    clock_t start;

    start = clock();

    while (true){
        duration = (( std::clock() - start ) / (double) CLOCKS_PER_SEC)*2;
        //only 10 questions
        if(i>=10){
            break;
        }
        //if all locked in
        if(p_manager.all_locked()||duration>timeForQuestion){
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

            start=clock();
        }


        //std::cout << question.to_string() << endl; 

        //detect markers

        cap >> frame;
        tuple<Mat,vector<Player> > result = tracking.detect_markers(frame, question, camera_width, camera_height,(int)(timeForQuestion-duration+1));
        vector<Player> new_infos = get<1>(result);
        //imshow(Wname, frame);

        //compare
        p_manager.update_player_info(center,new_infos); 

        // Render here

        vector<vector<float>> results;
        for(const Player& p:p_manager.get_players()){
            //cout<<p.get_points()<<endl;
            vector<float> mat (16);
            for(int j=0;j<16;j++){
                mat[j]=p.get_result_matrix()[j];
            }
            results.push_back(mat);

        }
        display(window,get<0>(result),results,p_manager.get_players());

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
        int key = waitKey(10); // doesnt work
        if (key == 27) {
            break;
        }
    }

    //put this in a while loop with a waitkey or timeout
   // draw_results();


    // Important -> Avoid memory leaks!
    glfwTerminate();

	//destroyWindow(Wname);
	return 0;
}

void initGL(int argc, char *argv[]){
    // For our connection between OpenCV/OpenGL
    // Pixel storage/packing stuff -> how to handle the pixel on the graphics card
    // For glReadPixels​ -> Pixel representation in the frame buffer
    glPixelStorei(GL_PACK_ALIGNMENT,   1);
    // For glTexImage2D​ -> Define the texture image representation
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Turn the texture coordinates from OpenCV to the texture coordinates OpenGL
    glPixelZoom(1.0, -1.0);

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
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_dif);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
}

void display(GLFWwindow* window, const cv::Mat &img_bgr, const vector<vector<float>>&results,vector<Player> players){
    // Copy picture data into bkgnd array
    memcpy(bkgnd, img_bgr.data, sizeof(bkgnd));

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Needed for rendering the real camera image
    glMatrixMode(GL_MODELVIEW);
    // No position changes
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    // Push the projection matrix (frustum) -> frustum will be saved on the stack
    glPushMatrix();
    glLoadIdentity();
    // In the ortho view all objects stay the same size at every distance
    glOrtho(0.0, camera_width, 0.0, camera_height, -1, 1);

    // -> Render the camera picture as background texture
    // Making a raster of the image -> -1 otherwise overflow
    glRasterPos2i(0, camera_height - 1);
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
    int counter=0;
    for(vector<float> result:results) {
        counter++;
        // -> Transpose the Modelview Matrix
        float resultTransposedMatrix[16];
        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y) {
                // Change columns to rows
                resultTransposedMatrix[x * 4 + y] = result[y * 4 + x];
            }
        }

        // Load the transpose matrix
        glLoadMatrixf(resultTransposedMatrix);

        // Rotate 90 desgress in x-direction
        glRotatef(-90, 1, 0, 0);
        // Scale down!
        glScalef(0.02, 0.02, 0.02);

        draw_player(counter);
    }
}


void reshape( GLFWwindow* window, int width, int height ){
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
