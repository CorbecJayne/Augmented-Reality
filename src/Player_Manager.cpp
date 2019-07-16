#include "Player_Manager.h"
#include "Player.h"
#include <optional>


using namespace std;
using namespace cv;

bool compare_players_time(const Player& one,const Player& two){
    return one.get_time() < two.get_time();
}

bool compare_players_scores(const Player& one,const Player& two){
    return one.get_points() < two.get_points();
}

void Player_Manager::give_score(int correct_area) {
    sort(players.begin(),players.end(),compare_players_time);
    int score=1000;
    for(auto it=players.begin(); it!=players.end()-1; it++){
        // add points
        if((*it).get_area() == correct_area){
            (*it).add_points(score); 
            // should the next player get the same points?
            if((*it).get_time() != (*(it+1)).get_time()) score-=250;
        }
    }
    // last player
    if(players.back().get_area() == correct_area){
        players.back().add_points(score);
    }
}

vector<Player> &Player_Manager::get_players(){
    return players;
}

void Player_Manager::set_players(const vector<Player> &players) {
    Player_Manager::players = players;
}

Player_Manager::~Player_Manager() = default;

Player_Manager::Player_Manager() {
    players.emplace_back(1, 0x1C44);
    players.emplace_back(2, 0x0272);
    players.emplace_back(3, 0x5A);
    players.emplace_back(4, 0x690);
}

void Player_Manager::reset_players(){
    for(auto & player: players){ 
        player.reset();
    }
}

void Player_Manager::set_areas(Point2f& center){
    for(auto & player: players){
        if(player.get_position_player() != Point2f(-1,-1)){
            Point2f position = player.get_position_player();
            int area = get_area_of_point(position, center);
            player.set_area(area);
        }
    }
}
	
bool Player_Manager::all_locked(){
    for(auto & player: players){
        if (!player.get_locked_in()){
            return false;
        }
    }
    return true;
}

std::string Player_Manager::get_scores(){
    sort(players.begin(),players.end(),compare_players_time);
    std::string result = " | ";
    for(auto & player: players){
        result += std::to_string(player.get_player_id()) + ": " + std::to_string(player.get_points()) + " | ";
    }
}

optional<reference_wrapper<Player>> Player_Manager::find_Player(int marker_id){ 
    for(auto & player: players){
        if(marker_id == player.get_marker_id()) return player;
    }
    return nullopt;
}

void Player_Manager::update_player_info(Point2f& center, const vector<Player>& new_info) {
    for(auto & info: new_info){
        int info_marker_id = info.get_marker_id();
        // NULL check: valid marker id
        if(auto maybe_player = find_Player(info_marker_id)){
            Player& found_player = *maybe_player;
            // not locked in
            if(!found_player.get_locked_in()){
                found_player.set_position_player(info.get_position_player());
                found_player.set_result_matrix(info.get_result_matrix());
                // first time detected or player changed the answer
                bool changed_answer = (found_player.get_area() != get_area_of_point(info.get_position_player(), center));
                bool first_time = (found_player.get_time() == (time_t)(-1));
                if(changed_answer || first_time){

                    found_player.set_time(info.get_time());
                }

                    // player has choosen his answer
                else if(difftime(info.get_time(), found_player.get_time()) >= 5){ // difftime() result in seconds
                    found_player.lock_in();
                }

                set_areas(center);
            }
        }
    }
}

// test where the point is based on center
//ol 3 - 0
//or 0 - 1
//ul 1 - 2
//ur 2 - 3

/* top_left	:   0
	 	* top_right 	:   1
	 	* bottom_left  	:   2
	 	* bottom_right 	:   3
	 	*/
int Player_Manager::get_area_of_point(const Point2f& p, Point2f& center){
    /* if(p.y > (center.y - question_window_height_from_center)){
        return -5;
    }
    */
    if(p.x>=center.x){
        if(p.y>=center.y){
            return 1;
        }else{
            return 3;
        }
    }else{
        if(p.y>=center.y){
            return 0;
        }else{
            return 2;
        }
    }
}

