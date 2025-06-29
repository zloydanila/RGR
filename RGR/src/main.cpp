#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <dlfcn.h>
#include <tuple>
#include <limits>
#include "grosfeld/grosfeld_cipher_interface.h"
#include "hill/hill_cipher_interface.h"
#include "polybius/polybius_cipher_interface.h"
#include "utils.h"

using namespace std;

enum class CipherType { Grosfeld, Hill, Polybius };
enum class ActionType { Encrypt = 1, Decrypt = 2 };

void writeLog(CipherType cipher, ActionType action, const string& key, const string& source) {
    ofstream logFile("log.txt", ios::app);
    if (!logFile) {
        throw runtime_error("Не удалось открыть файл лога.");
    }

    string cipherName;
    switch (cipher) {
        case CipherType::Grosfeld: cipherName = "Grosfeld"; break;
        case CipherType::Hill: cipherName = "Hill"; break;
        case CipherType::Polybius: cipherName = "Polybius"; break;
    }

    string actionName = (action == ActionType::Encrypt) ? "Encrypt" : "Decrypt";

    logFile << "Cipher: " << cipherName << ", Action: " << actionName 
            << ", Key: " << key << ", Source: " << source << "\n";
    logFile.close();
}

bool checkPassword() {
    const string correctPassword = "qwerty";
    int attempts = 3;

    while (attempts > 0) {
        cout << "\nВведите пароль доступа: ";
        string input;
        getline(cin, input);

        if (input == correctPassword) {
            cout << "\nДобро пожаловать!\n" << endl;
            return true;
        }

        attempts--;
        cout << "Неправильный пароль. Попыток осталось: " << attempts << endl;
    }

    cout << "Доступ заблокирован. Несанкционированный пользователь" << endl;
    return false;
}

void* loadLibrary(const string& libName) {
    vector<string> paths = {
        "./lib/" + libName,
        "../lib/" + libName,
        "lib/" + libName,
        "/usr/local/lib/" + libName,
        libName
    };

    for (const auto& path : paths) {
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (handle) {
            cout << "Загружена библиотека: " << path << endl;
            return handle;
        }
    }
    
    cerr << "Не удалось загрузить библиотеку " << libName << ": " << dlerror() << endl;
    return nullptr;
}

