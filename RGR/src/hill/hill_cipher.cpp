#include "hill/hill_cipher.h"
#include "hill/matrix_utils.h"
#include "utils.h"
#include <stdexcept>
#include <cctype>

using namespace std;

vector<unsigned char> hillCipher(
    const vector<unsigned char>& data, 
    const vector<vector<int>>& key, 
    bool encrypt
) {
    if (data.empty()) return {};
    
    if (data.size() % 2 != 0) {
        throw runtime_error("Размер данных должен быть четным");
    }
    
    vector<vector<int>> currentKey = key;
    if (!encrypt) {
        vector<vector<int>> invKey;
        if (!inverseMatrix(key, invKey)) {
            throw runtime_error("Невозможно найти обратную матрицу для ключа");
        }
        currentKey = invKey;
    }
    
    vector<unsigned char> result;
    result.reserve(data.size());
    
    for (size_t i = 0; i < data.size(); i += 2) {
        int block[2] = {data[i], data[i+1]};
        int encrypted[2] = {0, 0};
        
        for (int row = 0; row < 2; row++) {
            for (int col = 0; col < 2; col++) {
                encrypted[row] += currentKey[row][col] * block[col];
            }
            encrypted[row] = (encrypted[row] % MOD + MOD) % MOD;
        }
        
        result.push_back(static_cast<unsigned char>(encrypted[0]));
        result.push_back(static_cast<unsigned char>(encrypted[1]));
    }
    
    return result;
}

bool validateKey(const vector<vector<int>>& key) {
    if (key.size() != 2 || key[0].size() != 2 || key[1].size() != 2) {
        return false;
    }
    
    int det = determinant2x2(key);
    if (det == 0) return false;
    
    try {
        modInverse(det, MOD);
        return true;
    } catch (...) {
        return false;
    }
}

vector<vector<int>> generateHillKey() {
    vector<vector<int>> key(2, vector<int>(2));
    while (true) {
        for (auto& row : key) {
            for (int& val : row) {
                val = randomInt(0, 255);
            }
        }
        if (validateKey(key)) break;
    }
    return key;
}