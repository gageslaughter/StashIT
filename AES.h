#ifndef AES_H
#define AES_H

#include <string>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <openssl/aes.h>


std::string encrypt(const std::string& userInput, const unsigned char* key, const unsigned char* iv);
std::string decrypt(const std::string& encrypted, const unsigned char* key, const unsigned char* iv);

#endif // AES_H
