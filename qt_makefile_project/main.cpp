#include <iostream>

using namespace std;

#ifdef ENGLISH
string s("hello!");
#else
string s("Witaj!");
#endif

int main(int argc, char* argv[])
{
    cout << s + '\n';
    return 0;
}
