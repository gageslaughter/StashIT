#include <iostream>
#include <fstream> 
#include "login.h"
#include <string.h>





void menu(){
    std::cout << "----------StashIT----------" << std::endl;
    std::cout << "1) Add Password" << std::endl;
    std::cout << "2) Search Password" << std::endl;
    std::cout << "3) Close" << std::endl;
}

int main(){
std::string pin;
int choice;
bool quit = false;

system("clear");
Login login;
system("clear");







while(!quit){
    menu();
    std::cin >> choice;
    switch (choice){
        case 1:{
        
        break;
        }

        case 2:{

        break;
        }
        
        case 3:{
            quit = true;
        break;
    }
    }
    }

    return 0;
}