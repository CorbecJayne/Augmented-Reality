#include "Db_Api.h"

int main(){
    Db_Api dbApi;
    dbApi.retrieveQuestions();
    dbApi.printQuestions();
    return 0;
}
