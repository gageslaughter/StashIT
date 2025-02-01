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

void writeRecord(std::fstream &Dout, std::string pin, std::string key, std::string iv);
std::string getPassword(bool fileResult);
bool validate(std::string pin);
int getPin();
void setPin();



};