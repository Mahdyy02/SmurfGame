#include "Cryptographer.h"

Cryptographer::Cryptographer() : encrypted(false) {
    this->char_cipher = {
        {'a', 'd'}, {'b', 'e'}, {'c', 'f'}, {'d', 'g'}, {'e', 'h'},
        {'f', 'i'}, {'g', 'j'}, {'h', 'k'}, {'i', 'l'}, {'j', 'm'},
        {'k', 'n'}, {'l', 'o'}, {'m', 'p'}, {'n', 'q'}, {'o', 'r'},
        {'p', 's'}, {'q', 't'}, {'r', 'u'}, {'s', 'v'}, {'t', 'w'},
        {'u', 'x'}, {'v', 'y'}, {'w', 'z'}, {'x', 'a'}, {'y', 'b'},
        {'z', 'c'}, {' ', ' '}, {':','~'}
    };

    num_cipher = {
        {'0', '5'}, {'1', '9'}, {'2', '7'}, {'3', '0'}, {'4', '2'},
        {'5', '4'}, {'6', '1'}, {'7', '8'}, {'8', '3'}, {'9', '6'}
    };
}

void Cryptographer::encrypt() {
    std::string encrypted_message;
    for (char c : this->message) {
        if (isdigit(c)) {
            encrypted_message += this->num_cipher[c];
        }
        else {
            char transformed_char = this->char_cipher[tolower(c)];
            encrypted_message += islower(c) ? transformed_char : toupper(transformed_char);
        }
    }
    this->message = encrypted_message;
    this->encrypted = true;
}

void Cryptographer::decrypt() {
    std::string decrypted_message;
    for (char c : this->message) {
        if (isdigit(c)) {
            for (const auto& pair : num_cipher) {
                if (pair.second == c) {
                    decrypted_message += pair.first;
                    break;
                }
            }
        }
        else {
            char original_char = c;
            for (const auto& pair : char_cipher) {
                if (pair.second == tolower(c)) {
                    original_char = pair.first;
                    break;
                }
            }
            decrypted_message += islower(c) ? original_char : toupper(original_char);
        }
    }
    this->message = decrypted_message;
    this->encrypted = false;
}

bool Cryptographer::isEncrypted() const {
    return this->encrypted;
}

const std::string& Cryptographer::getMessage() const {
    return this->message;
}