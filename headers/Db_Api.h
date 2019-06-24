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

    static std::string replaceAll(std::string t_source, const std::string& t_old, const std::string& t_new);


    /*
     * amount has to be [10;50]
     * category default -1 means any category
     * category 9 is general knowledge
     * category 18 is computer science
     * difficulty can be empty, easy, medium, hard
     */
    int retrieveQuestions(int t_amount=10,int t_category=-1,std::string t_difficulty="");

    static size_t WriteCallback(void *contents, size_t t_size, size_t t_nmemb, void *userp);

    /* takes the whole response as a string and converts it into questions,
     * placing each into the questions vector
     */
    void parseQuestions(const std::string& t_input,int t_amount);

    void cleanQuestions();

    void printQuestions();

    Question getNextQuestion();
};