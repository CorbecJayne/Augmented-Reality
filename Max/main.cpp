#include <iostream>
#include <string>
#include <sstream>
#include <curl/curl.h>
#include <vector>

using namespace std;

struct question{
    string question;
    string correct_answer;
    string wrong_answer_one;
    string wrong_answer_two;
    string wrong_answer_three;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
static void parseQuestions(const string& input);
static void cleanQuestions();
string replaceAll(std::string str, const std::string& from, const std::string& to);
static void printQuestions();

vector<question>questions;

//PARAMETERS FOR QUERY
int amount=10;              //valid amount 10-50
int category=9;             //general knowledge
const string type="multiple";     //4 answers , #REQUIRED
string query;


int main() {
    CURL *curl;
    CURLcode res;
    string readBuffer;
    stringstream stream;

    stream<<"https://opentdb.com/api.php?amount="<<amount<<"&category="<<category<<"&type="<<type;
    query=stream.str();

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, &query[0u]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        //cout << readBuffer << std::endl;
    }else{
        cout<<"curl failed"<<endl;
    }
    parseQuestions(readBuffer);
    cleanQuestions();
    printQuestions();

    return 0;
}

static void printQuestions(){
    for(int i=0;i<questions.size();i++){
        cout << "Question : " << questions[i].question <<"\ncorrect answer : " <<questions[i].correct_answer<<" incorrect answers : "<<questions[i].wrong_answer_one<<","<<questions[i].wrong_answer_two<<","<<questions[i].wrong_answer_three<<endl;
    }
}

static void cleanQuestions(){
    for(int i=0;i<questions.size();i++){
        questions[i].question=replaceAll(questions[i].question,"&quot;","'");
        questions[i].correct_answer=replaceAll(questions[i].correct_answer,"&quot;","'");
        questions[i].wrong_answer_one=replaceAll(questions[i].wrong_answer_one,"&quot;","'");
        questions[i].wrong_answer_two=replaceAll(questions[i].wrong_answer_two,"&quot;","'");
        questions[i].wrong_answer_three=replaceAll(questions[i].wrong_answer_three,"&quot;","'");

        questions[i].question=replaceAll(questions[i].question,"&#039;","'");
        questions[i].correct_answer=replaceAll(questions[i].correct_answer,"&#039;","'");
        questions[i].wrong_answer_one=replaceAll(questions[i].wrong_answer_one,"&#039;","'");
        questions[i].wrong_answer_two=replaceAll(questions[i].wrong_answer_two,"&#039;","'");
        questions[i].wrong_answer_three=replaceAll(questions[i].wrong_answer_three,"&#039;","'");

        questions[i].question=replaceAll(questions[i].question,"&iacute;","í");
        questions[i].correct_answer=replaceAll(questions[i].correct_answer,"&iacute;","í");
        questions[i].wrong_answer_one=replaceAll(questions[i].wrong_answer_one,"&iacute;","í");
        questions[i].wrong_answer_two=replaceAll(questions[i].wrong_answer_two,"&iacute;","í");
        questions[i].wrong_answer_three=replaceAll(questions[i].wrong_answer_three,"&iacute;","í");
    }
}

static void parseQuestions(const string& input){
    size_t index_question_begin=0;
    size_t index_question_end=0;
    size_t index_incorrect_answers=0;
    size_t index_incorrect_answer_seperator=0;
    string question_text;
    string correct_answer;
    string wrong_answer_one;
    string wrong_answer_two;
    string wrong_answer_three;
    question question;
    for(int i=0;i<amount;i++){
        index_question_begin=input.find("question",index_question_end)+11;
        index_question_end=input.find("correct_answer",index_question_begin)-3;
        index_incorrect_answers=input.find("incorrect_answers",index_question_end)-3;

        question_text=input.substr(index_question_begin,index_question_end-index_question_begin);

        index_question_end+=20  ;
        correct_answer=input.substr(index_question_end,index_incorrect_answers-index_question_end);

        index_incorrect_answers+=24;
        index_incorrect_answer_seperator=input.find(',',index_incorrect_answers)-1;
        wrong_answer_one=input.substr(index_incorrect_answers,index_incorrect_answer_seperator-index_incorrect_answers);

        index_incorrect_answers=input.find(',',index_incorrect_answer_seperator+2)-1;
        index_incorrect_answer_seperator+=3;
        wrong_answer_two=input.substr(index_incorrect_answer_seperator,index_incorrect_answers-index_incorrect_answer_seperator);

        index_incorrect_answer_seperator=input.find(']',index_incorrect_answers)-1;
        index_incorrect_answers+=3;
        wrong_answer_three=input.substr(index_incorrect_answers,index_incorrect_answer_seperator-index_incorrect_answers);

        question.question=question_text;
        question.correct_answer=correct_answer;
        question.wrong_answer_one=wrong_answer_one;
        question.wrong_answer_two=wrong_answer_two;
        question.wrong_answer_three=wrong_answer_three;
        questions.push_back(question);
    }
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string replaceAll(std::string str, const std::string& from, const std::string& to) {
    /*size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;*/
    size_t pos = str.find(from);

    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        str.replace(pos, from.size(), to);
        // Get the next occurrence from the current position
        pos =str.find(from, pos + to.size());
    }
    return str;
}