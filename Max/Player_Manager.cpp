#include "Player_Manager.h"
#include "Player.h"

using namespace std;

bool compare_players(Player one,Player two){
    return one.getTime()<two.getTime();
}

void Player_Manager::give_score() {
   sort(players.begin(),players.end(),compare_players);
    int score=1000;
    for(auto & player : players){
        // where is correct area stored?
        // "if player is correct"
        if(player.getArea()){
            player.add_points(score);
            score-=250;
        }
    }
}



Player_Manager::~Player_Manager() = default;

const vector<Player> &Player_Manager::getPlayers() const {
    return players;
}

void Player_Manager::setPlayers(const vector<Player> &players) {
    Player_Manager::players = players;
}

Player_Manager::Player_Manager() {
    players.emplace_back(98);
    players.emplace_back(23);
    players.emplace_back(123);
    players.emplace_back(53);
}
