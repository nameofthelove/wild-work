#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>

using namespace std;

class DataInfo {
private:
    struct RealEstate {
        string name;
        string date;
        int cost;

        RealEstate(const string& name_, const string& date_, int cost_) : name(name_), date(date_), cost(cost_) {}
    };
    vector<RealEstate> estateData;
public:
    void read_info(const string& s) {
        string name, date;
        int cost = 0;

        regex namePattern(R"([A-Za-zА-Яа-я]+\s[A-Za-zА-Яа-я]+)");
        regex datePattern(R"((\d{4})\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01]))");
        regex costPattern(R"(\b\d+\b)");

        smatch match;

        if (regex_search(s, match, namePattern)) name = match.str();
        if (regex_search(s, match, datePattern)) date = match.str();
        string remaining = regex_replace(s, datePattern, "");
        if (regex_search(remaining, match, costPattern)) cost = stoi(match.str());

        if (!name.empty() && !date.empty() && cost != 0) {
            estateData.push_back(RealEstate(name, date, cost));
        }
    }

    void cout_result() const {
        if (!estateData.empty()) {
            cout << "Недвижимость:" << endl;
            for (const auto& estate : estateData) {
                printf("%s %s %d\n", estate.name.c_str(), estate.date.c_str(), estate.cost);
            }
        }
        else {
            cout << "Данные считаны неверно, проверьте файл." << endl;
        }
    }

    // Функция для фильтрации и сортировки списка недвижимости по цене
    void filter_and_sort(int minPrice, int maxPrice) {
        // Фильтруем недвижимость по заданному диапазону цен
        vector<RealEstate> filteredEstates;
        for (const auto& estate : estateData) {
            if (estate.cost >= minPrice && estate.cost <= maxPrice) {
                filteredEstates.push_back(estate);
            }
        }

        // Сортируем от минимальной к максимальной цене
        sort(filteredEstates.begin(), filteredEstates.end(), [](const RealEstate& a, const RealEstate& b) {
            return a.cost < b.cost;
            });

        // Выводим отфильтрованные и отсортированные данные
        if (!filteredEstates.empty()) {
            cout << "Отфильтрованная недвижимость (цены от " << minPrice << " до " << maxPrice << "):" << endl;
            for (const auto& estate : filteredEstates) {
                printf("%s %s %d\n", estate.name.c_str(), estate.date.c_str(), estate.cost);
            }
        }
        else {
            cout << "Нет недвижимости в заданном диапазоне цен." << endl;
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

    // Пример вызова функции фильтрации и сортировки
    int minPrice;
    int maxPrice;
    cout << "Введите минимальную и максимальную цену: ";
    cin >> minPrice >> maxPrice;
    result.filter_and_sort(minPrice, maxPrice);

    return 0;
}
