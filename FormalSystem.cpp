//
// Created by Nik Carlson on 22.08.2020.
//


#include <iostream>
#include "Parser/Parser.h"
#include "Axioms/Axioms.h"
#include "RulesOfInference/RulesOfInference.h"

void printAns(const std::string &expression) {
    std::string str;
    std::pair<std::string, std::pair<int, int>> result;
    std::map<std::string, int> checkTakes;
    std::vector<std::pair<std::pair<std::string, std::pair<int, int>>, std::string>> proof;
    std::map<std::string, std::vector<std::pair<std::string, int>>> moduses;
    std::string treeStr;
    int index = 0;
    while (std::getline(std::cin, str) && !str.empty()) {
        bool flag = false;
        ++index;
        ASTree *tree = parse(str, true);
        treeStr = toStringInfix(tree);
        std::pair<std::string, int> axiom = std::make_pair("", -1), hAxiom;
        if (isLogicAxiomScheme(tree, axiom)) flag = true;
        if (isArithmeticAxiom(tree, hAxiom) || isArithmeticAxiomScheme(tree, hAxiom)) {
            if (axiom.second == -1 || hAxiom.second < axiom.second) {
                axiom = std::make_pair(hAxiom.first, hAxiom.second);
            }
            flag = true;
        }
        if (!flag && isPredicateAxiomScheme(tree, axiom)) flag = true;

        if (flag) {
            result = std::make_pair(axiom.first, std::make_pair(axiom.second, 0));
        }

        if (!flag && (isModusPonens(checkTakes, moduses, treeStr, result, true) ||
                      isQuantifierRule(checkTakes, tree, result))) {
            flag = true;
        }

        if (flag) {
            if (result.second.first != -1) {
                if (result.second.second == 0) {
                    std::cout << "[" << index << ". " << result.first <<
                              result.second.first << "] " << treeStr << '\n';

                } else {
                    std::cout << "[" << index << ". " << "M.P. " <<
                              result.second.first << ", " << result.second.second << "] " << treeStr << '\n';
                }
                if (tree->data.getId() == "->")
                    moduses[toStringInfix(tree->right)].emplace_back(toStringInfix(tree->left), index);
                checkTakes[treeStr] = index;
            } else {
                std::cout << "Expression " << index << ": " << result.first << '\n';
                return;
            }
        } else {
            std::cout << "Expression " << index << " is not proved." << '\n';
            return;
        }
    }
    if (expression != treeStr) {
        std::cout << "The proof proves different expression.\n";
    }
}

int main() {

    std::iostream::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::string expression;
    std::getline(std::cin, expression);
    expression = expression.substr(2);
    expression = toStringInfix(parse(expression, true));
    std::cout << "|-" << expression << '\n';
    printAns(expression);

    return 0;
}