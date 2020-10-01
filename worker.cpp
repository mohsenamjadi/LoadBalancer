#include "worker.hpp"

void worker::setFields(std::string data){
    int index = Tools::findCharIndex(data, '@');
    std::string filters = data.substr(0, index);
    std::string files = data.substr(index+2);
    std::cout<< " filters is :" << std::endl << std::endl;
    std::cout<< filters << std::endl << std::endl;
    std::cout<< " files is :" << std::endl << std::endl;
    std::cout<< files << std::endl << std::endl;
    fillMap(filters);
    setFiles(files);
}

void worker::fillMap(std::string filterString){
    std::vector<std::string> filter;
    filter = Tools::splitBySpace(filterString);
    for (int i = 0; i < filter.size(); i = i+2){
        filters[filter[i]] = filter[i+1];
        std::cout<< " filters[filter[i]] is :" << std::endl << std::endl;
        std::cout<< filters[filter[i]] << std::endl << std::endl;
    }
}

void worker::setFiles(std::string fileString){
    files = Tools::splitBySpace(fileString);

}

void worker::processFiles(){
    std::ifstream fs;
    std::string line;
    for (int i = 0; i < files.size(); i++){
        fs.open(files[i],std::fstream::in);
        if (!fs) {
            std::cerr << "Unable to open file!";
            exit(1);  
        }
        getline(fs, line);
        std::cout<< " line is :" << std::endl << std::endl;
        std::cout<< line << std::endl << std::endl;
        if (searchTable.size()==0){
            std::vector<std::string> header = Tools::splitBySpace(line);
            searchTable.push_back(header);

        }
        while(getline(fs,line)){
            std::vector<std::string> l = Tools::splitBySpace(line);
            /*for (int i =0; i< l.size(); i++){
                for (int j =0; j < l[i].size(); j++){
                    std::cout<< " l[i] is :" << std::endl << std::endl;
                    std::cout<< l[i] << std::endl << std::endl;
            }    
                }*/
            searchTable.push_back(l);
        }
        /*for (int i =0; i< searchTable.size(); i++){
                for (int j =0; j < searchTable[i].size(); j++){
                std::cout<< " searchTable[i] is :" << std::endl << std::endl;
                std::cout<< searchTable[i][j] << std::endl << std::endl;
                }
        }*/
        fs.close();
            
    }
}

void worker::search(){
    int header;
    for(std::map <std::string,std::string> ::iterator it = filters.begin(); it != filters.end(); ++it) {
        std::cout<< " it-> first is :" << std::endl << std::endl;
        std::cout<< it -> first << std::endl << std::endl;
        std::cout<< " it-> second is :" << std::endl << std::endl;
        std::cout<< it -> second << std::endl << std::endl;
        for (int i =0; i < searchTable[0].size(); i++){
            if (searchTable[0][i] == it -> first){
                header = i;
                std::cout<< " header is :" << std::endl << std::endl;
                std::cout<< header << std::endl << std::endl;
                std::cout<< " searchTable[0] ["<< i <<"] is :"  << std::endl << std::endl;
                std::cout<< searchTable[0][i] << std::endl << std::endl;
                break;
            }
        }
        for(int i =searchTable.size()-1; i > 0; i--){
            if(searchTable[i][header] != it -> second)
                std::cout<< " this is searchTable[i][header] :" << i << header << std::endl << std::endl;
                std::cout<< searchTable[i][header] << std::endl << std::endl;
                searchTable.erase(searchTable.begin()+i);
        }   
    }
    for (int i =0; i< searchTable.size(); i++){
        for (int j =0; j < searchTable[i].size(); j++){
            std::cout<< " searchTable[i] infunctionsearch is :" << std::endl << std::endl;
            std::cout<< searchTable[i][j] << std::endl << std::endl;
        }
    }
}

void worker::makeDataReady(){
    for (int i =0; i< searchTable.size(); i++){
        for (int j =0; j < searchTable[i].size(); j++){
            /*std::cout<< " searchTable[i][j] is :" << std::endl << std::endl;
            std::cout<< searchTable[i][j] << std::endl << std::endl;*/
            dataForPresenter += searchTable[i][j] += " ";
        }
        dataForPresenter += " @";
    }
    dataForPresenter += "\n";
    std::cout<< " dataForPresenter is :" << std::endl << std::endl;
    std::cout<< dataForPresenter << std::endl;
}

void worker::sendDataToPresenter(){
    int fd = open(WORKERSNAMEDPIPE, O_WRONLY);
    write(fd,dataForPresenter.c_str(),(dataForPresenter.length())+1);
    close(fd);
}