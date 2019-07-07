#include <string>
#include <experimental/random>

class Question {
    std::string question;
    std::string correct_answer;
    std::string wrong_answer_one;
    std::string wrong_answer_two;
    std::string wrong_answer_three;

    //area id
    int correct_position;

public:

    const std::string &getQuestion() const;

    void setQuestion(const std::string &quest);

    const std::string &getCorrectAnswer() const;

    void setCorrectAnswer(const std::string &correctAnswer);

    const std::string &getWrongAnswerOne() const;

    void setWrongAnswerOne(const std::string &wrongAnswerOne);

    const std::string &getWrongAnswerTwo() const;

    void setWrongAnswerTwo(const std::string &wrongAnswerTwo);

    const std::string &getWrongAnswerThree() const;

    void setWrongAnswerThree(const std::string &wrongAnswerThree);

    void setCorrectPosition(const int &pos);

    int getCorrectPosition() const;

    //to string function: question + answer possibilities - use random position of correct_position in string
    const std::string to_string() const;
};

