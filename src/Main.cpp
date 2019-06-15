#include <Db_Api.h>
#include <Player_Manager.h>
#include <Marker_Tracking.h>
#include <UI_Manager.h>
#include <Player.h>

using namespace std;
using namespace cv;

int main(){
	
	//instantiate db & retrieve questions
	Db_Api db;
	db.retrieveQuestions();

	// instantiate UI Manager
	UI_Manager ui_manager;
	
	//instantiate Player Manager 
	Player_Manager p_manager = Player_Manager(ui_manager);
	
	//instantiate Marker_Tracking & calibrate
	//Marker_Tracking tracking;
	//tracking.calibrate();
	
	Mat image;

	int i=0;
	while(i<10){
		// get next camera image
		//ui_manager.get_next_image(image);

		//retrieve question
		Question question = db.getNextQuestion();
		
		//ui_manager show current question + answer
		ui_manager.display(question.to_string());
		
		//detect markers
		//vector<Player> new_infos = tracking.detect_markers(image);
		
		//compare 
		//p_manager.update_player_info(new_info);
		
		//if all locked in
		//if(p_manager.all_locked()){
			//mark correct answer
		//	ui_manager.display(question.getCorrectAnswer());
		
			//add points depending on if correct & order
		//	p_manager.give_score(question.getCorrectPosition());
		
			//display player info
		//	ui_manager.display(p_manager.get_scores());

	//	}
	    i++;
	}
	
	
	return 0;
}