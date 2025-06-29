#include "hill/matrix_utils.h"
#include <stdexcept>
#include <tuple>

using namespace std;

int modInverse(int a, int m) {
    a = (a % m + m) % m;
    if (a == 0) {
        throw runtime_error("Обратный элемент не существует");
    }
    
    int t = 0, new_t = 1;
    int r = m, new_r = a;
    
    while (new_r != 0) {
        int quotient = r / new_r;
        int tmp_t = t;
        t = new_t;
        new_t = tmp_t - quotient * new_t;
        
        int tmp_r = r;
        r = new_r;
        new_r = tmp_r - quotient * new_r;
    }
    
    if (r != 1) {
        throw runtime_error("Обратный элемент не существует");
    }
    if (t < 0) t += m;
    return t;
}

int determinant2x2(const vector<vector<int>>& key) {
    if (key.size() != 2 || key[0].size() != 2 || key[1].size() != 2) {
        throw runtime_error("Ключ должен быть 2x2");
    }
    long long a = static_cast<long long>(key[0][0]);
    long long b = static_cast<long long>(key[0][1]);
    long long c = static_cast<long long>(key[1][0]);
    long long d = static_cast<long long>(key[1][1]);
    long long det = a * d - b * c;
    return (det % MOD + MOD) % MOD;
}

bool inverseMatrix(const vector<vector<int>>& key, vector<vector<int>>& invKey) {
    if (key.size() != 2 || key[0].size() != 2 || key[1].size() != 2) {
        return false;
    }
    
    int det = determinant2x2(key);
    if (det == 0) return false;
    
    try {
        int inv_det = modInverse(det, MOD);
        
        invKey = vector<vector<int>>(2, vector<int>(2));
        invKey[0][0] = (key[1][1] * inv_det % MOD + MOD) % MOD;
        invKey[0][1] = ((-key[0][1] * inv_det % MOD) + MOD) % MOD;
        invKey[1][0] = ((-key[1][0] * inv_det % MOD) + MOD) % MOD;
        invKey[1][1] = (key[0][0] * inv_det % MOD + MOD) % MOD;
        
        return true;
    } catch (...) {
        return false;
    }
}