#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

/**
 * 代码网址：https://www.cnblogs.com/catgatp/p/6407783.html
 * @param str 源字符串
 * @param old_value 被替换的部分
 * @param new_value 替换成
 * @return 更改后的字符串
 */
string &replace_all_distinct(string &str, const string &old_value, const string &new_value);

int daysCount(int month, int day);

struct Student {
    string Name;
    string Gender;
    string Major;
    long long Phone = 0;
    string Email;
    int BirthYear = 0;
    int BirthMonth = 0;
    int BirthDay = 0;

    void print() const {
        cout << "Name:" << Name << endl;
        cout << "Gender:" << Gender << endl;
        cout << "Major:" << Major << endl;
        cout << "Phone:" << Phone << endl;
        cout << "Email:" << Email << endl;
        cout << "BirthYear:" << BirthYear << endl;
        cout << "BirthMonth:" << BirthMonth << endl;
        cout << "BirthDay:" << BirthDay << endl;
    }

    void analyse(string str) {
        int now = 0, last;
        for (int section = 0; section < 5; section = (now != 0) ? (section + 1) : 0) {
            last = now;
            now = str.find(',', last + 1);
            if ((now >= 0) || (section == 4)) {
                switch (section) {
                    case 0:
                        this->Name = str.substr(last, now - last);
                        break;
                    case 1:
                        this->BirthYear = stoi(str.substr((last + 7), 4));
                        this->BirthMonth = stoi(str.substr((last + 11), 2));
                        this->BirthDay = stoi(str.substr((last + 13), 2));
                        if (stoi(str.substr((last + 17), 1)) % 2 == 0)
                            this->Gender = "Female";
                        else
                            this->Gender = "Male";
                        break;
                    case 2:
                        this->Major = str.substr(last + 1, now - last - 1);
                        break;
                    case 3:
                        this->Phone = stoll(str.substr(last + 1, 11));
                        break;
                    case 4:
                        this->Email = str.substr(last + 1, (str.length() - now));
                        return;
                    default:
                        break;
                }
            } else {
                cout << "错误" << endl;
                cin >> str;
            }
        }
    }

};

int main(int argc, char *argv[]) {
    string str;
    struct Student A;
    string FilePath = argv[1];
    struct Student Birth;
    string BirthdayPeople, BirthdayPeopleBackup;

    time_t time_seconds = time(nullptr);
    struct tm now_time{};
    localtime_s(&now_time, &time_seconds);

    replace_all_distinct(FilePath, "\\", "\\\\");
    ifstream fin(FilePath, ios::binary);

    int minDays, toDays, tempDays, backup;
    getline(fin, str);
    BirthdayPeople = BirthdayPeopleBackup = str;
    Birth.analyse(str);
    minDays = daysCount(Birth.BirthMonth, Birth.BirthDay);
    toDays = daysCount((now_time.tm_mon + 1), now_time.tm_mday);
    backup = (minDays < toDays) ? minDays : toDays;

    do {
        A.analyse(str);
        tempDays = daysCount(A.BirthMonth, A.BirthDay);
        if (toDays <= minDays) {
            if ((tempDays > toDays) && (tempDays < minDays)) {
                BirthdayPeople = str;
                minDays = tempDays;
            }
        } else if (tempDays > toDays) {
            minDays = tempDays;
            BirthdayPeople = str;
        } else if (tempDays < minDays) {
            minDays = backup = tempDays;
            BirthdayPeople = BirthdayPeopleBackup = str;
        }
        //A.print();
        cout << A.Name << "的生日是" << A.BirthMonth << "月" << A.BirthDay << "日，还有"
             << ((tempDays >= toDays) ? (tempDays - toDays) : (365 + tempDays - toDays)) << "天" << endl;
    } while (getline(fin, str));
    fin.close();

    Birth.analyse(((minDays >= toDays) ? BirthdayPeople : BirthdayPeopleBackup));
    cout << endl << "下一个过生日的同学是" << Birth.Name << "，还有"
         << ((minDays >= toDays) ? (minDays - toDays) : (365 + backup - toDays)) << "天" << endl;
    system("pause");
    return 0;
}

string &replace_all_distinct(string &str, const string &old_value, const string &new_value) {
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else break;
    }
    return str;
}

int daysCount(int month, int day) {
    int power[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = 0;
    for (int i = 0; i < (month - 1); i++) days = days + power[i];
    days = days + day;
    return days;
}