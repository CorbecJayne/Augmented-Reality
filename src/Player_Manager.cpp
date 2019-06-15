#include "Player_Manager.h"
#include "Player.h"
#include <optional>


using namespace std;
using namespace cv;

bool compare_players_time(Player one,Player two){
    return one.get_time() < two.get_time();
}

bool compare_players_scores(Player one,Player two){
    return one.get_points() < two.get_points();
}

void Player_Manager::give_score(int correct_area) {
    sort(players.begin(),players.end(),compare_players_time);
    int score=1000;
    for(auto it=players.begin(); it!=players.end()-1; it++){
        // add points
        if((*it).get_area() == correct_area) (*it).add_points(score); 
        // should the next player get the same points
        if((*it).get_time() != (*(it+1)).get_time()) score-=250;
    }

    for(auto & player : players){
        //TODO: to something if both player answered at the same time
        if(player.get_area() == correct_area){
            player.add_points(score); 
            score-=250;
        }
    }
}

const vector<Player> &Player_Manager::get_players() const {
    return players;
}

void Player_Manager::set_players(const vector<Player> &players) {
    Player_Manager::players = players;
}

Player_Manager::~Player_Manager() = default;

Player_Manager::Player_Manager(UI_Manager manager) {
    // players.emplace_back(98);
    // players.emplace_back(23);
    // players.emplace_back(123);
    // players.emplace_back(53);
    
    players.emplace_back(1, 0x1C44);
    players.emplace_back(2, 0x0272);
    players.emplace_back(3, 0x2121);
    players.emplace_back(4, 0x0969);

    ui_manager = manager; 
}

void Player_Manager::reset_players(){
    for(auto & player: players){
        player.reset();
    }
}

void Player_Manager::set_areas(){
    for(auto & player: players){
        if(player.get_position_player() != Point(-1,-1)){
            Point position = player.get_position_player();
            int area = ui_manager.get_area_of_point(position);
            player.set_area(area);
        }
    }
}
	
bool Player_Manager::all_locked(){
    for(auto & player: players){
        if (!player.get_locked_in()) return false;
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

optional<Player> Player_Manager::find_Player(int marker_id){ 
    for(auto & player: players){
        if(marker_id == player.get_marker_id()) return player;
    }
    return nullopt;
}

void Player_Manager::update_player_info(vector<Player> new_info){
    for(auto & info: new_info){
        int info_marker_id = info.get_marker_id();
        // NULL check: valid marker id
        if(auto maybe_player = find_Player(info_marker_id)){
            Player& found_player = *maybe_player;
            // not locked in
            if(!found_player.get_locked_in()){
                // first time detected or player changed the answer
                bool changed_answer = (found_player.get_area() != ui_manager.get_area_of_point(info.get_position_player()));
                bool first_time = (found_player.get_time() == (time_t)(-1));
                if(changed_answer || first_time){
                    found_player.set_position_player(info.get_position_player());
                    found_player.set_time(info.get_time()); 
                } 
                // player has choosen his answer
                else if(difftime(info.get_time(), found_player.get_time()) >= 5){ // difftime() result in seconds
                    found_player.lock_in();
                }

                set_areas();
            }
        }
    }
}
