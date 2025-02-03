#include "storage.h"






storage::storage() {
    std::fstream file("user.data", std::ios::in | std::ios::out | std::ios::binary | std::ios::app);

    if (!file.is_open()) {
        file.open("user.data", std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error Opening File" << std::endl;
        }
    }
}

storage::~storage(){

}



void storage::writeRecord(std::fstream &Dout, std::string &name, std::string &password, const unsigned char *key, const unsigned char *iv,const unsigned char *key2, const unsigned char *iv2){
    std::string truncatedName = name.substr(0, NAME_WIDTH);
    std::string truncatedPass = password.substr(0, PASS_WIDTH);
    std::string paddedName = truncatedName;
    std::string paddedPass = truncatedPass;
    paddedName.resize(NAME_WIDTH, '\0');
    paddedName.resize(PASS_WIDTH, '\0');
    Dout.write(paddedName.c_str(), NAME_WIDTH);
    Dout.write(paddedName.c_str(), NAME_WIDTH);
    Dout.write(reinterpret_cast<const char*>(key), KEY_WIDTH);
    Dout.write(reinterpret_cast<const char*>(iv), IV_WIDTH);
    Dout.write(reinterpret_cast<const char*>(key2), KEY_WIDTH2);
    Dout.write(reinterpret_cast<const char*>(iv2), IV_WIDTH2);
}



void storage::readRecord(std::fstream &Din, std::string &Name) {
    
    
    
    
    //search for name after its already sorted
    
    
    
    //instead of 0, we will use record size * index
    Din.seekg(0, std::ios::beg);
    unsigned char keyArray[KEY_WIDTH];
    unsigned char ivArray[IV_WIDTH];
    unsigned char keyArray2[KEY_WIDTH];
    unsigned char ivArray2[IV_WIDTH];

    std::string buffer(RECORD_SIZE, NULL);
    Din.read(&buffer[0], RECORD_SIZE);
    Din.gcount();
    if (Din.gcount() != RECORD_SIZE) {
    std::cerr << "Failed to read complete record. Read only " << Din.gcount() << " bytes.\n";
    }

    std::string name = buffer.substr(0, NAME_WIDTH);
    std::string pass = buffer.substr(NAME_WIDTH, PASS_WIDTH);
    std::vector<unsigned char> keyVec(buffer.begin() + NAME_WIDTH + PASS_WIDTH, buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH);
    std::vector<unsigned char> ivVec(buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH, buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH + IV_WIDTH);



    std::vector<unsigned char> keyVec2(buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH + IV_WIDTH, buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH + IV_WIDTH + KEY_WIDTH2);
    std::vector<unsigned char> ivVec2(buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH + IV_WIDTH + KEY_WIDTH2, buffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH + IV_WIDTH + KEY_WIDTH2 + IV_WIDTH2);
    
    std::memcpy(keyArray, keyVec.data(), KEY_WIDTH);
    std::memcpy(ivArray, ivVec.data(), IV_WIDTH);
    std::memcpy(keyArray2, keyVec2.data(), KEY_WIDTH2);
    std::memcpy(ivArray2, ivVec2.data(), IV_WIDTH2);

//std::cout << "Reading from: " <<  PIN_WIDTH + KEY_WIDTH << "-" << PIN_WIDTH + KEY_WIDTH + IV_WIDTH << std::endl;

//std::cout << "Reading from: " <<  PIN_WIDTH << "-" << PIN_WIDTH+KEY_WIDTH << std::endl;
//std::cout << "Reading from: 0-" << PIN_WIDTH << std::endl;
    //std::cout << "PIN (PRE): " << pin << std::endl;

    
    std::string decryptedName = decrypt(name, keyArray, ivArray);
    std::string decryptedPass = decrypt(pass, keyArray, ivArray);
    
    std::cout << "Decrypted NAME: " << decryptedName << std::endl;
    std::cout << "Decrypted PASS: " << decryptedPass << std::endl;
    
}


bool storage::addPassword(std::string &name, std::string &password){
    std::fstream file;
    unsigned char key[16];
    unsigned char iv[16];
    unsigned char key2[16];
    unsigned char iv2[16];


    file.open("user.data");
    
    if(!file.is_open()){
        std::cerr << "Error Opening File" << std::endl;
    }

    if (!RAND_bytes(key, sizeof(key))) {
        std::cerr << "Error generating key!" << std::endl;
        return false;
    }
    if (!RAND_bytes(iv, sizeof(iv))) {
        std::cerr << "Error generating IV!" << std::endl;
        return false;
    }
    if (!RAND_bytes(key2, sizeof(key))) {
        std::cerr << "Error generating key!" << std::endl;
        return false;
    }
    if (!RAND_bytes(iv2, sizeof(iv))) {
        std::cerr << "Error generating IV!" << std::endl;
        return false;
    }

    std::string encrypted_Name = encrypt(name, key, iv);
    std::string encrypted_Pass = encrypt(password, key2, iv2);
    writeRecord(file, encrypted_Name, encrypted_Pass, key, iv, key2, iv2);



return true;
}


bool storage::searchPassword(std::string &name){
std::fstream file;
file.open("user.data");
    
    if(!file.is_open()){
        std::cerr << "Error Opening File" << std::endl;
    }
readRecord(file, name);
return true;
}

