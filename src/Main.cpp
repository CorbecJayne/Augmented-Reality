#include <Db_Api.h>
#include <Player_Manager.h>
#include <Marker_Tracking.h>
#include <Player.h>

using namespace std;
using namespace cv;

int main() {
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
	string Wname = "Frame Capture";
	//namedWindow(Wname);

	//instantiate Player Manager 
	Player_Manager p_manager = Player_Manager();

	//instantiate Marker_Tracking & calibrate
	Marker_Tracking tracking;
	Point center;
	//center = tracking.calibrate();

	// get next camera frame

	int i = 0;
	while (i < 10)
	{

		//retrieve question
		Question question = db.getNextQuestion();
		
		//ui_manager show current question + answer
		//std::cout << question.to_string() << endl;

		//detect markers
		for (int i = 0; i < 100; i++){
			cap >> frame;
			vector<Player> new_infos = tracking.detect_markers(frame);
			for (Player player : new_infos){
				std::cout << "DEBUG: " << player.get_marker_id() << endl;
			}
		}  

		
		//compare 
		//p_manager.update_player_info(center,new_infos);
		//imshow(Wname, frame);
		int key = waitKey(10);
		if (key == 27){
			break;
		}

		i++;
	}
	
	//if all locked in
	//if(p_manager.all_locked()){
		//mark correct answer
	//	ui_manager.display(question.getCorrectAnswer());
	
		//add points depending on if correct & order
	//	p_manager.give_score(question.getCorrectPosition());
	
		//display player info
	//	ui_manager.display(p_manager.get_scores());

	//	}
	
	//destroyWindow(Wname);
	return 0;
}