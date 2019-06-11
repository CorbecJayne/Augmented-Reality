
#include "Player.h"
#include <vector>
#include <string>

class Player_Manager{
	
	std::vector<Player> players;

	public:

	/**
	 * @brief Constructs 4 Players with markerId & playerId
	 * 
	 */
    Player_Manager();

    virtual ~Player_Manager();

	/**
	 * @brief Get all player
	 * 
	 * @return const std::vector<Player>& 
	 */
    const std::vector<Player> &get_players() const;

	/**
	 * @brief Set the players
	 * 
	 * @param players 
	 */
    void set_players(const std::vector<Player> &players);
	
	/**
	 * @brief Resets position and timestamp of all player
	 * 
	 */
	void reset_players();

	/**
	 * @brief Generates a nice announcement of the player scores
	 * 
	 * @return std::string 
	 */
	std::string get_scores();
   	
	/**
	 * @brief Sets the scores of all players based on the timestamps. Possible points: 1000,750,500,250
	 * 
	 */
	void give_score();
	
	/**
	 * @brief Set the area of the player based on its position
	 * 
	 */
	void set_areas();
	
	/**
	 * @brief Checks if all player have answered
	 * 
	 * @return true 
	 * @return false 
	 */
	bool all_locked();
};