#include <vector>
#include <string>
#include "Question.h"


class Db_Api{
    std::vector<Question>questions;
    int counter;
public:

    Db_Api(){
        counter=0;
    }
    ~Db_Api()= default;

    const std::vector<Question> &getQuestions() const;

    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);


    /*
     * amount has to be [10;50]
     * category default -1 means any category
     * category 9 is general knowledge
     * category 18 is computer science
     * difficulty can be empty, easy, medium, hard
     */
    int retrieveQuestions(int amount=10,int category=-1,std::string difficulty="");

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    /* takes the whole response as a string and converts it into questions,
     * placing each into the questions vector
     */
    void parseQuestions(const std::string& input,int amount);

    void cleanQuestions();

    void printQuestions();

    Question getNextQuestion();
};