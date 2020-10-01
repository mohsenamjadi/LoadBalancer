#ifndef WORKER_HPP
#define WORKER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tools.hpp"
#include "define.hpp"

class worker{
private:
    std::map <std::string,std::string> filters;
    std::vector<std::string> files;
    std::vector<std::vector<std::string> > searchTable;
    std::string dataForPresenter;
public:
    void setFields(std::string data);
    void fillMap(std::string filterString);
    void setFiles(std::string fileString);
    void processFiles();
    void search();
    void makeDataReady();
    void sendDataToPresenter();
};

#endif 
