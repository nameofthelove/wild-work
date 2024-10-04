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
                printf("%s %s %d\n", estate.name.c_str(), estate.date.c_str(), estate.cost);
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