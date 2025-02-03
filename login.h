#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <cstring>


class Login{

public:
std::string pin;
const int RECORD_SIZE = 48;
const int PIN_WIDTH = 16;
const int KEY_WIDTH = 16;
const int IV_WIDTH = 16;



Login();
~Login();

void writeRecord(std::fstream &Dout, std::string pin, const unsigned char *key, const unsigned char *iv);
void readRecord(std::fstream &Din, std::string &userPIN);
std::string getPassword();
bool validate(std::string &pin, std::fstream &Dout, bool newUser);
int getPin();
void setPin();



};