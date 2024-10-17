#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>

using namespace std;

class DataInfo {
public:
    struct RealEstate {
        string name;
        string date;
        int cost;

        RealEstate(const string& name_, const string& date_, int cost_) : name(name_), date(date_), cost(cost_) {}

        // Перегрузка оператора >>
        friend ostream& operator<<(ostream& os, const RealEstate& estate) {
            os << estate.name << " " << estate.date << " " << estate.cost;
            return os;
        }
    };

    vector<RealEstate> estateData;

    // Публичная функция для извлечения имени
    string extract_name(const string& s) {
        regex namePattern(R"("[A-Za-zА-Яа-я]+\s[A-Za-zА-Яа-я]+")");
        smatch match;
        if (regex_search(s, match, namePattern)) {
            return match.str();
        }
        return "";
    }

    // Публичная функция для извлечения даты
    string extract_date(const string& s) {
        regex datePattern(R"((\d{4})\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01]))");
        smatch match;
        if (regex_search(s, match, datePattern)) {
            return match.str();
        }
        return "";
    }

    // Публичная функция для извлечения стоимости
    int extract_cost(const string& s) {
        regex costPattern(R"(\b\d{3,}\b)");  // Найти числа, которые имеют 3 или более цифр, чтобы избежать захвата года
        smatch match;
        if (regex_search(s, match, costPattern)) {
            return stoi(match.str());
        }
        return 0;
    }

    // Чтение строки и извлечение информации
    void read_info(const string& s) {
        string name = extract_name(s);
        string date = extract_date(s);

        // Удаляем найденную дату из строки, чтобы она не повлияла на поиск стоимости
        string remaining = regex_replace(s, regex(R"((\d{4})\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01]))"), "");
        int cost = extract_cost(remaining);

        // Убираем кавычки из имени
        name.erase(remove(name.begin(), name.end(), '\"'), name.end());

        if (!name.empty() && !date.empty() && cost != 0) {
            estateData.push_back(RealEstate(name, date, cost));
        }
    }

    // Вывод данных о недвижимости
    void cout_result() const {
        if (!estateData.empty()) {
            cout << "Недвижимость:" << endl;
            for (const auto& estate : estateData) {
                cout << estate << endl; // Используем перегруженный оператор >>
            }
        }
        else {
            cout << "Данные считаны неверно, проверьте файл." << endl;
        }
    }

    // Фильтрация данных по цене
    void gap(int minPrice, int maxPrice) const {
        vector<RealEstate> filteredData;
        for (const auto& estate : estateData) {
            if (estate.cost >= minPrice && estate.cost <= maxPrice) {
                filteredData.push_back(estate);
            }
        }

        if (!filteredData.empty()) {
            sort(filteredData.begin(), filteredData.end(), [](const RealEstate& a, const RealEstate& b) {
                return a.cost < b.cost;
                });

            for (const auto& estate : filteredData) {
                cout << estate << endl; // Используем перегруженный оператор >>
            }
        }
        else {
            cout << "Нет данных для вывода в заданном диапазоне цен." << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    ifstream file_in("test_data.txt");

    if (!file_in.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return 1;
    }

    DataInfo result;
    string line;

    while (getline(file_in, line)) {
        result.read_info(line);
    }

    result.cout_result();

    int minPrice, maxPrice;
    cout << "Введите минимальную цену: ";
    cin >> minPrice;
    cout << "Введите максимальную цену: ";
    cin >> maxPrice;

    result.gap(minPrice, maxPrice);

    return 0;
}

