#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include "common.h"
#include <vector>
using namespace std;
using Matrix = vector<vector<int>>;

int determinant2x2(const Matrix& key);
bool inverseMatrix(const Matrix& key, Matrix& invKey);

#endif