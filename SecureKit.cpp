
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cctype>
#include <limits>
using namespace std;


class PasswordAnalyzer {
private:
    vector<string> commonPasswords;

public:
    PasswordAnalyzer() {
        commonPasswords = {"123456", "password", "qwerty", "12345678", "111111",
                           "abc123", "admin", "iloveyou", "letmein", "welcome"};
    }

    bool isCommon(const string &pass) const {
        string lower = pass;
        transform(lower.begin(), lower.end(), lower.begin(),
                  [](unsigned char c) { return tolower(c); });
        return find(commonPasswords.begin(), commonPasswords.end(), lower) != commonPasswords.end();
    }

    int score(const string &pass) const {
        if (isCommon(pass)) return 0;
        int s = 0;
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;

        if (pass.size() >= 8) s++;
        if (pass.size() >= 12) s++;

        for (unsigned char c : pass) {
            if (isupper(c)) hasUpper = true;
            else if (islower(c)) hasLower = true;
            else if (isdigit(c)) hasDigit = true;
            else hasSymbol = true;
        }
        s += hasUpper + hasLower + hasDigit + hasSymbol;
        return s;
    }

    string rating(int s) const {
        if (s <= 2) return "Weak";
        if (s <= 4) return "Medium";
        if (s == 5) return "Strong";
        return "Very Strong";
    }

    void analyze(const string &pass) const {
        if (isCommon(pass))
            cout << "Warning: this is a very common password!\n";
        int s = score(pass);
        cout << "Score: " << s << "/6  ->  " << rating(s) << "\n";
        if (pass.size() < 8) cout << "Tip: use at least 8 characters.\n";
    }
};


class PasswordGenerator {
private:
    mt19937 engine;

    char pick(const string &set) {
        uniform_int_distribution<size_t> dist(0, set.size() - 1);
        return set[dist(engine)];
    }

public:
    PasswordGenerator() : engine(random_device{}()) {}

    string generate(int length) {
        const string lower = "abcdefghijklmnopqrstuvwxyz";
        const string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const string digits = "0123456789";
        const string symbols = "!@#$%^&*?-_";
        const string all = lower + upper + digits + symbols;

        if (length < 4) length = 4;

       
        string result;
        result += pick(lower);
        result += pick(upper);
        result += pick(digits);
        result += pick(symbols);
        while ((int)result.size() < length) result += pick(all);

        shuffle(result.begin(), result.end(), engine);
        return result;
    }
};
class CaesarCipher {
public:
    static string shiftText(const string &text, int key) {
        key = ((key % 26) + 26) % 26;
        string out = text;
        for (char &c : out) {
            if (isupper((unsigned char)c)) c = 'A' + (c - 'A' + key) % 26;
            else if (islower((unsigned char)c)) c = 'a' + (c - 'a' + key) % 26;
        }
        return out;
    }
    static string encrypt(const string &text, int key) { return shiftText(text, key); }
    static string decrypt(const string &text, int key) { return shiftText(text, -key); }
};

int readInt(const string &prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number, try again: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

string readLine(const string &prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

void showMenu() {
    cout << "\n===== SecureKit =====\n"
         << "1. Check password strength\n"
         << "2. Generate a strong password\n"
         << "3. Encrypt text (Caesar)\n"
         << "4. Decrypt text (Caesar)\n"
         << "0. Exit\n";
}

int main() {
    PasswordAnalyzer analyzer;
    PasswordGenerator generator;
    int choice;

    do {
        showMenu();
        choice = readInt("Choose: ");

        switch (choice) {
        case 1:
            analyzer.analyze(readLine("Enter password: "));
            break;
        case 2: {
            int len = readInt("Length (min 4): ");
            cout << "Generated: " << generator.generate(len) << "\n";
            break;
        }
        case 3: {
            string text = readLine("Text: ");
            int key = readInt("Key: ");
            cout << "Encrypted: " << CaesarCipher::encrypt(text, key) << "\n";
            break;
        }
        case 4: {
            string text = readLine("Text: ");
            int key = readInt("Key: ");
            cout << "Decrypted: " << CaesarCipher::decrypt(text, key) << "\n";
            break;
        }
        case 0:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
