#include "loadBalancer.hpp"
#include <string.h>

void loadBalancer::getInput(){
    getline(std::cin, commandLine);
    //std::cout<< commandLine << std::endl;
}

bool loadBalancer::parseInput(){
    if (commandLine == "quit")
        return 0;
    const char chars[]= { '-' , '=' };
    commandParts = Tools::split(commandLine, chars);
	int lastElement = commandParts.size() - 1;
    for (int i = 0; i < lastElement; i++){
    	    std::cout<< commandParts[i] << std::endl << std::endl;

    }
    this->setFields();
    return 1;
}

void loadBalancer::setFields(){
    int lastElement = commandParts.size() - 1;
    for (int i = 0; i < lastElement-3; i = i+2){
        if (commandParts[i+1] == ASCEND || commandParts[i+1] == DECSEND){
            sortingString += commandParts[i];
            sortingString += " ";
            sortingString += commandParts[i+1];
            sortingString += " ";
            
        }
        
        else{
            filterString += commandParts[i];
            filterString += " ";
            filterString += commandParts[i+1];
            filterString += " ";
        }
    }
    std::cout<< "filter string is :" << std::endl << std::endl;
    std::cout<< filterString << std::endl << std::endl;
    std::cout<< "sorting string is :" << std::endl << std::endl;
	std::cout<< sortingString << std::endl << std::endl;
    prcCnt = stoi(commandParts[lastElement-2]);
    std::cout<< "prcCnt is :" << std::endl << std::endl;
    std::cout<< prcCnt << std::endl << std::endl;
    dirPath = "./" + commandParts[lastElement];
    std::cout<< "dirPath is :" << std::endl << std::endl;
    std::cout<< dirPath << std::endl << std::endl;
}

void loadBalancer::createNamedPipe(){
    mkfifo(MAINNAMEDPIPE,0666);
    mkfifo(WORKERSNAMEDPIPE,0666);
}

void loadBalancer::getFiles(){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dirPath.c_str())) != NULL) {
    	std::cout<< "dir is :" << std::endl << std::endl;
    	std::cout<< dir << std::endl << std::endl;
        while ((ent = readdir (dir)) != NULL) {
        	std::cout<< "ent is :" << std::endl << std::endl;
    		std::cout<< ent << std::endl << std::endl;
            if (strcmp(ent->d_name,".") && strcmp(ent->d_name,"..")){
            	std::cout<< "ent->d_name is :" << std::endl << std::endl;
    			std::cout<< ent->d_name << std::endl << std::endl;
                dirFiles.push_back(ent->d_name);
            }
        }
        closedir (dir);
    } else { /* could not open directory */
        perror ("open directory failed!");
        exit(1);
    }
}

void loadBalancer::devideFilesAndCreateWorkers(){
    const int workerNum = this -> prcCnt;
    int fileCount = dirFiles.size();
    int eachWorkerFiles = fileCount / workerNum;
    int remain = fileCount - (eachWorkerFiles*workerNum);
    for (int i = 0; i < this->prcCnt; i++)
        this-> workerNumFiles.push_back(eachWorkerFiles);
    for (int i = 0; i < remain; i++)
        this-> workerNumFiles[i]++;
    int p[workerNum][2];
    std::cout<< "p[workerNum][2] : p["<<workerNum<<"][2] is :" << std::endl << std::endl;
    std::cout<< p[workerNum][2] << std::endl << std::endl;
    int k = 0;
    for (int i = 0; i < workerNum; i++){
        if (pipe(p[i])== -1){
            perror("pipe() failed!");
            exit(1);
        }
        int pid = fork();
        if (pid == 0){
        	std::cout<< "p["<<i<<"][1] is and it will be closed :" << std::endl << std::endl;
    		std::cout<< p[i][1] << std::endl << std::endl;
            close(p[i][1]);
            char msg[1000];
            std::cout<< "p[i][0] is :" << std::endl << std::endl;
    		std::cout<< p[i][0] << std::endl << std::endl; 
            read(p[i][0], msg, 1000); 
            close(p[i][0]);
            char* argv[3] = {WORKER,msg,NULL};
            std::cout<< "worker and msg are :" << std::endl << std::endl;
    		std::cout<< WORKER << "and msg is " << msg << std::endl << std::endl;
            execv("/home/mohsen/Documents/os-course-project2/testafterdebug/worker", argv);
        }
        else if(pid > 0){
            close(p[i][0]);
            std::string data;
            data = filterString + " @ ";
            std::cout<< "data is :" << std::endl << std::endl;
    		std::cout<< data << std::endl << std::endl;
            for (int j = 0; j < workerNumFiles[i]; j++){
                data = data + dirPath + "/" + dirFiles[k];
                data = data + " ";
                std::cout<< " data is in loop:" << std::endl << std::endl;
    			std::cout<< data << std::endl << std::endl;
                k++;
            }
            write(p[i][1], data.c_str() , (data.length())+1);
            close(p[i][1]);
        }
    }
}
    
void loadBalancer::createPresenter(){
    int pid = fork();
    if(pid == 0){
        std::ifstream fd;
        fd.open(MAINNAMEDPIPE, std::fstream::in);
        std::string configLine;
        getline(fd,configLine);
        std::cout<< " configLine is :" << std::endl << std::endl;
    	std::cout<< configLine << std::endl << std::endl;
        fd.close();
        execl("/home/mohsen/Documents/os-course-project2/testafterdebug/presenter", PRESENTER, configLine.c_str(), NULL);
    }
    else if (pid > 0){
        int fd = open(MAINNAMEDPIPE, O_WRONLY);
        std::string prcCntStr = std::to_string(prcCnt);
        std::string data = sortingString + "@" + prcCntStr + "\n";
        std::cout<< " data in createPresenter finction is :" << std::endl << std::endl;
    	std::cout<< data << std::endl << std::endl;
        write(fd,data.c_str(),(data.length())+1);
        close(fd);
    }
}

void loadBalancer::clear(){
    this-> commandLine = "";
    this-> commandParts.clear();
    this-> prcCnt = 0;
    this-> dirPath = "";
    this-> dirFiles.clear();
    this->workerNumFiles.clear();
}