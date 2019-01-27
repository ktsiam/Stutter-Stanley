#include <iostream>

int main() {
        std::string s;
        
        
        std::cout << (char)std::cin.peek() << std::endl;
        char c; std::cin >> c;
        std::cout << c;
        std::cout << (char)std::cin.peek() << std::endl;

}
