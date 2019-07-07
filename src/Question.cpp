
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Question.h"

using namespace std;
const string &Question::getQuestion() const {
    return question;
}

void Question::setQuestion(const string &quest) {
    Question::question = quest;
}

const string &Question::getCorrectAnswer() const {
    return correct_answer;
}

void Question::setCorrectAnswer(const string &correctAnswer) {
    correct_answer = correctAnswer;
}

const string &Question::getWrongAnswerOne() const {
    return wrong_answer_one;
}

void Question::setWrongAnswerOne(const string &wrongAnswerOne) {
    wrong_answer_one = wrongAnswerOne;
}

const string &Question::getWrongAnswerTwo() const {
    return wrong_answer_two;
}

void Question::setWrongAnswerTwo(const string &wrongAnswerTwo) {
    wrong_answer_two = wrongAnswerTwo;
}

const string &Question::getWrongAnswerThree() const {
    return wrong_answer_three;
}

void Question::setWrongAnswerThree(const string &wrongAnswerThree) {
    wrong_answer_three = wrongAnswerThree;
}

int Question::getCorrectPosition() const {
    return correct_position;
}

void Question::setCorrectPosition(const int &pos) {
    correct_position=pos;
}

const string Question::to_string() const {
    stringstream stream;
    vector<string> answers;
    answers.push_back(wrong_answer_one);
    answers.push_back(wrong_answer_two);
    answers.push_back(wrong_answer_three);
    auto rng = default_random_engine {};
    std::shuffle(begin(answers), end(answers), rng);
    stream<<question<<"\n";
    for(int i=0;i<4;i++){
        if(i==correct_position){
          stream<<"question area "<<i<<": "<<correct_answer;
        }else{
            stream<<"question area "<<i<<": "<<answers.at(answers.size()-1);
            answers.pop_back();
        }
        stream<<"\n";
    }
    return stream.str();
}

