#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <Vector>
#include <iomanip>
#include <fstream>
#include <xhash>
#include "conio.h"
#include <vector>
#include <algorithm>
#include "Task.h"

using namespace stdext;

using namespace input;



template <class T>
bool WriteToFile(T obj, string adress) {
    fstream file;
    try {
        file.open(adress, ios::app | ios::out | ios::in | ios::binary);
    }
    catch(...) {
        cout << "File open error";
    }
    if (file) {
        file.write(reinterpret_cast<char*>(&obj), sizeof(T));
        file.close();
        return 1;
    }
    return 0;
}

template <class T>
class subarray {
public:
    T entry;
    int number;
    friend bool operator>(subarray a, subarray b) {
        return a.entry > b.entry;
    }
    friend bool operator<(subarray a, subarray b) {
        return a.entry < b.entry;
    }
};


class Error : public Lists {
    static int size;
    char owner[80];
    char errorName[80];
    char errorDescription[1000] = "";
    char errorStatus[20];
    bool isFinished;
public:
    string getOwner(){
        return owner;
    }
    void setDone()
    {
        string str;
        cout << "Завершено? (Y/N): ";
        cin >> str;
        this->isFinished = str == "Y";
    }
    string getName() {
        return errorName;
    }
    void setInformation(string name) {
        getline(cin, name);
        strcpy(this->errorName, name.c_str());
        string str;
        cout << "Введите описание ошибки: ";
        getline(cin, str);
        strcpy(this->errorDescription, str.c_str());

        while (true) {
            cout << "Введите статус ошибки: " << endl;
            cout << "\n\t[1] - Приемлемый\n\t[2] - Средний\n\t[3] - Критический" << endl;
            int status = GetInt();
            if (status > 0 && status <= 3) {
                if (status == 1) {
                    strcpy(this->errorStatus, "Приемлемый");
                    break;
                }
                else if (status == 2) {
                    strcpy(this->errorStatus, "Средний");
                    break;
                }
                else if (status == 3) {
                    strcpy(this->errorStatus, "Критический");
                    break;
                }
            }
            else {
                cout << "\nВведите корректный статус";
                continue;
            }
        }
        cout << "Введите имя пользователя, которому принадлежит ошибка: ";
        cin.ignore(10000, '\n');
        cin.clear();
        getline(cin, str);
        strcpy(this->owner,str.c_str());
        cout << "Завершено? (Y/N): ";
        cin >> str;
        this->isFinished = str == "Y";
    }
    bool returnIsFinished() {
        return isFinished;
    }
    void getInformation() {
        cout << "Ошибка: " << this->errorName << "\nОписание: " << this->errorDescription << "\nКритичность: " << this->errorStatus
            << "\nСтатус: " << (this->isFinished ? "Завершено" : "Не завершено") <<"\nПринадлежит: "<< owner << "\n" << endl;
    }
    friend bool operator<(Error a, Error b) {
        return a.returnIsFinished() < b.returnIsFinished();
    }
    friend bool operator>(Error a, Error b) {
        return a.returnIsFinished() > b.returnIsFinished();
    }
    friend ostream& operator << (ostream os, const Error err) {
        return os << "Ошибка: " << err.errorName << "\nОписание: " << err.errorDescription << "\nКритичность: " << err.errorStatus
            << "\nСтатус: " << (err.isFinished ? "Завершено" : "Не завершено") << "\nПринадлежит: " << err.owner << "\n" << endl;
    }
};

class User {
    char password[80] = "";
    char userName[80] = "";
    bool isAdmin = false;
public:
    string getPassword() {
        return password;
    }
    string getUserName() {
        return userName;
    }
    virtual bool getStatus() {
        return isAdmin;
    }
    void setInformation(string _userName, bool status ,string _password) {
        this->isAdmin = status;
        strcpy(userName, _userName.c_str());
        strcpy(password, to_string(hash_value(_password)).c_str());
    }
};

class menu {
public:
    static void userChoice();
    static void createUser();
    static void adminMenu();
    static void userMenu(User);
    static void showErrors(User);
    static void userMenu();
    static void showErrors();
    static void showTasks(User);
    static void showTasks();
    static void editErr(vector<Error>& vecErr, int i);
};

