
#include "Player.h"
#include <vector>
#include <string>

class Player_Manager{
	
	//pointer ?
	std::vector<Player> players;


	
	public:

    Player_Manager();

    virtual ~Player_Manager();

    const std::vector<Player> &getPlayers() const;

    void setPlayers(const std::vector<Player> &players);

    //Constructs 4 Players markerId & playerId
	
	
	
	
	//resets position and timestamp
	void reset_players();
	
	std::string get_scores();
	
	/*iterate over players and compare timestamps
    * sorts players vector by player timestamp
    * players get 1000,750,500,250 points
    * only if they are correct
    */
	void give_score();
	
	
	//iterate ..
	void set_areas();
	
	bool all_locked();
};