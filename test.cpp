#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
 
using namespace std;

int main(){
    std::ifstream ifs("index.html");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );

    cout << content << endl;

}
