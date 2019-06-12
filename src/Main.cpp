#include <Db_Api.h>
#include <Player_Manager.h>
#include <Marker_Tracking.h>
#include "headers/Db_Api.h"
#include "headers/Player_Manager.h" 
#include "headers/Marker_Tracking.h" 
#include "headers/UI_Manager.h" 

int main(){
	
	//instantiate db & retrieve questions
	Db_Api db;
	db.retrieveQuestions();
	
	//instantiate Player Manager 
	Player_Manager p_manager;
	
	//instantiate Marker_Tracking & calibrate
	Marker_Tracking tracking;
	tracking.calibrate();

	UI_Manager ui_manager;
	Mat image;

	while(true){
		// get next camera image
		ui_manager.get_next_image(image);

		//retrieve question
		Question question = db.getNextQuestion();
		
		//ui_manager show current question + answer
		ui_manager.display(question.show_question());
		
		//detect markers
		vector<Player> new_infos = tracking.detect_markers(image);
		
		//compare 
		p_manager.update_player_info(new_info);
		
		//if all locked in
		if(p_manager.all_locked()){
			//mark correct answer
			ui_manager.display(question.getCorrectAnswer());
		
			//add points depending on if correct & order
			p_manager.give_score();
		
			//display player info
			ui_manager.display(p_manager.get_scores());

		}
	}
	
	
	return 0;
}