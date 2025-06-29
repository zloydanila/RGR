#ifndef GROSFELD_CIPHER_H
#define GROSFELD_CIPHER_H

#include <vector>
#include <string>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

vector<unsigned char> grosfeldCipher(
    const vector<unsigned char>& data, 
    const string& key, 
    bool encrypt
);

string generateGrosfeldKey(int length);

#ifdef __cplusplus
}
#endif

#endif