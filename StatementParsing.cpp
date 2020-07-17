//
// Created by Nik Carlson on 17.07.2020.
//

#include <iostream>
#include <string>
#include "ASTree/ASTree.h"
#include "Parser/Parser.h"

void printAns() {
    std::string str;
    std::getline(std::cin, str);
    ASTree *tree = parse(str);
    std::cout << toString(tree) << std::endl;
}

int main() {
    printAns();
    return 0;
}