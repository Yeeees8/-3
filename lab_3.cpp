#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// --- Вспомогательные функции проверки символов ---
bool isLower(char c) {
    return c >= 'a' && c <= 'z';
}

bool isUpper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool isLetter(char c) {
    return isLower(c) || isUpper(c);
}

bool isDigitChar(char c) {
    return c >= '0' && c <= '9';
}

char toLowerChar(char c) {
    if (isUpper(c)) return char(c - 'A' + 'a');
    return c;
}

char toUpperChar(char c) {
    if (isLower(c)) return char(c - 'a' + 'A');
    return c;
}

bool isPunctChar(char c) {
    return c == '.' || c == ',' || c == '!' || c == '?' ||
           c == ';' || c == ':' || c == '-' || c == '"' || c == '\'';
}

// --- Разбиение текста на слова вручную ---
vector<string> splitWords(const string& text) {
    vector<string> words;
    string cur = "";
    for (int i = 0; i < (int)text.length(); i++) {
        char c = text[i];
        if (isLetter(c) || isDigitChar(c)) {
            cur += c;
        } else {
            if (cur.length() > 0) {
                words.push_back(cur);
                cur = "";
            }
        }
    }
    if (cur.length() > 0) {
        words.push_back(cur);
    }
    return words;
}

// --- Пункт 1: Ввод последовательности ---
string inputFromKeyboard() {
    cout << "Введите строку (в конце поставьте точку):\n";
    string s;
    getline(cin, s);
    return s;
}

string inputFromFile() {
    cout << "Введите имя файла (например, input.txt): ";
    string fname;
    getline(cin, fname);
    ifstream fin(fname);
    if (!fin.is_open()) {
        cout << "Ошибка открытия файла!\n";
        return "";
    }
    string line, res = "";
    while (getline(fin, line)) {
        if (!res.empty()) res += " ";
        res += line;
    }
    fin.close();
    return res;
}

// --- Пункт 2: Редактирование входного текста ---
string cleanSpaces(const string& s) {
    string res = "";
    int n = (int)s.length();
    int i = 0;
    while (i < n && s[i] == ' ') i++;

    for (; i < n; i++) {
        if (s[i] == ' ') {
            if (!res.empty() && res.back() != ' ') {
                res += ' ';
            }
        } else if (s[i] == '.') {
            if (!res.empty() && res.back() == ' ') {
                res.pop_back();
            }
            res += '.';
        } else {
            res += s[i];
        }
    }
    if (!res.empty() && res.back() == ' ') res.pop_back();
    return res;
}

string cleanPunctuation(const string& s) {
    string res = "";
    int n = (int)s.length();
    int i = 0;
    while (i < n) {
        if (s[i] == '.') {
            int cnt = 0;
            while (i < n && s[i] == '.') {
                cnt++;
                i++;
            }
            if (cnt >= 3) res += "...";
            else res += '.';
        } else if (isPunctChar(s[i])) {
            char p = s[i];
            res += p;
            while (i < n && s[i] == p) i++;
        } else {
            res += s[i];
            i++;
        }
    }
    return res;
}

string fixRegister(const string& s) {
    string res = s;
    bool newWord = true;
    for (int i = 0; i < (int)res.length(); i++) {
        char c = res[i];
        if (isLetter(c) || isDigitChar(c)) {
            if (newWord) {
                if (isLetter(c)) res[i] = toUpperChar(c);
                newWord = false;
            } else {
                if (isLetter(c)) res[i] = toLowerChar(c);
            }
        } else {
            newWord = true;
        }
    }
    return res;
}

string editText(const string& s) {
    string t = cleanSpaces(s);
    t = cleanPunctuation(t);
    t = fixRegister(t);
    return t;
}

// --- Пункт 3: Задания на выборку слов ---
void task3_1(const vector<string>& words) {
    cout << "3.1 В обратном порядке: ";
    for (int i = (int)words.size() - 1; i >= 0; i--) {
        cout << words[i] << " ";
    }
    cout << "\n";
}

