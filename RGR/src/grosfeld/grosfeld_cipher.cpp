#include "grosfeld/grosfeld_cipher.h"
#include "utils.h"
#include <stdexcept>

using namespace std;

vector<unsigned char> grosfeldCipher(
    const vector<unsigned char>& data, 
    const string& key, 
    bool encrypt
) {
    if (key.empty()) throw invalid_argument("Ключ не может быть пустым");
    if (data.empty()) return {};
    
    vector<unsigned char> result(data.size());
    const size_t keyLen = key.size();
    
    for (size_t i = 0; i < data.size(); i++) {
        int k = key[i % keyLen] - '0';
        if (encrypt) {
            result[i] = (data[i] + k) % 256;
        } else {
            result[i] = (data[i] - k + 256) % 256;
        }
    }
    
    return result;
}

string generateGrosfeldKey(int length) {
    if (length <= 0) throw invalid_argument("Длина ключа должна быть положительной");
    
    string key;
    key.reserve(length);
    
    for (int i = 0; i < length; i++) {
        key.push_back('0' + randomInt(0, 9));
    }
    
    return key;
}