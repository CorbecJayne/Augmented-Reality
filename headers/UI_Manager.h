using namespace std;
using namespace cv;

class UI_Manager
{
	vector<Area> areas;
	Point center;
	VideoCapture cap(0);
	
public:
	UI_Manager();
	~UI_Manager();
	
	void display(string text);
	void set_areas();
	
	int get_area_of_point(Point p);

	// get camera image
	Mat get_next_image();
	
};
