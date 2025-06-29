#include "grosfeld/grosfeld_cipher_interface.h"
#include "grosfeld/grosfeld_cipher.h"
#include "utils.h" 
#include <iostream>
#include <random>

using namespace std;

void runGrosfeldCipher() {
    try {
        cout << "=== Шифр Гросфельда ===" << endl;

        int encryptChoice = getChoice(
            "1. Шифровать\n2. Дешифровать\nВыбор: ", {1, 2}
        );
        bool encrypt = (encryptChoice == 1);

        string key;
        int keySourceChoice = getChoice(
            "1. Ввести ключ\n2. Сгенерировать ключ\nВыбор: ", {1, 2}
        );

        if (keySourceChoice == 2) {
            int length = getIntegerInput("Длина ключа (1-99): ", 1, 99);
            key = generateGrosfeldKey(length);
            cout << "Сгенерированный ключ: " << key << endl;
        } else {
            while (true) {
                key = getLine("Введите ключ (только цифры): ", false);
                if (isDigits(key)) break;
                else cout << "Ошибка: Ключ должен содержать только цифры.\n";
            }
        }
        
        int dataSourceChoice = getChoice(
            "1. Ввод текста\n2. Работа с файлом\nВыбор: ", {1, 2}
        );

        if (dataSourceChoice == 2) {
            string inputFile = getLine("Входной файл: ", false);
            string outputFile = getLine("Выходной файл: ", false);

            auto inputData = readFile(inputFile);
            auto outputData = grosfeldCipher(inputData, key, encrypt);
            writeFile(outputFile, outputData);

            cout << "Операция завершена успешно!" << endl;
        } else {
            string text = getLine("Введите текст: ", false);

            vector<unsigned char> inputData(text.begin(), text.end());
            auto outputData = grosfeldCipher(inputData, key, encrypt);

            cout << "\nРезультат: ";
            cout.write(reinterpret_cast<const char*>(outputData.data()), outputData.size());
            cout << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка в runGrosfeldCipher: " << e.what() << endl;
    }
}