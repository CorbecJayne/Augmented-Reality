#include <string>

class Question {
    std::string question;
    std::string correct_answer;
    std::string wrong_answer_one;
    std::string wrong_answer_two;
    std::string wrong_answer_three;
	
	// area id
	int correct_position;

public:
    const std::string &getQuestion() const;

    void setQuestion(const std::string &question);

    const std::string &getCorrectAnswer() const;

    void setCorrectAnswer(const std::string &correctAnswer);

    const std::string &getWrongAnswerOne() const;

    void setWrongAnswerOne(const std::string &wrongAnswerOne);

    const std::string &getWrongAnswerTwo() const;

    void setWrongAnswerTwo(const std::string &wrongAnswerTwo);

    const std::string &getWrongAnswerThree() const;

    void setWrongAnswerThree(const std::string &wrongAnswerThree);
	
	int get_correct_position();
	
	//constructor with random value for correct position
};