//
// Created by evo on 06.06.19.
//
#include <string>
#include "Question.h"
using namespace std;
const string &Question::getQuestion() const {
    return question;
}

void Question::setQuestion(const string &question) {
    Question::question = question;
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
