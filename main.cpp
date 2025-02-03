#include <iostream>
#include <fstream> 
#include "login.h"
#include "storage.h"
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
storage storage;
system("clear");







while(!quit){
    menu();
    std::cin >> choice;
    std::string userInput1, userInput2, userInput3;
    int selection = 0;
    switch (choice){
        case 1:{
        std::cout << "What would you like to call this Password?" << std::endl;
        std::cin >> userInput1;
        while(selection != 1){
        std::cout << "What is the password?" << std::endl;
        std::cin >> userInput2;
        std::cout << "Is your password correct?" << '\n' << userInput2 << std::endl;
        std::cout << "1)Yes - 2) No" << std::endl;
        std::cin >> selection;
        if(selection == 1){
            system("clear");
            storage.addPassword(userInput1, userInput2);
        }
        }
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