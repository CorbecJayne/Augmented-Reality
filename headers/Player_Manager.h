#pragma once

#include "Player.h"
#include <vector>
#include <string>
#include <time.h>
#include <optional>
#include <functional>
#include <opencv2/opencv.hpp>

bool compare_players_scores(const Player& one,const Player& two);

class Player_Manager{
	
	std::vector<Player> players;
	int question_window_height_from_center;

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
    std::vector<Player> &get_players();

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
    void set_areas(cv::Point2f& center); 
	
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
    void update_player_info(cv::Point2f& center, const std::vector<Player>& new_info);

	/**
	 * @brief Find the player with the specified marker id
	 * 
	 * @param marker_id 
	 * @return Player 
	 */
	std::optional<std::reference_wrapper<Player>> find_Player(int marker_id);


	/* uses member center to calculate area of given point returns accordingly
	 * /* top_left	:   0
	 	* top_right 	:   1
	 	* bottom_left  	:   2
	 	* bottom_right 	:   3
	 	*/
	int get_area_of_point(const cv::Point2f& p, cv::Point2f& center);


	/* getter and setter for question window height, the window in which the question 
	 * will be displayed
	 * As a basis center of screen is used!!
	 */ 
	void set_question_window_height_from_center(int height){
		question_window_height_from_center = height;
	}

	int get_question_window_height_from_center(){
		return question_window_height_from_center;
	}

};