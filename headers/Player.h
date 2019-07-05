#pragma once
#include <opencv2/opencv.hpp>
#include <time.h>

class Player
{
	// ID of the player
	int player_Id;
	// ID of the marker
    int marker_Id;
	// Total points of the player, default:0
	int points;
	// Position of the marker on the display, default: (-1,-1)
	cv::Point2f position_player;
	// the answer area based on the position, default:-1
	int area;
	// Timestamp when the answer/ answer was detected , default: -1
	time_t time;
	// If the current position/ answer counts as final, default:false
	bool locked_in;
	// pose of marker
    std::vector<float> result_matrix;


public:
    Player(time_t time);

	Player(int player_Id, int marker_Id);

    Player(cv::Point2f position_player, int marker_Id, time_t time, std::vector<float> result_matrix);

    const std::vector<float> &get_result_matrix() const;

    /**
     * @brief Reset position, area, time, locked_in
     *
     */
	void reset();

	/**
	 * @brief Get the player id
	 * 
	 * @return int 
	 */
	int get_player_id() const;

	/**
	 * @brief Get the marker id
	 * 
	 * @return int 
	 */
	int get_marker_id() const;
	
	/**
	 * @brief Add amount to points
	 * 
	 * @param amount 
	 */
	void add_points(int amount);

	/**
	 * @brief Get the points
	 * 
	 * @return int 
	 */
    int get_points() const;

	/**
	 * @brief Get the position_player
	 * 
	 * @return cv::Point2f 
	 */
    cv::Point2f get_position_player() const;

	/**
	 * @brief Set the position_player
	 * 
	 * @param position 
	 */
	void set_position_player(cv::Point2f position);

	/**
	 * @brief Get the area
	 * 
	 * @return int 
	 */
    int get_area() const;

	/**
	 * @brief Set the area
	 * 
	 * @param area 
	 */
	void set_area(int area);

	/**
	 * @brief Set the time
	 * 
	 * @param t 
	 */
	void set_time(time_t t);

	/**
	 * @brief Get the time
	 * 
	 * @return time_t 
	 */
    time_t get_time() const;

	/**
	 * @brief Get locked_in
	 * 
	 * @return true 
	 * @return false 
	 */
	bool get_locked_in() const;

	/**
	 * @brief Set locked_in to true
	 * 
	 */
	void lock_in();

	/**
	 * @brief Set locked_in
	 * 
	 * @param value 
	 */
	void set_locked_in(bool value);


    void set_result_matrix(std::vector<float> result_mat);
};