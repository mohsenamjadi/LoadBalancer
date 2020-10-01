#include "presenter.hpp"

void presenter::config(std::string configLine){
    int index = Tools::findCharIndex(configLine, '@');
    std::string sortingSring = configLine.substr(0, index);
    std::string prcCntSring = configLine.substr(index+1);
    std::cout<< " sortingSring is" << std::endl << std::endl;
    std::cout<< sortingSring << std::endl << std::endl;
    fillMap(sortingSring);
    this -> prcCnt = stoi(prcCntSring);
}

void presenter::fillMap(std::string sortingString){
    std::vector<std::string> sort = Tools::splitBySpace(sortingString);
    for (int i =0; i < sort.size(); i += 1){
        sorting[sort[i]] = sort[i+1];
        std::cout<< " sorting[sort["<<i<<"]] is :" << std::endl << std::endl;
        std::cout<< sorting[sort[i]] << std::endl << std::endl;
    }
}

void presenter::getDataFromWorkers(){
    std::ifstream fd;
    fd.open(WORKERSNAMEDPIPE, std::fstream::in);
    std::string workerRes;
    for (int i = 0; i < prcCnt; i++){
        getline(fd,workerRes);
        std::vector<std::string> res = splitData(workerRes);
        std::vector<std::vector<std::string> > resTable;
        for (int i =0; i < res.size(); i++){
            std::vector<std::string> splitted = Tools::splitBySpace(res[i]);
            resTable.push_back(splitted);
        }
        std::map <std::string,std::string> ::iterator it = sorting.begin();
        sortResult(resTable,it->first,it->second);
        mergeResult(resTable,it->first,it->second);
        for (int i =0; i< finalResult.size(); i++){
            for (int j =0; j < finalResult[i].size(); j++){
                 std::cout << getpid()<< ": "<< finalResult[i][j] << " ";
            }
            std::cout<<std::endl << std::endl;
        }
    }
    fd.close();
}

std::vector<std::string> presenter::splitData(std::string data){
    std::vector<std::string> result;
    std::string token;
    for(int i=0; i<data.length(); i++){
        if(data[i] != '@')
            token += data[i];
        else{
            if(token.length()) {
                result.push_back(token);
                token = "";
            }
        }
    }
    return result;
}

void presenter::sortResult(std::vector<std::vector<std::string> > &res, std::string sortBy,
    std::string type){
    int header;
    for (int i =0; i < res[0].size(); i++){
        if (res[0][i] == sortBy){
            header = i;
            break;
        }
    }
    if (!Tools::isNumber(res[1][header])){
        for (int i = 1; i < res.size(); i++){
            for (int j = 1; j < res.size()-1; j++){
                if(((res[j][header] > res[j+1][header]) && type == ASCEND)||
                    ((res[j][header] < res[j+1][header]) && type == DECSEND)){
                    std::vector<std::string> temp = res[i];
                    res[i] = res[j];
                    res[j] = temp;
                }
            }
        }
    }
    else{
        for (int i = 1; i < res.size(); i++){
            for (int j = 1; j < res.size()-1; j++){
                if(((stoi(res[j][header]) > stoi(res[j+1][header])) && type == ASCEND)||
                    ((stoi(res[j][header]) < stoi(res[j+1][header])) && type == DECSEND)){
                    std::vector<std::string> temp = res[i];
                    res[i] = res[j];
                    res[j] = temp;
                }
            }
        }
    }
}

void presenter::mergeResult(std::vector<std::vector<std::string> > data,std::string sortBy,
    std::string type){
    int header;
    for (int i =0; i < data[0].size(); i++){
        if (data[0][i] == sortBy){
            header = i;
            break;
        }
    }
    int firstSize = finalResult.size();
    int secondSize = data.size();
    int firstPtr = 1, secondPtr = 1;
    std::vector<std::vector<std::string> > temp;
    temp.push_back(data[0]);
    while(firstPtr != firstSize || secondPtr != secondSize){
        if (firstSize == 0){
            temp = data;
            break;
        }
        if(secondSize == 0){
            temp = finalResult;
            break;
        }
        if (!Tools::isNumber(data[1][header])){
            if (firstPtr == firstSize){
                temp.push_back(data[secondPtr]);
                secondPtr++;
            }
            else if (secondPtr == secondSize || secondSize == 0){
                temp.push_back(finalResult[firstPtr]);
                firstPtr++;
            }
            else{
                if(((finalResult[firstPtr][header] > data[secondPtr][header]) && type == ASCEND) ||
                    ((finalResult[firstPtr][header] < data[secondPtr][header]) && type == DECSEND)){
                    temp.push_back(data[secondPtr]);
                    secondPtr++;
                }
                else{
                    temp.push_back(finalResult[firstPtr]);
                    firstPtr++;
                }
            }
        }
        else{
            if (firstPtr == firstSize){
                temp.push_back(data[secondPtr]);
                secondPtr++;
            }
            else if (secondPtr == secondSize){
                temp.push_back(finalResult[firstPtr]);
                firstPtr++;
            }
            else{
                if(((stoi(finalResult[firstPtr][header]) > stoi(data[secondPtr][header])) && type == ASCEND) ||
                    ((stoi(finalResult[firstPtr][header]) < stoi(data[secondPtr][header])) && type == DECSEND)){
                    temp.push_back(data[secondPtr]);
                    secondPtr++;
                }
                else{
                    temp.push_back(finalResult[firstPtr]);
                    firstPtr++;
                }
            }
        }
    }
    finalResult = temp;
}

void presenter::showResult(){
    for (int i =0; i< finalResult.size(); i++){
        for (int j =0; j < finalResult[i].size(); j++){
            std::cout<< finalResult[i][j] << " ";
        }
        std::cout<<std::endl;
    }
}