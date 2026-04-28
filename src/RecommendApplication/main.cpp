#include <iostream>

#include "application/application.hpp"


int main(int argc, char** argv){
    int N = 10;
    if(argc == 2){
        N = std::stoi(std::string(argv[1]));
    }

    Application application(N);

    application.Run();
    
}