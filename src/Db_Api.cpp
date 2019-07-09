/* This code uses the API provided by https://opentdb.com/
 * The API is covered by this license: https://creativecommons.org/licenses/by-sa/4.0/
 * No changes to the provided data were made.
 */


#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include <vector>
#include "Db_Api.h"

using namespace std;

int Db_Api::retrieveQuestions(int t_amount,int t_category,const string& t_difficulty) {
    CURL *curl;
    CURLcode res;
    string readBuffer;
    stringstream stream;
    string query;
    string diff;
    const string type="multiple";     //4 answers , #REQUIRED
    if(t_amount<10||t_amount>50){
        cout<<"amount must be [10,50]"<<endl;
        return -1;
    }
    if(t_difficulty.empty()){
        diff="";
    }else{
        diff="&difficulty="+t_difficulty;
    }
    if(t_category==-1){
        stream << "https://opentdb.com/api.php?amount=" << t_amount << diff <<"&type=" << type;
    }else {
        stream << "https://opentdb.com/api.php?amount=" << t_amount << "&category=" << t_category << diff << "&type=" << type;
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
            cout<<"curl perform returned "<<res<<endl;
        }

        curl_easy_cleanup(curl);
    }else{
        cout<<"curl failed"<<endl;
    }
    int index_response_code=readBuffer.find("response_code");
    string response_code=readBuffer.substr(index_response_code+15,1);
    if(response_code!="0"){
        return -1;
    }
    readBuffer=replaceAll(readBuffer,"&quot;","'");
    readBuffer=replaceAll(readBuffer,"&#039;","'");
    readBuffer=replaceAll(readBuffer,"&iacute;","í");
    readBuffer=replaceAll(readBuffer,"&amp;","&");
    readBuffer=replaceAll(readBuffer,"&ldquo;","'");
    readBuffer=replaceAll(readBuffer,"&rdquo;","'");
    readBuffer=replaceAll(readBuffer,"&rsquo;","'");
    readBuffer=replaceAll(readBuffer,"&lsquo;","'");
    readBuffer=replaceAll(readBuffer,"&scaron;","Š");
    readBuffer=replaceAll(readBuffer,"&Eacute;","É");
    readBuffer=replaceAll(readBuffer,"&eacute;","é");

    parseQuestions(readBuffer,t_amount);

    return 0;
}

void Db_Api::parseQuestions(const string& t_input,int t_amount){
    size_t index_question_begin=0;
    size_t index_question_end=0;
    size_t index_incorrect_answers=0;
    size_t index_incorrect_answer_separator=0;
    string question_text;
    string correct_answer;
    string wrong_answer_one;
    string wrong_answer_two;
    string wrong_answer_three;
    for(int i=0;i<t_amount;i++){
        Question question;
        index_question_begin=t_input.find("question",index_question_end)+11;
        index_question_end=t_input.find("correct_answer",index_question_begin)-3;
        index_incorrect_answers=t_input.find("incorrect_answers",index_question_end)-3;

        question_text=t_input.substr(index_question_begin,index_question_end-index_question_begin);

        index_question_end+=20  ;
        correct_answer=t_input.substr(index_question_end,index_incorrect_answers-index_question_end);

        index_incorrect_answers+=24;
        index_incorrect_answer_separator=t_input.find(',',index_incorrect_answers)-1;
        wrong_answer_one=t_input.substr(index_incorrect_answers,index_incorrect_answer_separator-index_incorrect_answers);

        index_incorrect_answers=t_input.find(',',index_incorrect_answer_separator+2)-1;
        index_incorrect_answer_separator+=3;
        wrong_answer_two=t_input.substr(index_incorrect_answer_separator,index_incorrect_answers-index_incorrect_answer_separator);

        index_incorrect_answer_separator=t_input.find(']',index_incorrect_answers)-1;
        index_incorrect_answers+=3;
        wrong_answer_three=t_input.substr(index_incorrect_answers,index_incorrect_answer_separator-index_incorrect_answers);

        question.setQuestion(question_text);
        question.setCorrectAnswer(correct_answer);
        question.setWrongAnswerOne(wrong_answer_one);
        question.setWrongAnswerTwo(wrong_answer_two);
        question.setWrongAnswerThree(wrong_answer_three);
        question.setCorrectPosition(rand()%4);
        questions.push_back(question);
    }
}

size_t Db_Api::WriteCallback(void *contents, size_t t_size, size_t t_nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, t_size * t_nmemb);
    return t_size * t_nmemb;
}

string Db_Api::replaceAll(std::string t_source, const std::string& t_old, const std::string& t_new)  {
    size_t pos = t_source.find(t_old);
    while( pos != std::string::npos)
    {
        t_source.replace(pos, t_old.size(), t_new);
        pos =t_source.find(t_old, pos + t_new.size());
    }
    return t_source;
}

Question Db_Api::getNextQuestion() {
    if(counter<questions.size()) {
        return questions[counter++];
    }else{
        cout<<"no questions left"<<endl;
        return Question();
    }
}

const vector<Question> &Db_Api::getQuestions() const {
    return questions;
}