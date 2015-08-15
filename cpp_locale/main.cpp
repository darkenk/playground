#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <locale>

using namespace std;

int main()
{
    typedef map<std::string, int> count_map;
    typedef string::size_type str_size;
    int signals;
    signals wp;
    wp = 12;

    // set locale to native locale (user defined)
    locale native{""};
    cin.imbue(native);
    cout.imbue(native);

    count_map counts{};

    string word{};
    // read words until stream is valid
    while (cin >> word) {
        string copy;
        for (char ch : word) {
            if (isalnum(ch, native)) {
                copy.push_back(ch);
            }
        }
        if (not copy.empty()) {
            ++counts[copy];
        }
    }

    str_size longest{0};
    for (auto pair : counts) {
        if (pair.first.size() > longest) {
            longest = pair.first.size();
        }
    }

    const int count_size{10};
    for (auto pair : counts) {
        cout << setw(longest) << left << pair.first <<
                setw(count_size) << right << pair.second << '\n';
    }
}

