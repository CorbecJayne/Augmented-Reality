
#include "Player.h"
#include <opencv2/opencv.hpp>

Player::Player(int player_Id, int marker_Id) :
    player_Id(player_Id), marker_Id(marker_Id), locked_in(false), points(0), area(-1), position_player(cv::Point2f(-1,-1)), time((time_t)(-1)) {
    for(int i=0;i<16;i++){
        result_matrix.push_back(0);
    }
}

Player::Player(cv::Point2f position_player, int marker_Id, time_t time, std::vector<float> result_mat) :
        player_Id(-1), marker_Id(marker_Id), locked_in(false), points(0), area(-1), position_player(position_player), time(time) {
    if(result_mat.size()!=16){
        for(int i=0;i<16;i++){
            result_matrix.push_back(0);
        }
    }else{
        result_matrix=result_mat;
    }
}

void Player::set_time(time_t t) {
    Player::time=t;
}

time_t Player::get_time() const {
    return time;
}

int Player::get_area() const {
    return area;
}

void Player::set_area(int new_area){
    area = new_area;
}

int Player::get_points() const {
    return points;
}

void Player::add_points(int amount) {
    points+=amount;
}

cv::Point2f Player::get_position_player() const{
    return position_player;
}

void Player::set_position_player(cv::Point2f position){
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
    set_position_player(cv::Point2f(-1,-1));
    set_area(-1);
    set_time((time_t)(-1));
    set_locked_in(false);
    result_matrix={};
    for(int i=0;i<16;i++){
        result_matrix.push_back(0);
    }
}

int Player::get_player_id() const{
    return player_Id;
}

int Player::get_marker_id() const{
    return marker_Id;
}

const std::vector<float> &Player::get_result_matrix() const {
    return result_matrix;
}

void Player::set_result_matrix(std::vector<float> result_mat){
    result_matrix = result_mat;
}
