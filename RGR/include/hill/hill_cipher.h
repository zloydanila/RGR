#ifndef HILL_CIPHER_H
#define HILL_CIPHER_H

#include <vector>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

vector<unsigned char> hillCipher(
    const vector<unsigned char>& data, 
    const vector<vector<int>>& key, 
    bool encrypt
);

bool validateKey(const vector<vector<int>>& key);
vector<vector<int>> generateHillKey();

#ifdef __cplusplus
}
#endif

#endif