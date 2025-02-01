#include <iostream>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <cstring>


class Login{

public:
std::string pin;



Login();
~Login();

void writeRecord(std::fstream &Dout, std::string pin, const unsigned char *key, const unsigned char *iv);
void readRecord(std::fstream &Dout);
std::string getPassword(bool fileResult);
bool validate(std::string pin, std::fstream &Dout, bool newUser);
int getPin();
void setPin();



};