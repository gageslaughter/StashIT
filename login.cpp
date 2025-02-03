#include "login.h"
#include "AES.h"
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>



Login::Login() {
    std::fstream file("user.txt", std::ios::in | std::ios::binary);

    if (file.is_open()) {
        std::cout << "PIN: ";
        std::string password = getPassword();
        validate(password, file, false);
    } 
    else {
        file.open("user.txt", std::ios::out | std::ios::binary);
        
        if (file.is_open()) {
            std::cout << "Please create a PIN: ";
            std::string password = getPassword();
            validate(password, file, true);
        }
        else {
            std::cerr << "Error opening file for writing!\n";
        }
    }
}



Login::~Login(){



}

std::vector<unsigned char> stringToUnsignedCharArray(const std::string& str) {
    return std::vector<unsigned char>(str.begin(), str.end());
}

void Login::writeRecord(std::fstream &Dout, std::string pin, const unsigned char *key, const unsigned char *iv) {
    std::string truncatedPIN = pin.substr(0, PIN_WIDTH);
    std::string paddedPIN = truncatedPIN;
    paddedPIN.resize(PIN_WIDTH, '\0');
    Dout.write(paddedPIN.c_str(), PIN_WIDTH);
    Dout.write(reinterpret_cast<const char*>(key), KEY_WIDTH);
    Dout.write(reinterpret_cast<const char*>(iv), IV_WIDTH);
}


void Login::readRecord(std::fstream &Din, std::string &userPIN) {
    Din.seekg(0, std::ios::beg);
    unsigned char keyArray[KEY_WIDTH];
    unsigned char ivArray[IV_WIDTH];

    std::string buffer(RECORD_SIZE, NULL);
    Din.read(&buffer[0], RECORD_SIZE);
    Din.gcount();
    if (Din.gcount() != RECORD_SIZE) {
    std::cerr << "Failed to read complete record. Read only " << Din.gcount() << " bytes.\n";
    }

    std::string pin = buffer.substr(0, PIN_WIDTH);
    //std::cout << "Reading from: 0-" << PIN_WIDTH << std::endl;
    std::vector<unsigned char> keyVec(buffer.begin() + PIN_WIDTH, buffer.begin() + PIN_WIDTH + KEY_WIDTH);
    //std::cout << "Reading from: " <<  PIN_WIDTH << "-" << PIN_WIDTH+KEY_WIDTH << std::endl;
    std::vector<unsigned char> ivVec(buffer.begin() + PIN_WIDTH + KEY_WIDTH, buffer.begin() + PIN_WIDTH + KEY_WIDTH + IV_WIDTH);
    //std::cout << "Reading from: " <<  PIN_WIDTH + KEY_WIDTH << "-" << PIN_WIDTH + KEY_WIDTH + IV_WIDTH << std::endl;
    std::memcpy(keyArray, keyVec.data(), KEY_WIDTH);
    std::memcpy(ivArray, ivVec.data(), IV_WIDTH);



    //std::cout << "PIN (PRE): " << pin << std::endl;
/**/
    
    std::string decrypted = decrypt_pin(pin, keyArray, ivArray);
    if(userPIN != decrypted){
        std::cout << "Incorrect PIN" << std::endl;
        exit(0);
    }
    //std::cout << "Decrypted PIN: " << decrypted << std::endl;
    
}




std::string Login::getPassword() {
    std::string password;
    char ch;
    struct termios oldt, newt;

    
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



bool Login::validate(std::string &pin, std::fstream &Dout, bool newUser) {
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

    std::cout << "PIN (PRE): " << pin << std::endl;
/*
    std::cout << "Key (pre): ";
        for (unsigned char byte : key) {
            std::cout << std::hex << (int)byte << " ";
        }
    std::cout << std::endl;

    std::cout << "IV (pre): ";
        for (unsigned char byte : iv) {
            std::cout << std::hex << (int)byte << " ";
        }
    std::cout << std::endl;
    */

    std::string encrypted_pin = encrypt_pin(pin, key, iv);
    writeRecord(Dout, encrypted_pin, key, iv);

    return true;  
}
else {
    readRecord(Dout, pin);

}


}


int Login::getPin(){

}
void Login::setPin(){

}