#include "polybius/polybius_cipher.h"
#include <stdexcept>
#include <cctype>
#include <cwctype>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>

using namespace std;
void charToCoords(wchar_t c, int& row, int& col) {

    bool isUpper = iswupper(c);
    c = towlower(c);
    

    if (c >= L'a' && c <= L'z') {
        row = 0;
        col = c - L'a';
        if (isUpper) row |= 0x80; 
        return;
    }
    

    if (c >= L'а' && c <= L'я') {
        row = 1;
        col = c - L'а';

        if (c == L'ё') {
            col = 32;  
        }
        if (isUpper) row |= 0x80;  
        return;
    }
    

    if (c == L'ё') {
        row = 1;
        col = 32;
        if (isUpper) row |= 0x80;
        return;
    }
    

    row = 2;
    col = static_cast<int>(c);
}


wchar_t coordsToChar(int row, int col) {

    bool isUpper = (row & 0x80) != 0;
    row &= 0x7F;  
    
    wchar_t c = 0;
    

    if (row == 0) {
        c = L'a' + col;
        if (isUpper) c = towupper(c);
        return c;
    }
    

    if (row == 1) {
        if (col == 32) {
            c = L'ё';  
        } else {
            c = L'а' + col;
        }
        if (isUpper) c = towupper(c);
        return c;
    }
    

    return static_cast<wchar_t>(col);
}

vector<unsigned char> polybiusCipher(
    const vector<unsigned char>& input, 
    bool encrypt
) {

    wstring_convert<codecvt_utf8<wchar_t>> conv;
    
    if (encrypt) {

        wstring winput;
        try {
            winput = conv.from_bytes(
                reinterpret_cast<const char*>(input.data()), 
                reinterpret_cast<const char*>(input.data() + input.size())
            );
        } catch (...) {
            throw runtime_error("Ошибка декодирования UTF-8");
        }
        
        wstring woutput;
        for (wchar_t c : winput) {
            int row, col;
            charToCoords(c, row, col);
            

            woutput += static_cast<wchar_t>(row);
            woutput += static_cast<wchar_t>(col);
        }
        
        try {
            string output = conv.to_bytes(woutput);
            return vector<unsigned char>(output.begin(), output.end());
        } catch (...) {
            throw runtime_error("Ошибка кодирования в UTF-8");
        }
    } else {

        string text(input.begin(), input.end());
        wstring winput;
        try {
            winput = conv.from_bytes(text);
        } catch (...) {
            throw runtime_error("Ошибка декодирования UTF-8");
        }
        
        if (winput.size() % 2 != 0) {
            throw runtime_error("Некорректный размер зашифрованных данных");
        }
        
        wstring woutput;
        for (size_t i = 0; i < winput.size(); i += 2) {
            int row = static_cast<int>(winput[i]);
            int col = static_cast<int>(winput[i+1]);
            woutput += coordsToChar(row, col);
        }
        
        try {
            string output = conv.to_bytes(woutput);
            return vector<unsigned char>(output.begin(), output.end());
        } catch (...) {
            throw runtime_error("Ошибка кодирования в UTF-8");
        }
    }
}