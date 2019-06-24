#pragma once 
#include <opencv2/opencv.hpp>

class UI_Manager
{
	cv::Point center;
	cv::VideoCapture camera;
	std::string Wname;
	
public:

	UI_Manager();
	UI_Manager(cv::VideoCapture &cap);
	~UI_Manager()= default;
	
	void display(std::string text);
	void set_areas();

	/* uses member center to calculate area of given point returns accordingly
	 * top_left     :   0
	 * top_right    :   1
	 * bottom_left  :   2
	 * bottom_right :   3
	 */
	int get_area_of_point(cv::Point p);

	// get camera image
	cv::Mat get_next_image(cv::Mat &mat);

    const cv::Point &getCenter() const;

    void setCenter(const cv::Point &center);
};
