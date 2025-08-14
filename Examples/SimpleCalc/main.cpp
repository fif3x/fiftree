#include "../lib/calculator.h" // modify path to suit needs
#include <iostream>

int main(){
    std::cout << "2 + 1: " << Calculator::add(2, 1) << "\n";
    std::cout << "2 - 1: " << Calculator::subtract(2, 1) << "\n";

    return 0;
}