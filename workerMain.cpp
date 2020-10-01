#include <iostream>
#include <unistd.h>
#include "worker.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    worker w;
    w.setFields(argv[1]);
    w.processFiles();
    w.search();
    w.makeDataReady();
    w.sendDataToPresenter();
    return 0;
}