string wordToLower(const string& w) {
    string res = w;
    for (int i = 0; i < (int)res.length(); i++) res[i] = toLowerChar(res[i]);
    return res;
}

void task3_2(vector<string> words) {
    int n = (int)words.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (wordToLower(words[j]) > wordToLower(words[j + 1])) {
                string tmp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = tmp;
            }
        }
    }
    cout << "3.2 По алфавиту: ";
    for (int i = 0; i < n; i++) cout << words[i] << " ";
    cout << "\n";
}

bool hasDigits(const string& w) {
    for (int i = 0; i < (int)w.length(); i++) {
        if (isDigitChar(w[i])) return true;
    }
    return false;
}

void task3_3(const vector<string>& words) {
    cout << "3.3 Без цифр: ";
    for (int i = 0; i < (int)words.size(); i++) {
        if (!hasDigits(words[i])) cout << words[i] << " ";
    }
    cout << "\n";
}

bool hasDuplicateLetters(const string& w) {
    string low = wordToLower(w);
    for (int i = 0; i < (int)low.length(); i++) {
        if (!isLetter(low[i])) continue;
        for (int j = i + 1; j < (int)low.length(); j++) {
            if (low[i] == low[j]) return true;
        }
    }
    return false;
}

void task3_4(const vector<string>& words) {
    cout << "3.4 С повторяющимися буквами: ";
    for (int i = 0; i < (int)words.size(); i++) {
        if (hasDuplicateLetters(words[i])) cout << words[i] << " ";
    }
    cout << "\n";
}

bool firstLetterRepeats(const string& w) {
    if (w.empty() || !isLetter(w[0])) return false;
    char first = toLowerChar(w[0]);
    for (int i = 1; i < (int)w.length(); i++) {
        if (toLowerChar(w[i]) == first) return true;
    }
    return false;
}

void task3_5(const vector<string>& words) {
    cout << "3.5 Первая буква встречается еще раз: ";
    for (int i = 0; i < (int)words.size(); i++) {
        if (firstLetterRepeats(words[i])) cout << words[i] << " ";
    }
    cout << "\n";
}

void task3_6(const vector<string>& words) {
    vector<string> onlyLetters, onlyDigits, mixed;
    for (int i = 0; i < (int)words.size(); i++) {
        bool hasL = false, hasD = false;
        for (int j = 0; j < (int)words[i].length(); j++) {
            if (isLetter(words[i][j])) hasL = true;
            if (isDigitChar(words[i][j])) hasD = true;
        }
        if (hasL && !hasD) onlyLetters.push_back(words[i]);
        else if (!hasL && hasD) onlyDigits.push_back(words[i]);
        else if (hasL && hasD) mixed.push_back(words[i]);
    }
    cout << "3.6 Группировка:\n  Только буквы: ";
    for (const string& w : onlyLetters) cout << w << " ";
    cout << "\n  Только цифры: ";
    for (const string& w : onlyDigits) cout << w << " ";
    cout << "\n  Буквы и цифры: ";
    for (const string& w : mixed) cout << w << " ";
    cout << "\n";
}

// --- Пункт 4: Задания на преобразование слов ---
void task4_1(vector<string> words) {
    cout << "4.1 Первая прописная: ";
    for (int i = 0; i < (int)words.size(); i++) {
        if (!words[i].empty() && isLetter(words[i][0])) {
            words[i][0] = toUpperChar(words[i][0]);
        }
        cout << words[i] << " ";
    }
    cout << "\n";
}

void task4_2(const vector<string>& words) {
    cout << "4.2 Длина каждого слова: ";
    for (int i = 0; i < (int)words.size(); i++) {
        cout << words[i] << "(" << words[i].length() << ") ";
    }
    cout << "\n";
}

