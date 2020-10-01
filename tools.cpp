#include "tools.hpp"

using namespace std;

vector<string> Tools::splitBySpace(string statement){
    vector<string> result;
    string token;
    for(int i=0; i<statement.length(); i++){
        if(statement[i] != ' ')
            token += statement[i];
        else{
            if(token.length()) {
                result.push_back(token);
                token = "";
            }
        }
    }
    if (token != ""){
        result.push_back(token);
    }
    return result;
}

vector<string> Tools::split(string statement , const char* delimeters){
    vector<string> result;
    string token;
    for(int i=0; i<statement.length(); i++){
        for (int j = 0; j < sizeof(delimeters); j++){
            if(statement[i] != ' ' && statement[i] != delimeters[j]){
                if (j == sizeof(delimeters)-1){
                    token += statement[i];
                }
            }
            else{
                if(token.length()) {
                    result.push_back(token);
                    token = "";
                }
                break;
            }
        }
    }
    if (token != ""){
        result.push_back(token);
    }
    return result;
}

int Tools::findCharIndex(std::string s, char c){
    for (int i = 0; i < s.length(); i++){
        if (s[i] == c)
            return i;
    }
    return -1;
}

bool Tools::isNumber(const std::string& s){
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}