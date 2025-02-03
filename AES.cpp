#include "AES.h"

std::string pad(const std::string& input, int block_size) {
    int padding = block_size - (input.size() % block_size);
    std::string padded_input = input;
    padded_input.append(padding, padding);
    return padded_input;
}

std::string encrypt_pin(const std::string& pin, const unsigned char* key, const unsigned char* iv) {
    //std::cout << "PIN PRE ENCRYPT:" << pin << std::endl;
    std::string padded_pin = pad(pin, AES_BLOCK_SIZE);
    //std::cout << "PIN POST pad:" << padded_pin << std::endl;

    unsigned char encrypted_pin[padded_pin.size()];

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating EVP context!" << std::endl;
        return "";
    }


    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv) != 1) {
        std::cerr << "Error initializing encryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int out_len;
    if (EVP_EncryptUpdate(ctx, encrypted_pin, &out_len, reinterpret_cast<const unsigned char*>(padded_pin.c_str()), padded_pin.size()) != 1) {
        std::cerr << "Error during encryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int final_len;
    if (EVP_EncryptFinal_ex(ctx, encrypted_pin + out_len, &final_len) != 1) {
        std::cerr << "Error finalizing encryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    EVP_CIPHER_CTX_free(ctx);

    return std::string(reinterpret_cast<char*>(encrypted_pin));
}

std::string decrypt_pin(const std::string& pin, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "Error creating EVP context!" << std::endl;
        return "";
    }

    const EVP_CIPHER* cipher = EVP_aes_128_cbc();
    if (EVP_DecryptInit_ex(ctx, cipher, nullptr, key, iv) != 1) {
        std::cerr << "Error initializing decryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int out_len, final_len;
    unsigned char decrypted_pin[1024];

    if (EVP_DecryptUpdate(ctx, decrypted_pin, &out_len, reinterpret_cast<const unsigned char*>(pin.c_str()), pin.size()) != 1) {
        std::cerr << "Error during decryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    if (EVP_DecryptFinal_ex(ctx, decrypted_pin + out_len, &final_len) != 1) {
        std::cerr << "Error finalizing decryption!" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    std::string result(reinterpret_cast<char*>(decrypted_pin), out_len + final_len);

    EVP_CIPHER_CTX_free(ctx);

    return result;
}
