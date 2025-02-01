#ifndef AES_H
#define AES_H

#include <string>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <openssl/aes.h>


std::string encrypt_pin(const std::string& pin, const unsigned char* key, const unsigned char* iv);
std::string decrypt_pin(const std::string& encrypted_pin, const unsigned char* key, const unsigned char* iv);

#endif // AES_H
