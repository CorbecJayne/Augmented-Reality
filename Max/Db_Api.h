#include <vector>
#include <string>
#include "Question.h"
class Db_Api{
    std::vector<Question>questions;
public:

    Db_Api()= default;
    ~Db_Api()= default;

    const std::vector<Question> &getQuestions() const;

    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);


    /*
     * amount has to be [10;50]
     * category default -1 means any category
     * category 9 is general knowledge
     *
     */
    void retrieveQuestions(int amount=10,int category=-1);

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

    void parseQuestions(const std::string& input,int amount);

    void cleanQuestions();

    void printQuestions();
};