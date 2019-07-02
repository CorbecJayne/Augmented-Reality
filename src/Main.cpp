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
	namedWindow(Wname);

	//instantiate Player Manager 
	Player_Manager p_manager = Player_Manager();

	//instantiate Marker_Tracking & calibrate
	Marker_Tracking tracking;
	Point2f center;
	center.x=0;
	center.y=0;
	//center = tracking.calibrate();

    Question question = db.getNextQuestion();

	while (true){

        //if all locked in
        if(p_manager.all_locked()){
            //retrieve question
            question = db.getNextQuestion();
            cout<<question.to_string()<<endl;
            //mark correct answer
            //	ui_manager.display(question.getCorrectAnswer());

            //add points depending on if correct & order
            p_manager.give_score(question.getCorrectPosition());

            //display player info
            p_manager.reset_players();

        }


		//std::cout << question.to_string() << endl;

		//detect markers

        cap >> frame;
        vector<Player> new_infos = tracking.detect_markers(frame);
        imshow(Wname, frame);
        for (const Player& player : new_infos){
            // std::cout << "DEBUG: " << player.get_marker_id() << endl;
        }
		//compare 
		p_manager.update_player_info(center,new_infos);

        for(Player p:p_manager.get_players()){
            cout<<p.get_points()<<endl;
        }

		int key = waitKey(10);
		if (key == 27) {
            break;
        }
	}
	

	
	//destroyWindow(Wname);
	return 0;
}