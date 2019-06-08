#include "Db_Api.h"
#include "Player_Manager.h"
#include <iostream>

using namespace std;

int main(){
    Db_Api dbApi;
    dbApi.retrieveQuestions();
    Player_Manager playerManager;
    playerManager.give_score();
    return 0;
}
