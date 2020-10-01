#include <iostream> 

#include "presenter.hpp"

using namespace std;

int main(int argc, char const *argv[]){
    presenter pr;
    pr.config(argv[1]);
    pr.getDataFromWorkers();
    pr.showResult();
    return 1;
}