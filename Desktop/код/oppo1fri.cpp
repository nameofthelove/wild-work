#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>

using namespace std;

class PressureData {
private:
    struct PressureDataItem
    {
        string date;
        double height;
        int pressure;

        PressureDataItem(const string date_, double height_, int pressure_) : date(date_), height(height_), pressure(pressure_) {}
    };
    vector<PressureDataItem> pressureData;


public:
    void read_info(string s) {
        string name;
        double height = 0.0;
        int pressure = 0;

        regex datePattern(R"((\d{4})\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01]))");
        regex heightPattern(R"(\d+\,\d{2})");
        regex pressurePattern(R"((\d+)\s*$)");

        smatch match;

        if (regex_search(s, match, datePattern)) name = match.str();
        if (regex_search(s, match, heightPattern)) height = stod(match.str());
        string remaining = regex_replace(s, regex(R"((\d{4})\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01]))"), "");  // Убираем дату
        if (regex_search(remaining, match, pressurePattern))  pressure = stoi(match.str());

        if (!name.empty() && height != 0.0 && pressure != 0) {
            pressureData.push_back(PressureDataItem(name, height, pressure));
        }
        else {
            cout << "Неверно считаны данные!" << endl;
        }
    }


    void cout_result() const {
        if (!pressureData.empty()) {
            cout << "Измерения давления: " << endl;
            for (const auto& press : pressureData) {
                printf("%s %.2f %d\n", press.date.c_str(), press.height, press.pressure);
            }
        }
        else {
            cout << "Данные считаны неверно!" << endl;
        }
    }

    bool compare(const PressureDataItem& a, const PressureDataItem& b) {
        return a.pressure < b.pressure;
    }
    void filter_by_exact_height(double exactHeight) const {
        vector<PressureDataItem> data;

        for (const auto& d : pressureData) {
            if (d.height == exactHeight) data.push_back(d);
        }

        if (!data.empty()) {
            sort(data.begin(), data.end(), compare);
            for (const auto& item : data) printf("%s %.2f %d\n", item.date.c_str(), item.height, item.pressure);
        }
        else cout << "Данная высота отсутсвует" << endl;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    ifstream file_in("pressure_data.txt");

    if (!file_in) {
        cout << "Ошибка открытия файла." << endl;
        return 1;
    }

    PressureData result;
    string line;

    while (getline(file_in, line)) {
        result.read_info(line);
    }

    result.cout_result();

    double exactHeight;
    cout << "Введите высоту: ";
    cin >> exactHeight;

    result.filter_by_exact_height(exactHeight);
}