int main() {
    initLocale();
    
    if (!checkPassword()) {
        return 1;
    }

    while (true) {

        void* grosfeldLib = loadLibrary("libgrosfeld.so");
        bool grosfeldAvailable = (grosfeldLib != nullptr);
        
        void* hillLib = loadLibrary("libhill.so");
        bool hillAvailable = (hillLib != nullptr);
        
        void* polybiusLib = loadLibrary("libpolybius.so");
        bool polybiusAvailable = (polybiusLib != nullptr);

        if (!grosfeldAvailable && !hillAvailable && !polybiusAvailable) {
            cerr << "\nОшибка: ни одна из библиотек шифрования не найдена.\n";
            cerr << "Убедитесь, что библиотеки находятся в каталоге ./lib/\n";
            cerr << "Нажмите Enter для выхода...";
            cin.get();
            return 1;
        }

        using GrosfeldFunc = vector<unsigned char>(*)(const vector<unsigned char>&, const string&, bool);
        GrosfeldFunc grosfeldCipherFunc = nullptr;
        if (grosfeldLib) {
            grosfeldCipherFunc = reinterpret_cast<GrosfeldFunc>(dlsym(grosfeldLib, "grosfeldCipher"));
            if (!grosfeldCipherFunc) {
                cerr << "Ошибка загрузки функции Grosfeld: " << dlerror() << endl;
                grosfeldAvailable = false;
            }
        }

        using HillFunc = vector<unsigned char>(*)(const vector<unsigned char>&, const vector<vector<int>>&, bool);
        HillFunc hillCipherFunc = nullptr;
        if (hillLib) {
            hillCipherFunc = reinterpret_cast<HillFunc>(dlsym(hillLib, "hillCipher"));
            if (!hillCipherFunc) {
                cerr << "Ошибка загрузки функции Hill: " << dlerror() << endl;
                hillAvailable = false;
            }
        }

        using PolybiusFunc = vector<unsigned char>(*)(const vector<unsigned char>&, bool);
        PolybiusFunc polybiusCipherFunc = nullptr;
        if (polybiusLib) {
            polybiusCipherFunc = reinterpret_cast<PolybiusFunc>(dlsym(polybiusLib, "polybiusCipher"));
            if (!polybiusCipherFunc) {
                cerr << "Ошибка загрузки функции Polybius: " << dlerror() << endl;
                polybiusAvailable = false;
            }
        }
        if (!grosfeldAvailable && !hillAvailable && !polybiusAvailable) {
            cerr << "\nОшибка: ни одна из библиотек не содержит необходимых функций.\n";
            cerr << "Нажмите Enter для выхода...";
            cin.get();
            return 1;
        }

        int cipherChoice;
        CipherType cipher;
        bool validCipher = false;

        while(!validCipher) {
            cout << "\nВыберите шифр:\n";
            if (grosfeldAvailable) cout << "1 — Шифр Гросфельда\n";
            if (hillAvailable) cout << "2 — Шифр Хилла\n";
            if (polybiusAvailable) cout << "3 — Квадрат Полибия\n";
            cout << "4 — Выход\n";
            cout << "Ваш выбор: ";

            if (!(cin >> cipherChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Ошибка ввода. Пожалуйста, введите число.\n";
                continue;
            }

            if (cipherChoice == 4) {

                if (grosfeldLib) dlclose(grosfeldLib);
                if (hillLib) dlclose(hillLib);
                if (polybiusLib) dlclose(polybiusLib);
                return 0;
            }

            switch (cipherChoice) {
                case 1: 
                    validCipher = grosfeldAvailable; 
                    cipher = CipherType::Grosfeld; 
                    break;
                case 2: 
                    validCipher = hillAvailable; 
                    cipher = CipherType::Hill; 
                    break;
                case 3: 
                    validCipher = polybiusAvailable; 
                    cipher = CipherType::Polybius; 
                    break;
                default: 
                    validCipher = false; 
                    break;
            }
            
            if (!validCipher) {
                cerr << "Выбранный модуль недоступен. Пожалуйста, выберите другой.\n";
            }
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string input;
        string sourceChoice;
        cout << "\nВыберите источник текста:\n1 — Файл\n2 — Ввести вручную\nВаш выбор: ";
        if (!(cin >> sourceChoice) || (sourceChoice != "1" && sourceChoice != "2")) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "Ошибка: введите 1 или 2.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (sourceChoice == "1") {
            string filename;
            cout << "Введите имя файла: ";
            getline(cin, filename);
            
            ifstream inFile(filename, ios::binary);
            if (!inFile) {
                cerr << "Ошибка: файл '" << filename << "' не найден.\n";
                continue;
            }
            
            ostringstream content;
            content << inFile.rdbuf();
            input = content.str();
            inFile.close();
            cout << "Файл успешно прочитан (" << input.size() << " байт).\n";
        } else {
            cout << "Введите текст: ";
            getline(cin, input);
        }

        int actionChoice;
        cout << "\nВыберите действие:\n1 — Зашифровать\n2 — Расшифровать\nВаш выбор: ";
        if (!(cin >> actionChoice) || (actionChoice != 1 && actionChoice != 2)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "Ошибка: введите 1 (шифрование) или 2 (дешифрование).\n";
            continue;
        }
        ActionType action = static_cast<ActionType>(actionChoice);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<unsigned char> outputData;
        bool operationSuccess = true;
        string key;
        vector<vector<int>> hillKey;
        
        try {
            switch(cipher) {
                case CipherType::Grosfeld:

                    cout << "Введите ключ: ";
                    getline(cin, key);
                    
                    if (grosfeldCipherFunc) {
                        vector<unsigned char> inputData(input.begin(), input.end());
                        outputData = grosfeldCipherFunc(inputData, key, action == ActionType::Encrypt);
                    } else {
                        throw runtime_error("Функция шифрования Grosfeld не загружена");
                    }
                    break;
                    
                case CipherType::Hill:
                    hillKey = vector<vector<int>>(2, vector<int>(2));
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                            cout << "Введите элемент матрицы [" << i << "][" << j << "]: ";
                            cin >> hillKey[i][j];
                            cin.ignore();
                        }
                    }
                    
                    if (hillCipherFunc) {
                        vector<unsigned char> inputData(input.begin(), input.end());
                        outputData = hillCipherFunc(inputData, hillKey, action == ActionType::Encrypt);
                    } else {
                        throw runtime_error("Функция шифрования Hill не загружена");
                    }
                    break;
                    
                case CipherType::Polybius:
                    if (polybiusCipherFunc) {
                        vector<unsigned char> inputData(input.begin(), input.end());
                        outputData = polybiusCipherFunc(inputData, action == ActionType::Encrypt);
                    } else {
                        throw runtime_error("Функция шифрования Polybius не загружена");
                    }
                    break;
            }
        } catch (const exception& e) {
            cerr << "Ошибка при обработке: " << e.what() << endl;
            operationSuccess = false;
        }

        if (!operationSuccess) {
            continue;
        }

        string output(outputData.begin(), outputData.end());

        string sourceName = (sourceChoice == "1") ? "file" : "manual";
        writeLog(cipher, action, key, sourceName);

        string filePath;
        cout << "Введите путь для сохранения результата: ";
        getline(cin, filePath);
        
        ofstream outFile(filePath, ios::binary);
        if (!outFile) {
            cerr << "Ошибка: не удалось создать файл '" << filePath << "'\n";
            continue;
        }
        
        outFile << output;
        outFile.close();
        cout << "Результат успешно сохранен в " << filePath << " (" << output.size() << " байт)\n";

        char show;
        cout << "Показать результат? (y/n): ";
        cin >> show;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (show == 'y' || show == 'Y') {
            if (output.size() > 1000) {
                cout << "\nПервые 1000 символов результата:\n";
                cout << output.substr(0, 1000) << "\n... [полный результат сохранен в файл]\n";
            } else {
                cout << "\nРезультат:\n" << output << endl;
            }
        }
        if (grosfeldLib) dlclose(grosfeldLib);
        if (hillLib) dlclose(hillLib);
        if (polybiusLib) dlclose(polybiusLib);

        char choice;
        cout << "\nПродолжить работу? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    return 0;
}