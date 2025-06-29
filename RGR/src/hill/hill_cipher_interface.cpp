#include "hill/hill_cipher_interface.h"
#include "hill/hill_cipher.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

void runHillCipherInterface() {
    try {
        cout << "=== Шифр Хилла ===" << endl;
        
        vector<vector<int>> key;
        if (getChoice("1. Ввести ключ\n2. Сгенерировать ключ\nВыбор: ", {1, 2}) == 2) {
            key = generateHillKey();
            cout << "Сгенерированный ключ:\n";
            for (const auto& row : key) {
                for (int val : row) cout << val << " ";
                cout << endl;
            }
        } else {
            key = vector<vector<int>>(2, vector<int>(2));
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    int value = getIntegerInput(
                        "Элемент [" + to_string(i+1) + "][" + to_string(j+1) + "]: ",
                        0, 255
                    );
                    key[i][j] = value;
                }
            }
        }
        
        if (!validateKey(key)) {
            throw runtime_error("Недопустимый ключ (детерминант должен быть нечетным и ненулевым)");
        }
        
        bool encrypt = getChoice("1. Шифровать\n2. Дешифровать\nВыбор: ", {1, 2}) == 1;
        
        if (getChoice("1. Работа с текстом\n2. Работа с файлом\nВыбор: ", {1, 2}) == 2) {
            string inputFile = getLine("Входной файл: ", false);
            string outputFile = getLine("Выходной файл: ", false);
            
            auto inputData = readFile(inputFile);
            if (inputData.size() % 2 != 0) {
                throw runtime_error("Размер файла должен быть четным (добавьте 1 байт)");
            }
            auto outputData = hillCipher(inputData, key, encrypt);
            writeFile(outputFile, outputData);
            
            cout << "Операция завершена успешно!" << endl;
        } else {
            string text = getLine("Введите текст: ", false);
            
            if (encrypt && text.size() % 2 != 0) {
                text += ' ';
                cout << "Добавлен пробел для выравнивания размера\n";
            }

            else if (!encrypt && text.size() % 2 != 0) {
                throw runtime_error("Текст для дешифрования должен иметь четную длину");
            }
            
            vector<unsigned char> inputData(text.begin(), text.end());
            auto outputData = hillCipher(inputData, key, encrypt);
            
            if (!encrypt && !outputData.empty() && outputData.back() == ' ') {
                outputData.pop_back();
            }
            
            cout << "\nРезультат: ";
            cout.write(reinterpret_cast<const char*>(outputData.data()), outputData.size());
            cout << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}