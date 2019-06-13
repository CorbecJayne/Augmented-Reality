
#include "Player.h"
#include <opencv2/opencv.hpp>

Player::Player(time_t time) : time(time) {}

Player::Player(int player_Id, int marker_Id) : 
    player_Id(player_Id), marker_Id(marker_Id), locked_in(false), points(0), area(-1), position_player(cv::Point(-1,-1)), time((time_t)(-1)) {}

Player::Player(cv::Point position_player, int marker_Id, time_t time) :
    player_Id(-1), marker_Id(marker_Id), locked_in(false), points(0), area(-1), position_player(position_player), time(time) {}

void Player::set_time(time_t t) {
    Player::time=t;
}

time_t Player::get_time() const {
    return time;
}

int Player::get_area() const {
    return area;
}

void Player::set_area(int area){
    area = area;
}

int Player::get_points() const {
    return points;
}

void Player::add_points(int amount) {
    points+=amount;
}

cv::Point Player::get_position_player() const{
    return position_player;
}

void Player::set_position_player(cv::Point position){
    position_player = position;
}

bool Player::get_locked_in() const{
    return locked_in;
}

void Player::lock_in(){
    locked_in = true;
}

void Player::set_locked_in(bool value){
    locked_in = value;
}

void Player::reset(){
    set_position_player(cv::Point(-1,-1));
    set_area(-1);
    set_time((time_t)(-1));
    set_locked_in(false);
}

int Player::get_player_id() const{
    return player_Id;
}
