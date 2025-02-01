#include "login.h"
#include "AES.h"
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>



Login::Login(){
std::fstream file("user.txt");

if (file.good()){
    std::string password = getPassword(true);
    validate(password, file, false);
}
else {
    std::fstream file("user.txt", std::ios::out | std::ios::app);
    if (file.is_open()){
        std::string password = getPassword(false);
        validate(password, file, true);
        
    }
    else{
    }
}
}

Login::~Login(){



}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<unsigned char> stringToUnsignedCharArray(const std::string& str) {
    return std::vector<unsigned char>(str.begin(), str.end());
}

void Login::writeRecord(std::fstream &Dout, std::string pin, const unsigned char *key, const unsigned char *iv) {
    const int PIN_WIDTH = 10;
    const int KEY_WIDTH = 16;
    const int IV_WIDTH = 16;

    std::string truncatedPIN = pin.substr(0, PIN_WIDTH);

    Dout.write(truncatedPIN.c_str(), PIN_WIDTH);

    Dout.write(reinterpret_cast<const char*>(key), KEY_WIDTH);
    Dout.write(reinterpret_cast<const char*>(iv), IV_WIDTH);
}


void Login::readRecord(std::fstream &Din) {
    const int RECORD_SIZE = 42;
    const int PIN_WIDTH = 10;
    const int KEY_WIDTH = 16;
    const int IV_WIDTH = 16;

    Din.seekg(std::ios::beg);

    std::string buffer(RECORD_SIZE, '\0');
    Din.read(&buffer[0], RECORD_SIZE);

    if (Din.gcount() != RECORD_SIZE) {
        std::cerr << "Failed to read complete record.\n";
    }

    std::string pin = buffer.substr(0, PIN_WIDTH);
    std::string key = buffer.substr(PIN_WIDTH, KEY_WIDTH);
    std::string iv = buffer.substr(PIN_WIDTH + KEY_WIDTH, IV_WIDTH);

    
    unsigned char keyArray[KEY_WIDTH];
    unsigned char ivArray[IV_WIDTH];
    std::memcpy(keyArray, key.c_str(), KEY_WIDTH);
    std::memcpy(ivArray, iv.c_str(), IV_WIDTH);


    std::cout << "Key (raw): ";
        for (unsigned char byte : keyArray) {
            std::cout << std::hex << (int)byte << " ";
        }
    std::cout << std::endl;

    std::cout << "IV (raw): ";
        for (unsigned char byte : ivArray) {
            std::cout << std::hex << (int)byte << " ";
        }
    std::cout << std::endl;

    std::string decrypted = decrypt_pin(pin, keyArray, ivArray);
    std::cout << "Decrypted PIN: " << decrypted << std::endl;
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


bool Login::validate(std::string pin, std::fstream &Dout, bool newUser) {
    if (newUser){
    unsigned char key[16];
    unsigned char iv[16];

    if (!RAND_bytes(key, sizeof(key))) {
        std::cerr << "Error generating key!" << std::endl;
        return false;
    }
    if (!RAND_bytes(iv, sizeof(iv))) {
        std::cerr << "Error generating IV!" << std::endl;
        return false;
    }

    std::string encrypted_pin = encrypt_pin(pin, key, iv);
    writeRecord(Dout, encrypted_pin, key, iv);
    /*std::cout << "Encrypted PIN (hex): ";
    for (unsigned char c : encrypted_pin) {
        std::cout << std::hex << (int)c;
    }
    std::cout << std::endl;*/

    return true;  
}
else {
    readRecord(Dout);

}


}


int Login::getPin(){

}
void Login::setPin(){

}