#include <iostream>
#include <sys/wait.h>
#include "loadBalancer.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    loadBalancer lb;
    while (true){
        lb.getInput();
        if (!lb.parseInput())
            break;
        lb.createNamedPipe();
        lb.getFiles();
        lb.devideFilesAndCreateWorkers();		
        lb.createPresenter();
        int prcCnt = lb.getProcCount();
        for (int i = 0; i < prcCnt+1; i++){
            wait(NULL);
        }

        lb.clear();
    }
    return 0;
}
