
#include "Player.h"
#include <vector>
#include <string>

class Player_Manager{
	
	//pointer ?
	std::vector<Player> players;
	
	public:
	
	//Constructs 4 Players markerId & playerId
	
	
	
	
	//resets position and timestamp
	void reset_players();
	
	std::string get_scores();
	
	//iterate over players and compare timestamps
	void give_score();
	
	
	//iterate ..
	void set_areas();
	
	bool all_locked();
};