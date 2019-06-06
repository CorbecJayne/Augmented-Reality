class Player_Manager{
	
	//pointer ?
	vector<Player> players;
	
	public:
	
	//Constructs 4 Players markerId & playerId
	
	
	
	
	//resets position and timestamp
	void reset_players();
	
	string get_scores();
	
	//iterate over players and compare timestamps
	void give_score();
	
	
	//iterate ..
	void set_areas();
	
	bool all_locked();
}