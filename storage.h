#include <fstream> 
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "AES.h"


class storage{
const int RECORD_SIZE = 96;
const int NAME_WIDTH = 16;
const int PASS_WIDTH = 16;
const int KEY_WIDTH = 16;
const int IV_WIDTH = 16;
const int KEY_WIDTH2 = 16;
const int IV_WIDTH2 = 16;





public:

storage();
~storage();

void writeRecord(std::fstream &Dout, std::string &name, std::string &password, const unsigned char *key, 
const unsigned char *iv, const unsigned char *key2, const unsigned char *iv2);
void readRecord(std::fstream &Din, std::string &Name);
bool addPassword(std::string &name, std::string &password);
bool searchPassword(std::string &name);



};