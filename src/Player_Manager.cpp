#include "headers/Player_Manager.h"
#include "headers/Player.h"

using namespace std;

bool compare_players(Player one,Player two){
    return one.get_time() < two.get_time();
}

void Player_Manager::give_score() {
   sort(players.begin(),players.end(),compare_players);
    int score=1000;
    for(auto & player : players){
        //TODO: maybe get the correct area from Question through main as parameter when function is called?
        // where is correct area stored?
        // "if player is correct"
        if(player.get_area()){
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

Player_Manager::Player_Manager() {
    // players.emplace_back(98);
    // players.emplace_back(23);
    // players.emplace_back(123);
    // players.emplace_back(53);
    
    players.emplace_back(1, 0x1C44);
    players.emplace_back(2, 0x0272);
    players.emplace_back(3, 0x2121);
    players.emplace_back(4, 0x0969);
}

void Player_Manager::reset_players(){
    for(auto & player: players){
        player.reset();
    }
}

void Player_Manager::set_areas(){
    for(auto & player: players){
        //TODO
        player.get_position_player();
        player.set_area(1);
    }
}
	
bool Player_Manager::all_locked(){
    for(auto & player: players){
        if (!player.get_locked_in()) return false;
    }
    return true;
}

std::string Player_Manager::get_scores(){
    std::string result = " | ";
    for(auto & player: players){
        result += std::to_string(player.get_player_id()) + ": " + std::to_string(player.get_points()) + " | ";
    }
}
