#include <iostream>

int main()
{
    char buffer[] = "buffertostring";

    std::string s(buffer);
    std::cout << s  << std::endl;
    return 0;
}

