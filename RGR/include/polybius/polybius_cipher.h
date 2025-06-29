#ifndef POLYBIUS_CIPHER_H
#define POLYBIUS_CIPHER_H

#include <vector>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

vector<unsigned char> polybiusCipher(
    const vector<unsigned char>& input, 
    bool encrypt
);

#ifdef __cplusplus
}
#endif

#endif