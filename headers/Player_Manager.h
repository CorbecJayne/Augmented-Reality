
#include "Player.h"
#include "UI_Manager.h"
#include <vector>
#include <string>
#include <time.h>
#include <optional>
#include <functional>


class Player_Manager{
	
	std::vector<Player> players;
	UI_Manager ui_manager;

	public:

	/**
	 * @brief Constructs 4 Players with markerId & playerId
	 * 
	 */
    Player_Manager(UI_Manager manager);

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
	void give_score(int correct_area);
	
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

	/**
	 * @brief Set lock, time, position and areas of player if necessary
	 * 
	 */
	void update_player_info(std::vector<Player> new_info);

	/**
	 * @brief Find the player with the specified marker id
	 * 
	 * @param marker_id 
	 * @return Player 
	 */
	std::optional<std::reference_wrapper<Player>> find_Player(int marker_id);
};