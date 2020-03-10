#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include <regex>

using namespace std;

int main(){
    // string test = "ab\r\ncd\r\nef\r\n";
    string test = "aaaaaa\r\n\r\n";
    /*
    size_t pos=0;
    size_t prev=0;
    
    cout << test.find("c", prev) << endl;
    
    while ((pos = test.find("c", prev) != string::npos)){
        cout << pos << endl;
        prev = pos+1;
    }
    */
    cout << test << endl;
    regex b("[\r\n]{2}");
    if (regex_search(test, b))
        cout << "matched" << endl;
    else
        cout << "not matched" << endl;

    return 0;
}
