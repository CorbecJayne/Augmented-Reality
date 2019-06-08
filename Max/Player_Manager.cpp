#include "Player_Manager.h"
#include "Player.h"
#include <algorithm>

using namespace std;

bool compare_players (Player i,Player j) {
    return (i.getTime()<j.getTime());
}


/*players get 1000,750,500,250 points
 * only if they are correct
 */
void Player_Manager::give_score() {
    sort(players.begin(),players.end(),compare_players);
    int score=1000;
    for(int i=0;i<players.size();i++){
        // where is correct area stored?
        // "if player is correct"
        if(players[i].getArea()){
            players[i].add_points(score);
            score-=250;
        }
    }
}