void task4_3(vector<string> words) {
    cout << "Введите символы для удаления (без пробелов): ";
    string bad;
    getline(cin, bad);
    cout << "4.3 После удаления: ";
    for (int i = 0; i < (int)words.size(); i++) {
        string nw = "";
        for (int j = 0; j < (int)words[i].length(); j++) {
            char c = words[i][j];
            bool del = false;
            for (int k = 0; k < (int)bad.length(); k++) {
                if (c == bad[k]) { del = true; break; }
            }
            if (!del) nw += c;
        }
        if (!nw.empty()) cout << nw << " ";
    }
    cout << "\n";
}

void task4_4(vector<string> words) {
    cout << "4.4 Замена цифр (1->a .. 9->i): ";
    for (int i = 0; i < (int)words.size(); i++) {
        for (int j = 0; j < (int)words[i].length(); j++) {
            char c = words[i][j];
            if (c >= '1' && c <= '9') {
                words[i][j] = char('a' + (c - '1'));
            }
        }
        cout << words[i] << " ";
    }
    cout << "\n";
}

void task4_5(vector<string> words) {
    cout << "4.5 Цифры в конец слова: ";
    for (int i = 0; i < (int)words.size(); i++) {
        string letters = "", digits = "";
        for (int j = 0; j < (int)words[i].length(); j++) {
            if (isDigitChar(words[i][j])) digits += words[i][j];
            else letters += words[i][j];
        }
        cout << (letters + digits) << " ";
    }
    cout << "\n";
}

void task4_6(const vector<string>& words) {
    cout << "4.6 Вертикальный вывод:\n";
    for (int i = 0; i < (int)words.size(); i++) {
        cout << words[i] << "\n";
    }
}

void task4_7(const vector<string>& words) {
    cout << "4.7 Вывод в 2 колонки с выравниванием по правому краю:\n";
    int maxLen = 15;
    for (int i = 0; i < (int)words.size(); i++) {
        if ((int)words[i].length() + 2 > maxLen) maxLen = (int)words[i].length() + 2;
    }
    for (int i = 0; i < (int)words.size(); i++) {
        int spaces = maxLen - (int)words[i].length();
        for (int s = 0; s < spaces; s++) cout << ' ';
        cout << words[i];
        if (i % 2 == 1 || i == (int)words.size() - 1) cout << "\n";
    }
}

// --- Пункт 5: Поиск подстроки (Наивный, КМП, Бойер-Мур) ---
vector<int> naiveSearch(const string& text, const string& pat) {
    vector<int> pos;
    int n = (int)text.length();
    int m = (int)pat.length();
    if (m == 0 || m > n) return pos;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pat[j]) j++;
        if (j == m) pos.push_back(i);
    }
    return pos;
}

vector<int> buildLPS(const string& pat) {
    int m = (int)pat.length();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) len = lps[len - 1];
            else { lps[i] = 0; i++; }
        }
    }
    return lps;
}

