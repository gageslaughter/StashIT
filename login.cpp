#include "login.h"
#include "AES.cpp"



Login::Login(){
std::fstream file("user.txt");

if (file.good()){
    std::string password = getPassword(true);
    validate(password);
}
else {
    std::fstream file("user.txt", std::ios::out | std::ios::app);
    if (file.is_open()){
        std::string password = getPassword(false);
        validate(password);
        
    }
    else{
    }
}
}

Login::~Login(){



}

void Login::writeRecord(std::fstream &Dout, std::string pin, std::string key, std::string iv) {
const int PIN_WIDTH = 10;
const int KEY_WIDTH = 16;
const int IV_WIDTH = 16;

std::string truncatedPIN = pin.substr(0, 9);
std::string truncatedKey = key.substr(0,15);
std::string truncatedIV = iv.substr(0,15);

Dout << std::setw(PIN_WIDTH) << std::left << truncatedPIN << std::endl;
Dout << std::setw(KEY_WIDTH) << std::left << truncatedKey << std::endl;
Dout << std::setw(IV_WIDTH) << std::left << truncatedIV << std::endl;


}


std::string Login::getPassword(bool fileResult) {
    if(fileResult){
    std::string password;
    char ch;
    struct termios oldt, newt;

    std::cout << "PIN:";
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n') {
        password.push_back(ch);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    std::cout << std::endl;
    return password;
}
else {
    std::string password;
    char ch;
    struct termios oldt, newt;
    std::cout << "Please create a 4 digit PIN:" << std::endl;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    while ((ch = getchar()) != '\n') { 
        password.push_back(ch);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;
    return password;
}

}


bool Login::validate(std::string pin){
    unsigned char key[16];
    unsigned char iv[16];

    if (!RAND_bytes(key, sizeof(key))) {
        std::cerr << "Error generating key!" << std::endl;
        return 1;
    }
    if (!RAND_bytes(iv, sizeof(iv))) {
        std::cerr << "Error generating IV!" << std::endl;
        return 1;
    }

    std::string encrypted_pin = encrypt_pin(pin, key, iv);
    std::cout << "Encrypted PIN (hex): ";
    for (unsigned char c : encrypted_pin) {
        std::cout << std::hex << (int)c;
    }
    std::cout << std::endl;

}
int Login::getPin(){

}
void Login::setPin(){

}