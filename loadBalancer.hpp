#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>

#include "tools.hpp"
#include "define.hpp"

class loadBalancer{
private:
    std::string commandLine;
    std::vector<std::string> commandParts;
    std::string filterString;
    std::string sortingString;
    int prcCnt;
    std::string dirPath;
    std::vector<std::string> dirFiles;
    std::vector<int> workerNumFiles;
public:
    void getInput();
    bool parseInput();
    void setFields();
    void createNamedPipe();
    void getFiles();
    void devideFilesAndCreateWorkers();
    void createPresenter();
    void clear();
    int getProcCount(){return prcCnt;}
};

#endif