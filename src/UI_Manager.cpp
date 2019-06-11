#include "headers/UI_Manager.h"
#include <string>

using namespace std;
using namespace cv;

// like this
UI_Manager::UI_Manager() {
    VideoCapture cap(0);
	string Wname = "Frame Capture";
	namedWindow(Wname);
}

// display on console
void UI_Manager::display(string text){

}

// redundant?
void UI_Manager::set_areas(){

}

// test where the point is based on center
int UI_Manager::get_area_of_point(Point p){

}

// get camera image, use code from ARExercise
Mat UI_Manager::get_next_image(Mat frame){
    cap >> frame;
	imshow(Wname, frame);
}