vector<int> kmpSearch(const string& text, const string& pat) {
    vector<int> pos;
    int n = (int)text.length();
    int m = (int)pat.length();
    if (m == 0 || m > n) return pos;

    vector<int> lps = buildLPS(pat);
    int i = 0, j = 0;
    while (i < n) {
        if (pat[j] == text[i]) {
            i++; j++;
        }
        if (j == m) {
            pos.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pat[j] != text[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return pos;
}

vector<int> boyerMooreSearch(const string& text, const string& pat) {
    vector<int> pos;
    int n = (int)text.length();
    int m = (int)pat.length();
    if (m == 0 || m > n) return pos;

    int badChar[256];
    for (int i = 0; i < 256; i++) badChar[i] = -1;
    for (int i = 0; i < m; i++) badChar[(unsigned char)pat[i]] = i;

    int s = 0;
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pat[j] == text[s + j]) j--;
        if (j < 0) {
            pos.push_back(s);
            s += (s + m < n) ? m - badChar[(unsigned char)text[s + m]] : 1;
        } else {
            int shift = j - badChar[(unsigned char)text[s + j]];
            s += (shift > 1) ? shift : 1;
        }
    }
    return pos;
}

void runSearchComparison(const string& text) {
    cout << "Введите подстроку для поиска: ";
    string pat;
    getline(cin, pat);
    if (pat.empty()) {
        cout << "Пустая подстрока!\n";
        return;
    }

    auto t1 = steady_clock::now();
    vector<int> r1 = naiveSearch(text, pat);
    auto t2 = steady_clock::now();

    auto t3 = steady_clock::now();
    vector<int> r2 = kmpSearch(text, pat);
    auto t4 = steady_clock::now();

    auto t5 = steady_clock::now();
    vector<int> r3 = boyerMooreSearch(text, pat);
    auto t6 = steady_clock::now();

    cout << "Найденные позиции: ";
    if (r1.empty()) cout << "не найдено";
    else for (int idx : r1) cout << idx << " ";
    cout << "\n";

    cout << "Наивный поиск: " << duration_cast<nanoseconds>(t2 - t1).count() / 1000.0 << " мкс\n";
    cout << "Алгоритм КМП:  " << duration_cast<nanoseconds>(t4 - t3).count() / 1000.0 << " мкс\n";
    cout << "Бойер-Мур:     " << duration_cast<nanoseconds>(t6 - t5).count() / 1000.0 << " мкс\n";
}

// --- Индивидуальное задание №7: Проверка палиндрома с игнорированием знаков ---
void taskIndividual7(const string& text) {
    string clean = "";
    for (int i = 0; i < (int)text.length(); i++) {
        if (isLetter(text[i]) || isDigitChar(text[i])) {
            clean += toLowerChar(text[i]);
        }
    }
    bool isPal = true;
    int l = 0, r = (int)clean.length() - 1;
    while (l < r) {
        if (clean[l] != clean[r]) {
            isPal = false;
            break;
        }
        l++; r--;
    }
    cout << "Очищенная строка: " << clean << "\n";
    if (isPal) cout << "Результат: строка ЯВЛЯЕТСЯ палиндромом (true)\n";
    else cout << "Результат: строка НЕ является палиндромом (false)\n";
}

int main() {
    string text = "";
    bool hasText = false;

    while (true) {
        cout << "\n=== МЕНЮ ===\n"
             << "1. Ввести последовательность (1 - клавиатура, 2 - файл)\n"
             << "2. Отредактировать текст (пробелы, знаки, регистр)\n"
             << "3. Задания на выборку слов (все 6 пунктов)\n"
             << "4. Задания на преобразование слов (все 7 пунктов)\n"
             << "5. Поиск подстроки (Наивный, КМП, Бойер-Мур)\n"
             << "6. Индивидуальное задание №7 (Проверка палиндрома)\n"
             << "0. Выход\nВыбор: ";

        int choice;
        if (!(cin >> choice)) break;
        cin.ignore();

        if (choice == 0) break;

        if (choice != 1 && !hasText) {
            cout << "Сначала введите текст (пункт 1)!\n";
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Способ ввода (1 - клавиатура, 2 - файл): ";
                int mode;
                cin >> mode;
                cin.ignore();
                if (mode == 2) text = inputFromFile();
                else text = inputFromKeyboard();
                if (!text.empty()) {
                    hasText = true;
                    cout << "Исходный текст: " << text << "\n";
                }
                break;
            }
            case 2: {
                text = editText(text);
                cout << "Отредактированный текст:\n" << text << "\n";
                break;
            }
            case 3: {
                vector<string> words = splitWords(text);
                task3_1(words);
                task3_2(words);
                task3_3(words);
                task3_4(words);
                task3_5(words);
                task3_6(words);
                break;
            }
            case 4: {
                vector<string> words = splitWords(text);
                task4_1(words);
                task4_2(words);
                task4_3(words);
                task4_4(words);
                task4_5(words);
                task4_6(words);
                task4_7(words);
                break;
            }
            case 5: {
                runSearchComparison(text);
                break;
            }
            case 6: {
                taskIndividual7(text);
                break;
            }
            default:
                cout << "Неверный пункт меню.\n";
        }
    }
    return 0;
}
