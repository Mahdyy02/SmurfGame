#ifndef CRYPTOGRAPHER_H
#define CRYPTOGRAPHER_H

#include <iostream>
#include <string>
#include <unordered_map>

class Cryptographer {
private:
    bool encrypted;  
    std::unordered_map<char, char> char_cipher;  
    std::unordered_map<char, char> num_cipher;     

public:

    std::string message;

    Cryptographer();
    ~Cryptographer(){}

    void encrypt();
    void decrypt();
    bool isEncrypted() const;
    const std::string& getMessage() const;
};


#endif