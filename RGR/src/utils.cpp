#include "utils.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <random>
#include <termios.h>
#include <unistd.h>
#include <cctype>
#include <locale>
#include <codecvt>
#include <iomanip>

using namespace std;

void initLocale() {
    setlocale(LC_ALL, "");
    locale::global(locale(""));
    wcout.imbue(locale());
    cout.imbue(locale());
}

bool isDigits(const string& str) {
    for (char c : str) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return !str.empty(); 
}

vector<unsigned char> readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Не удалось открыть файл для чтения: " + filename);
    }

    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw runtime_error("Ошибка чтения файла: " + filename);
    }
    
    return buffer;
}

void writeFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Не удалось открыть файл для записи: " + filename);
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int randomInt(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getChoice(const string& prompt, const initializer_list<int>& options) {
    int choice;
    while (true) {
        cout << prompt;
        if (!(cin >> choice)) {
            cout << "Ошибка: Некорректный ввод. Пожалуйста, введите число.\n";
            clearInputBuffer();
            continue;
        }

        bool valid = false;
        for (int option : options) {
            if (choice == option) {
                valid = true;
                break;
            }
        }

        if (!valid) {
            cout << "Ошибка: Недопустимый выбор. Пожалуйста, выберите из предложенных вариантов.\n";
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();
        return choice;
    }
}

int getIntegerInput(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (!(cin >> value)) {
            cout << "Ошибка: Некорректный ввод. Пожалуйста, введите число.\n";
            clearInputBuffer();
            continue;
        }

        if (value < min || value > max) {
            cout << "Ошибка: Значение должно быть между " << min << " и " << max << "\n";
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();
        return value;
    }
}

string getLine(const string& prompt, bool maskInput) {
    string input;
    cout << prompt;
    cout.flush();

    if (maskInput) {
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
            cerr << "Ошибка: Не удалось настроить терминал. Ввод будет видимым." << endl;
            getline(cin, input);
        } else {
            getline(cin, input);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            cout << endl;
        }
    } else {
        getline(cin, input);
    }
    return input;
}