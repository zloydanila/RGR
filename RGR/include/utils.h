#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <initializer_list>
#include <locale>
#include <codecvt>

using namespace std;

void clearInputBuffer();
int getChoice(const string& prompt, const initializer_list<int>& options);
int getIntegerInput(const string& prompt, int min, int max);
string getLine(const string& prompt, bool maskInput = false);
bool isDigits(const string& str);
vector<unsigned char> readFile(const string& filename);
void writeFile(const string& filename, const vector<unsigned char>& data);
int randomInt(int min, int max);
void safePrint(const vector<unsigned char>& data);
void initLocale();

#endif 