string UserFile("C:\\Users\\bahar\\Desktop\\userList.bin");
string TasksFile("C:\\Users\\bahar\\Desktop\\tasksList.bin");
string ErrorsFile("C:\\Users\\bahar\\Desktop\\errorsList.bin");

int main(int argc, char* argv[])
{
    shared_ptr<menu> Menu(new menu);
    system("chcp 1251");
    system("cls");
    setlocale(LC_ALL, "Rus");
    menu::userChoice();
    return 0;
}
struct UserNumber {
    Task task;
    Error error;
    int number = NULL;
};
void menu::userChoice() {
    while (true) 
    {
        string name, access;
        string password;
        int temp;
        cout << "Какого пользователя хотите выбрать?" << endl;
        fstream file;
        User a;
        file.open(UserFile, ios::app | ios::out | ios::in | ios::binary);
        int i = 1;
        if (file) {
            file.seekg(0);
            while (file.read(reinterpret_cast<char*>(&a), sizeof(User))) {
                cout << i << ") " << a.getUserName() << ": " << (a.getStatus() ? "Админ" : "Пользователь") << endl;
                i++;
            }
            file.close();
            cout << "\n" << "[-1] Создать нового пользователя" << endl;
            cout << "[0] Выход" << endl;
        }
        else {
            cout << "Ошибочка вышла";
        }
        temp = GetInt();
        cin.ignore(10000, '\n');
        cin.clear();
        if (temp > 0 && temp < i) {
            int pos = ((int)temp - 1) * sizeof(User);
            file.open(UserFile, ios::app | ios::out | ios::in | ios::binary);
            file.seekp(pos, ios_base::beg);
            file.read(reinterpret_cast<char*>(&a), sizeof(User));
            file.close();
            cout << "Введите пароль:" << endl;
            password = InputPassword();
            if (to_string(hash_value(password)) == a.getPassword()) {
                cout << "\n" << "Пароль совпал" << endl;
                if (a.getStatus()) {
                    menu::adminMenu();
                }
                else {
                    menu::userMenu(a);
                }
            }
            else {
                cout << "\nНеправильный пароль" << endl;
            }
        }
        else if (temp == -1) {
            menu::createUser();
        } 
        else if (temp == 0) {
            exit(0);
        }
        else {
            cout << "Такого пользователя нет" << endl;
        }
    }
}

