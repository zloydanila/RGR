#include "polybius/polybius_cipher_interface.h"
#include "polybius/polybius_cipher.h"
#include "utils.h"
#include <iostream>

using namespace std;

void runPolybius() {
    try {
        cout << "=== Шифр Полибия ===" << endl;
        
        bool encrypt = getChoice("1. Шифровать\n2. Дешифровать\nВыбор: ", {1, 2}) == 1;
        
        if (getChoice("1. Работа с текстом\n2. Работа с файлом\nВыбор: ", {1, 2}) == 2) {
            string inputFile = getLine("Входной файл: ", false);
            string outputFile = getLine("Выходной файл: ", false);
            
            auto inputData = readFile(inputFile);
            auto outputData = polybiusCipher(inputData, encrypt);
            writeFile(outputFile, outputData);
            
            cout << "Операция завершена успешно!" << endl;
        } else {
            string text = getLine("Введите текст: ", false);
            
            vector<unsigned char> inputData(text.begin(), text.end());
            auto outputData = polybiusCipher(inputData, encrypt);
            
            cout << "\nРезультат: ";

            if (encrypt) {
                string resultStr(outputData.begin(), outputData.end());
                cout << resultStr;
            } else {
                cout.write(reinterpret_cast<const char*>(outputData.data()), outputData.size());
            }
            cout << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}