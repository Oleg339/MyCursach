#pragma once
using namespace std;
class Lists {
public:
    virtual void setInformation() {

    }
    virtual void getInformation() {
    }
};
namespace input {
    static string InputPassword() {
        string s{};
        char c{};
        int len = 0;
        while (true) {
            c = _getch();
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
                len++;
                s += c;
                _putch('*');
            }
            if (c == '\b') {
                if (len > 0) {
                    len--;
                    cout << "\b \b";
                    s.erase(s.length() - 1);
                }
            }
            if (c == '\r') {
                break;
            }
        }
        return s;
    }
    int GetInt() {
        int i;
        while (!(cin >> i)) {
            cin.clear();
            while (cin.get() != '\n' && i >= 0) {}
            cout << "Введите другой номер:" << endl;
        }
        return i;
    }
}
class Task : public Lists {
    char taskName[80] = "";
    char owner[80] = "";
    char taskDescription[1000] = "";
    bool isFinished = false;
public:
    string getName() {
        return taskName;
    }
    void setDone()
    {
        string str;
        cout << "Завершено? (Y/N): ";
        cin >> str;
        this->isFinished = str == "Y";
    }
    void setInformation(string name) {
        string str;
        strcpy(this->taskName, name.c_str());

        cout << "Введите описание задачи: ";
        getline(cin, str);
        strcpy(this->taskDescription, str.c_str());
        cout << "Введите имя пользователя, которому принадлежит задача: ";
        getline(cin, str);
        strcpy(this->owner, str.c_str());
        cout << "Завершено? (Y/N): ";
        cin >> str;
        this->isFinished = str == "Y";
    }
    void getInformation() {
        cout << "Задача: " << this->taskName << "\nОписание: " << this->taskDescription
            << "\nСтатус: " << (this->isFinished ? "Завершено" : "Не завершено") << "\nПринадлежит: " << owner << "\n" << endl;
    }
    bool returnIsFinished() {
        return isFinished;
    }
    string getOwner() {
        return owner;
    }
    friend bool operator<(Task a, Task b) {
        return a.returnIsFinished() < b.returnIsFinished();
    }
    friend bool operator>(Task a, Task b) {
        return a.returnIsFinished() > b.returnIsFinished();
    }
};
