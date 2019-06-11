/* This code uses the API provided by https://opentdb.com/
 * The API is covered by this license: https://creativecommons.org/licenses/by-sa/4.0/
 * No changes to the provided data were made.
 */


#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include <vector>
#include "headers/Db_Api.h"

using namespace std;

void Db_Api::retrieveQuestions(int amount,int category) {
    CURL *curl;
    CURLcode res;
    string readBuffer;
    stringstream stream;
    string query;
    const string type="multiple";     //4 answers , #REQUIRED
    if(amount<10||amount>50){
        cout<<"amount must be [10,50]"<<endl;
        return;
    }
    if(category==-1){
        stream << "https://opentdb.com/api.php?amount=" << amount << "&type=" << type;
    }else {
        stream << "https://opentdb.com/api.php?amount=" << amount << "&category=" << category << "&type=" << type;
    }
    query=stream.str();

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, &query[0u]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

        res = curl_easy_perform(curl);

        if(res!=0){
            cout<<"curl performed returned "<<res<<endl;
        }

        curl_easy_cleanup(curl);
    }else{
        cout<<"curl failed"<<endl;
    }
    replaceAll(readBuffer,"&quot;","'");
    replaceAll(readBuffer,"&#039;","'");
    replaceAll(readBuffer,"&iacute;","í");
    parseQuestions(readBuffer,amount);
    //cleanQuestions();
}

void Db_Api::printQuestions(){
    for(auto & question : questions){
        cout << "Question : " << question.getQuestion() <<"\ncorrect answer : " <<question.getCorrectAnswer()<<" incorrect answers : "<<question.getWrongAnswerOne()<<","<<question.getWrongAnswerTwo()<<","<<question.getWrongAnswerThree()<<endl;
    }
}

void Db_Api::cleanQuestions() {
    for(auto & question : questions){
        question.setQuestion(replaceAll(question.getQuestion(),"&quot;","'"));
        question.setCorrectAnswer(replaceAll(question.getCorrectAnswer(),"&quot;","'"));
        question.setWrongAnswerOne(replaceAll(question.getWrongAnswerOne(),"&quot;","'"));
        question.setWrongAnswerTwo(replaceAll(question.getWrongAnswerTwo(),"&quot;","'"));
        question.setWrongAnswerThree(replaceAll(question.getWrongAnswerThree(),"&quot;","'"));

        question.setQuestion(replaceAll(question.getQuestion(),"&#039;","'"));
        question.setCorrectAnswer(replaceAll(question.getCorrectAnswer(),"&#039;","'"));
        question.setWrongAnswerOne(replaceAll(question.getWrongAnswerOne(),"&#039;","'"));
        question.setWrongAnswerTwo(replaceAll(question.getWrongAnswerTwo(),"&#039;","'"));
        question.setWrongAnswerThree(replaceAll(question.getWrongAnswerThree(),"&#039;","'"));

        question.setQuestion(replaceAll(question.getQuestion(),"&iacute;","í"));
        question.setCorrectAnswer(replaceAll(question.getCorrectAnswer(),"&iacute;","í"));
        question.setWrongAnswerOne(replaceAll(question.getWrongAnswerOne(),"&iacute;","í"));
        question.setWrongAnswerTwo(replaceAll(question.getWrongAnswerTwo(),"&iacute;","í"));
        question.setWrongAnswerThree(replaceAll(question.getWrongAnswerThree(),"&iacute;","í"));
    }
}

void Db_Api::parseQuestions(const string& input,int amount){
    size_t index_question_begin=0;
    size_t index_question_end=0;
    size_t index_incorrect_answers=0;
    size_t index_incorrect_answer_separator=0;
    string question_text;
    string correct_answer;
    string wrong_answer_one;
    string wrong_answer_two;
    string wrong_answer_three;
    Question question;
    for(int i=0;i<amount;i++){
        index_question_begin=input.find("question",index_question_end)+11;
        index_question_end=input.find("correct_answer",index_question_begin)-3;
        index_incorrect_answers=input.find("incorrect_answers",index_question_end)-3;

        question_text=input.substr(index_question_begin,index_question_end-index_question_begin);

        index_question_end+=20  ;
        correct_answer=input.substr(index_question_end,index_incorrect_answers-index_question_end);

        index_incorrect_answers+=24;
        index_incorrect_answer_separator=input.find(',',index_incorrect_answers)-1;
        wrong_answer_one=input.substr(index_incorrect_answers,index_incorrect_answer_separator-index_incorrect_answers);

        index_incorrect_answers=input.find(',',index_incorrect_answer_separator+2)-1;
        index_incorrect_answer_separator+=3;
        wrong_answer_two=input.substr(index_incorrect_answer_separator,index_incorrect_answers-index_incorrect_answer_separator);

        index_incorrect_answer_separator=input.find(']',index_incorrect_answers)-1;
        index_incorrect_answers+=3;
        wrong_answer_three=input.substr(index_incorrect_answers,index_incorrect_answer_separator-index_incorrect_answers);

        question.setQuestion(question_text);
        question.setCorrectAnswer(correct_answer);
        question.setWrongAnswerOne(wrong_answer_one);
        question.setWrongAnswerTwo(wrong_answer_two);
        question.setWrongAnswerThree(wrong_answer_three);
        question.setCorrectPosition(rand()%4);
        questions.push_back(question);
    }
}

size_t Db_Api::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string Db_Api::replaceAll(std::string str, const std::string& from, const std::string& to)  {
    size_t pos = str.find(from);
    while( pos != std::string::npos)
    {
        str.replace(pos, from.size(), to);
        pos =str.find(from, pos + to.size());
    }
    return str;
}

Question Db_Api::getNextQuestion() {
    if(counter<questions.size()) {
        return questions[counter++];
    }else{
        cout<<"no questions left"<<endl;
    }
}

const vector<Question> &Db_Api::getQuestions() const {
    return questions;
}