void menu :: createUser() {
    string name;
    string password;
    system("cls");
    cout << "Введите имя пользователя:" << endl;
    getline(cin, name);
    cout << "Введите тип доступа:\n\t[0] - Админ\n\t[1] - Пользователь" << endl;
    int access = GetInt();
    cin.ignore(10000, '\n');
    cin.clear();
    bool found = false;
    cout << "Введите пароль:" << endl;
    char c;
    int len;
    password = InputPassword();
    while ((c = _getch()) != '\r')
    {
        password.push_back(c);
        _putch('*');
    }

    fstream file;
    User a;
    file.open(UserFile, ios::app | ios::out | ios::in | ios::binary);
    file.seekg(0);
    while (file.read(reinterpret_cast<char*>(&a), sizeof(User))) {
        if (name == a.getUserName()) {
            cout << "Пользователь с этим именем уже существует" << endl;
            system("pause");
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        User user;
        user.setInformation(name,access == 0, password);
        WriteToFile(user, UserFile);
        file.close();
    }
    system("cls");
}

void menu :: userMenu(User user) {
    system("cls");
    cout << "userMenu" << endl;
    
    while (true) {
        cout << "Что бы вы хотели просмотреть?\n\t[1] - Задачи\n\t[2] - Ошибки\n\t[0] - Выход" << endl;
        int t = GetInt();
        if (t == 1) {
            menu::showTasks(user);
        }
        else if (t == 2) {
            menu::showErrors(user);
        }
        else if (t == 0) {
            system("cls");
            break;
        }
        else {
            cout << "Введите корретный номер" << endl;
        }
    }  
}
void menu::showTasks(User user) {
    while (true) {
        system("cls");
        cout << "Задачи" << endl;
        bool sorting  = false; 
        fstream file;
        vector<Task> vecTask;
        subarray<Task> task0;
        vector<subarray<Task>> vecTask0;
        Task a;
        file.open(TasksFile, ios::app | ios::out | ios::in | ios::binary);
        int i = 1;
        int j = 1;
        if (file) {
            file.seekg(0);
            while (file.read(reinterpret_cast<char*>(&a), sizeof(Task))) {
                vecTask.push_back(a);
                if (a.getOwner() == user.getUserName()) {
                    
                    task0.entry = a;
                    task0.number = i - 1;
                    vecTask0.push_back(task0);
                    cout << j << ")";
                    a.getInformation();
                    j++;
                }
                i++;
            }
            file.close();
            cout << "[-1] - Отсортровать по завершенности" << endl;
            cout << "[-2] - Поиск записи по имени" << endl;
            cout << "[0] Выход" << endl;
        }
        else {
            cout << "Ошибочка вышла";
        }
        int temp = GetInt();
        if (temp > 0 && temp < j) {
            cin.ignore(10000, '\n');
            cin.clear();
            vecTask0[temp - 1].entry.setDone();
        }
        else if (temp == -1) {
            sorting = true;
        }
        else if (temp == 0) {
            system("cls");
            break;
        }
        else if (temp == -2) {
            string search;
            cin.ignore(10000, '\n');
            cin.clear();
            getline(cin, search);
            for (int i = 0; i < vecTask0.size(); i++) {
                if (search == vecTask0[i].entry.getName()) {
                    vecTask0[i].entry.getInformation();
                    system("pause");
                }
            }
            
        }
        else {
            cout << "Такой записи нет" << endl;
        }

        cin.ignore(10000, '\n');
        cin.clear();
        file.open(TasksFile, ios::trunc | ios::out | ios::in | ios::binary);
        file.close();
        for (int i = 0; i < vecTask0.size(); i++) {
            vecTask[vecTask0[i].number] = vecTask0[i].entry;
        }
        if (sorting) {
             sort(vecTask.begin(), vecTask.end());
        }
        for (int i = 0; i < vecTask.size(); i++) {
            
            WriteToFile(vecTask[i], TasksFile);
        }
        file.close();
    }
}
void menu::showErrors(User user) {
    while (true) {
        system("cls");
        cout << "Ошибки\n" << endl;
        bool sorting = false;
        fstream file;
        vector<Error> vecErr;
        subarray<Error> err0;
        vector<subarray<Error>> vecErr0;
        Error a;
        file.open(ErrorsFile, ios::app | ios::out | ios::in | ios::binary);
        int i = 1;
        int j = 1;
        if (file) {
            file.seekg(0);
            while (file.read(reinterpret_cast<char*>(&a), sizeof(Error))) {
                vecErr.push_back(a);
                if (a.getOwner() == user.getUserName()) {

                    err0.entry = a;
                    err0.number = i - 1;
                    vecErr0.push_back(err0);
                    cout << j << ")";
                    a.getInformation();
                    j++;

                }
                i++;
            }
            file.close();
            cout << "[-1] - Отсортировать по завершенности" << endl;
            cout << "[-2] - Поиск по имени" << endl;
            cout << "[0] Выход" << endl;
        }
        else {
            cout << "Ошибочка вышла";
        }
        int temp = GetInt();
        if (temp > 0 && temp < j) {
            cin.ignore(10000, '\n');
            cin.clear();
            vecErr0[temp - 1].entry.setDone();
        }
        else if (temp == -1) {
            sorting = true;
        }
        else if (temp == -2) {
            string search;
            cin.ignore(10000, '\n');
            cin.clear();
            getline(cin, search);
            for (int i = 0; i < vecErr0.size(); i++) {
                if (search == vecErr0[i].entry.getName()) {
                    vecErr0[i].entry.getInformation();
                    system("pause");
                }
            }
        }
        else if (temp == 0) {
            system("cls");
            break;
        }
        else {
            cout << "Такой записи нет" << endl;
        }

        cin.ignore(10000, '\n');
        cin.clear();
        file.open(ErrorsFile, ios::trunc | ios::out | ios::in | ios::binary);
        file.close();
        for (int i = 0; i < vecErr0.size(); i++) {
            vecErr[vecErr0[i].number] = vecErr0[i].entry;
        }
        if (sorting) {
            sort(vecErr.begin(), vecErr.end());
        }
        
        for (int i = 0; i < vecErr.size(); i++) {

            WriteToFile(vecErr[i], ErrorsFile);
        }
        file.close();
    }
}
void menu::adminMenu() {
    system("cls");
    cout << "Admin menu" << endl;
    while (true) {
        cout << "Что бы вы хотели просмотреть?\n\t[1] - Задачи\n\t[2] - Ошибки\n\t[0] - Выход" << endl;
        int t = GetInt();
        if (t == 1) {
            menu::showTasks();
        }
        else if (t == 2) {
            menu::showErrors();
        }
        else if (t == 0) {
            system("cls");
            break;
        }
        else {
            cout << "Введите корретный номер" << endl;
        }
    }
}
void addTask(vector<Task> &vecTask)
{
    system("cls");
    cout << "Введите имя задачи, которую хотите создать: ";
    Task task;
    string name;
    bool found = false;
    cin.ignore(10000, '\n');
    cin.clear();
    getline(cin, name);
    for (int i = 0; i < vecTask.size(); i++) {
        if (vecTask[i].getName() == name) {
            cout << "Такое имя уже существует" << endl;
            found = true;
        }
    }
    if (found) {
        cout << "Такое имя уже существует" << endl;
        addTask(vecTask);
    }
    task.setInformation(name);
    vecTask.push_back(task);
}

void editTask(vector<Task> &vecTask, int i)
{
    system("cls");
    cin.ignore(10000, '\n');
    cin.clear();
    cout << "Введите новое имя задачи: ";
    string name;
    bool found = false;
    getline(cin, name);
    for (int j = 0; j < vecTask.size(); j++) {
        if (vecTask[j].getName() == name) {
            cout << "Такое имя уже существует" << endl;
            found = true;
        }
    }
    if (found) {
        cout << "Такое имя уже существует" << endl;
        editTask(vecTask, i);
    }
    vecTask[--i].setInformation(name);
    
}
void delTask(vector<Task>& vecTask, int i) {
    vector<Task>::iterator del = vecTask.begin();
    del += --i;
    vecTask.erase(del);
}
void delErr(vector<Error>& vecErr, int i) {
    vector<Error>::iterator del = vecErr.begin();
    del += --i;
    vecErr.erase(del);
}

void menu::showTasks() {
    while (true) {
        system("cls");
        cout << "Какую запись хотите отредактировать" << endl;
        fstream file;
        vector<Task> vecTask;
        Task a;
        file.open(TasksFile, ios::app | ios::out | ios::in | ios::binary);
        int i = 1;
        if (file) {
            file.seekg(0);
            while (file.read(reinterpret_cast<char*>(&a), sizeof(Task))) {

                cout << i << ")";
                a.getInformation();
                vecTask.push_back(a);
                i++;
            }
            file.close();
            cout << "\n" << "[-1] Создать новую запись" << endl;
            cout << "[-2] Отсортировать по завершенности" << endl;
            cout << "[-3] Калькулятор" << endl;
            cout << "[-4] Поиск по имени" << endl;
            cout << "[0] Выход" << endl;
        }
        else {
            cout << "Ошибочка вышла";
        }
        cin.ignore(10000, '\n');
        cin.clear();
        int temp = GetInt();
        if (temp > 0 && temp < i) {
            cin.ignore(10000, '\n');
            cin.clear();
            int y;
            cout << "[0] - Отметить как сделанное\n[1] - Изменить\n[2] - Удалить" << endl;
            y = GetInt();
            if (y == 0) {
                vecTask[--temp].setDone();
            }
            else if (y == 1) {
                editTask(vecTask, temp);
            }
            else if (y == 2) {
                delTask(vecTask, temp);
            }
            else {
                cout << "Некоретный номер" << endl;
            }
        }
        else if (temp == -1) {
            addTask(vecTask);
        }
        else if (temp == -2) {
            sort(vecTask.begin(), vecTask.end());
        }
        else if (temp == -3) {
            system("calc");
        }
        else if (temp == -4) {
            string search;
            cin.ignore(10000, '\n');
            cin.clear();
            getline(cin, search);
            for (int i = 0; i < vecTask.size(); i++) {
                if (search == vecTask[i].getName()) {
                    vecTask[i].getInformation();
                    system("pause");
                }
            }
        }
        else if (temp == 0) {
            system("cls");
            break;
        }
        else {
            cout << "Такой записи нет" << endl;
        }
        
        cin.ignore(10000, '\n');
        cin.clear();
        file.open(TasksFile, ios::trunc | ios::out | ios::in | ios::binary);
        file.close();
        for (int i = 0; i < vecTask.size(); i++) {
            WriteToFile(vecTask[i], TasksFile);
        }
        file.close();
    }
}
void menu::editErr(vector<Error>& vecErr, int i) {
    system("cls");
    cout << "Введите новое имя ошибки: ";
    string name;
    bool found = false;
    getline(cin, name);
    for (int j = 0; j < vecErr.size(); j++) {
        if (vecErr[j].getName() == name) {
            cout << "Такое имя уже существует" << endl;
            found = true;
        }
    }
    if (found) {
        cout << "Такое имя уже существует" << endl;
        editErr(vecErr, i);
    }
    vecErr[--i].setInformation(name);
}
void addErr(vector<Error> &vecErr) {
    system("cls");
    cout << "Введите имя ошибки, которую хотите создать: ";
    Error error;
    string name;
    bool found = false;
    getline(cin, name);
    for (int i = 0; i < vecErr.size(); i++) {
        if (vecErr[i].getName() == name) {
            cout << "Такое имя уже существует" << endl;
            found = true;
        }
    }
    if (found) {
        cout << "Такое имя уже существует" << endl;
        addErr(vecErr);
    }
    error.setInformation(name);
    vecErr.push_back(error);
}
void menu::showErrors() {
    while (true) {
        system("cls");
        cout << "Какую запись хотите отредактировать" << endl;
        fstream file;
        vector<Error> vecErr;
        Error a;
        file.open(ErrorsFile, ios::app | ios::out | ios::in | ios::binary);
        int i = 1;
        if (file) {
            file.seekg(0);
            while (file.read(reinterpret_cast<char*>(&a), sizeof(Error))) {

                cout << i << ")";
                a.getInformation();
                vecErr.push_back(a);
                i++;
            }
            file.close();
            cout << "\n" << "[-1] Создать новую запись" << endl;
            cout << "[-2] Отсортировать по завершенности" << endl;
            cout << "[-3] Найти по имени" << endl;
            cout << "[0] Выход" << endl;
        }
        else {
            cout << "Ошибочка вышла";
        }
        int temp = GetInt();
        if (temp > 0 && temp < i) {
            cin.ignore(10000, '\n');
            cin.clear();
            int y;
            cout << "[0] - Отметить как сделанное\n[1] - Изменить\n[2] - Удалить" << endl;
            y = GetInt();
            if (y == 0) {
                vecErr[--temp].setDone();
            }
            else if (y == 1) {
               menu::editErr(vecErr, temp);
            }
            else if (y == 2) {
                delErr(vecErr, temp);
            }
            else {
                cout << "Некоретный номер" << endl;
            }
        }
        else if (temp == -1) {
            addErr(vecErr);
        }
        else if (temp == -2) {
            sort(vecErr.begin(), vecErr.end());
        }
        else if (temp == -3) {
            string search;
            cin.ignore(10000, '\n');
            cin.clear();
            getline(cin, search);
            for (int i = 0; i < vecErr.size(); i++) {
                if (search == vecErr[i].getName()) {
                    vecErr[i].getInformation();
                    system("pause");
                }
            }
        }
        else if (temp == 0) {
            system("cls");
            break;
        }
        else {
            cout << "Такой записи нет" << endl;
        }

        cin.ignore(10000, '\n');
        cin.clear();
        file.open(ErrorsFile, ios::trunc | ios::out | ios::in | ios::binary);
        file.close();
        for (int i = 0; i < vecErr.size(); i++) {
            WriteToFile(vecErr[i], ErrorsFile);
        }
        file.close();
    }
}


