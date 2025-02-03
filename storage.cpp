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



void storage::writeRecord(std::fstream &Dout, std::string &name, std::string &password, const unsigned char *key, const unsigned char *iv, const unsigned char *key2, const unsigned char *iv2) {
    std::string truncatedName = name.substr(0, NAME_WIDTH);
    std::string truncatedPass = password.substr(0, PASS_WIDTH);
    std::string paddedName = truncatedName;
    std::string paddedPass = truncatedPass;
    
    paddedName.resize(NAME_WIDTH, '\0');
    paddedPass.resize(PASS_WIDTH, '\0');
    Dout.seekp(0, std::ios::end); 

    Dout.write(paddedName.c_str(), NAME_WIDTH);
    Dout.write(paddedPass.c_str(), PASS_WIDTH);
    Dout.write(reinterpret_cast<const char*>(key), KEY_WIDTH);
    Dout.write(reinterpret_cast<const char*>(iv), IV_WIDTH);
    Dout.write(reinterpret_cast<const char*>(key2), KEY_WIDTH2);
    Dout.write(reinterpret_cast<const char*>(iv2), IV_WIDTH2);
}




void storage::readRecord(std::fstream &Din, std::string &Name) {
    std::string searchBuffer(RECORD_SIZE, '\0');
    
    Din.seekg(0, std::ios::end);
    int n = Din.tellg() / RECORD_SIZE;
    //std::cout << n << std::endl;
    Din.seekg(0, std::ios::beg);  
    
    int pos = -1;  
    
    for (int i = 0; i < n; ++i) {

        Din.seekg(i * RECORD_SIZE, std::ios::beg);
        Din.read(&searchBuffer[0], RECORD_SIZE);
        
        std::string encryptedName = searchBuffer.substr(0, NAME_WIDTH);
        //std::string paddedEncryptedName = encryptedName;
        //paddedEncryptedName.resize(NAME_WIDTH, '\0');
        
        std::vector<unsigned char> keyVec(searchBuffer.begin() + NAME_WIDTH + PASS_WIDTH, searchBuffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH);
        std::vector<unsigned char> ivVec(searchBuffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH, searchBuffer.begin() + NAME_WIDTH + PASS_WIDTH + KEY_WIDTH + IV_WIDTH);

        unsigned char keyArray[KEY_WIDTH];
        unsigned char ivArray[IV_WIDTH];
        
        std::memcpy(keyArray, keyVec.data(), KEY_WIDTH);
        std::memcpy(ivArray, ivVec.data(), IV_WIDTH);
        
        
        std::string encryptedSearch = encrypt(Name, keyArray, ivArray);
        std::string truncatedsearch = encryptedSearch.substr(0,NAME_WIDTH);
        //std::cout << "ENCRYPT FILE - " << encryptedName << std::endl;
        //std::cout << "ENCRYPTED SEARCH - " << truncatedsearch << std::endl;



        std::string decryptedName = decrypt(encryptedName, keyArray, ivArray);
        std::string decryptedSearch = decrypt(truncatedsearch, keyArray, ivArray);
        //std::cout << "DECYRPT FILE - " << decryptedName << std::endl;
        //std::cout << "DECYRPTED SEARCH - " << decryptedSearch << std::endl;


        /*std::cout << "Key (pre): ";
        for (unsigned char byte : keyArray) {
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::endl;

        std::cout << "IV (pre): ";
            for (unsigned char byte : ivArray) {
                std::cout << std::hex << (int)byte << " ";
            }
        std::cout << std::endl;
*/

    int result = decryptedSearch.compare(decryptedName);
    //std::cout << result << std::endl;

        if (result == 0) {
            pos = Din.tellg();
            std::cout << "Record found at byte: " << pos << std::endl;
            break;
        }
        else std::cout << "Record not found!" << std::endl;
    }
    
        
    
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
    //std::cout << "Initial Encrypt " << encrypted_Name << std::endl;
    std::string encrypted_Pass = encrypt(password, key2, iv2);


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

