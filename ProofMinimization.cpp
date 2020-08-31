//
// Created by Nik Carlson on 21.07.2020.
//
#include <iostream>
#include <map>
#include <set>
#include "ASTree/ASTree.h"
#include "Parser/Parser.h"
#include "Axioms/Axioms.h"
#include "RulesOfInference/RulesOfInference.h"

std::map<std::string, int> checkTakes;
std::vector<std::pair<std::pair<std::string, std::pair<int, int>>, std::string>> proof;

void parseFirstString(ASTree *&expression, std::string &allContextStr, std::map<std::string, int> &context) {
    std::string str;
    std::getline(std::cin, str);
    int pointer = str.find("|-");
    std::string allContext = str.substr(0, pointer);
    if (!allContext.empty()) allContext += " ";
    pointer += 2;
    while (str[pointer] == ' ' || str[pointer] == '\r' || str[pointer] == '\t') {
        ++pointer;
    }
    std::string strExpression = str.substr(pointer);
    int pos = 0, quantity = 1;
    for (int i = 0; i < allContext.size(); ++i) {
        if (allContext[i] == ',' || i == allContext.size() - 1) {
            std::string tmp = allContext.substr(pos, i - pos);
            std::string tmp2 = toStringInfix(parse(tmp, false));
            context[tmp2] = quantity++;
            allContextStr += tmp2 + ", ";
            pos = i + 1;
            while (pos < allContext.size() &&
                   (allContext[pos] == ' ' || allContext[pos] == '\r' || allContext[pos] == '\t')) {
                ++pos;
            }
        }
    }
    expression = parse(strExpression, false);
}

bool isHypothesis(ASTree *&proofTree, const std::string &stringProofTree, std::vector<ASTree *> &prevStatements,
                  const std::map<std::string, int> &context) {
    if (context.count(stringProofTree) != 0) {
        proof.emplace_back(std::make_pair("Hypothesis", std::make_pair(context.at(stringProofTree), 0)),
                           stringProofTree);
        prevStatements.emplace_back(proofTree);
        return true;
    }
    return false;
}

void setAnswer(const std::string &expr,
               std::map<int, std::pair<std::pair<std::string, std::pair<int, int>>, std::string>> &answer) {
    if (checkTakes.count(expr) == 0) return;
    int index = checkTakes.at(expr);
    answer.insert(std::make_pair(index, proof[index]));
    if (proof[index].first.first == "M.P.") {
        setAnswer(proof[proof[index].first.second.first].second, answer);
        setAnswer(proof[proof[index].first.second.second].second, answer);
    }
}

void minimize(const std::string &expression, const std::string &allContextSrt) {
    std::map<int, std::pair<std::pair<std::string, std::pair<int, int>>, std::string>> answer;
    setAnswer(expression, answer);
    checkTakes.clear();
    std::string firstString = allContextSrt;
    if (!firstString.empty()) {
        firstString.resize(firstString.size() - 2);
        firstString += " ";
    }
    firstString += "|- " + expression;
    std::cout << firstString << std::endl;

    std::map<std::string, int> newNumbers;
    int ind = 1;
    for (auto &it : answer) {
        newNumbers[it.second.second] = ind;
        ++ind;
    }
    ind = 1;
    for (auto &it : answer) {
        std::cout << "[" << ind++ << ". ";
        if (it.second.first.first == "Ax. sch." || it.second.first.first == "Hypothesis") {
            std::cout << it.second.first.first << " " << it.second.first.second.first << "] " << it.second.second
                      << "\n";
        } else if (it.second.first.first == "M.P.") {
            std::cout << it.second.first.first << " " << newNumbers[proof[it.second.first.second.first].second]
                      << ", "
                      << newNumbers[proof[it.second.first.second.second].second] << "] " << it.second.second << "\n";

        }
    }
}

bool check(const std::string &expression, const std::map<std::string, int> &context) {
    std::map<std::string, std::vector<std::pair<std::string, int>>> modusPonenses;
    std::vector<ASTree *> prevStatements;
    std::string proofStr;
    int index = 0;
    bool isCurrentProof = true, proven;
    ASTree *proofTree = nullptr;
    std::string proofTreeStr;

    while (std::getline(std::cin, proofStr) && !proofStr.empty()) {
        proofTree = parse(proofStr, false);
        proofTreeStr = toStringInfix(proofTree);
        proven = false;
        if (checkTakes.count(proofTreeStr) == 0) {
            //Check Modus Ponens
            std::pair<std::string, std::pair<int, int>> mp;
            if (isModusPonens(checkTakes, modusPonenses, proofTreeStr, mp)) {
                proof.emplace_back(std::make_pair("M.P.",
                                                  std::make_pair(mp.second.first, mp.second.second)), mp.first);
                prevStatements.emplace_back(proofTree);
                proven = true;
            }
            //Check Hypothesis
            if (!proven && isHypothesis(proofTree, proofTreeStr, prevStatements, context)) proven = true;

            std::pair<std::string, int> axiom;
            //Check Axiom
            if (!proven && isLogicAxiomScheme(proofTree, axiom)) {
                proof.emplace_back(std::make_pair(axiom.first, std::make_pair(axiom.second, 0)),
                                   proofTreeStr);
                prevStatements.emplace_back(proofTree);
                proven = true;
            }
            if (proven) {
                checkTakes[proofTreeStr] = index;
                if (proofTree->data.getId() == "->") {
                    modusPonenses[toStringInfix(proofTree->right)].emplace_back(toStringInfix(proofTree->left), index);
                }
                ++index;
            } else {
                isCurrentProof = false;
                break;
            }
        }
    }
    return !(!isCurrentProof || proofTreeStr != expression);
}

int main() {
    std::iostream::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    ASTree *expression;
    std::string allContextStr;
    std::map<std::string, int> context;
    parseFirstString(expression, allContextStr, context);
    std::string expressionStr = toStringInfix(expression);
    if (!check(expressionStr, context)) {
        std::cout << "Proof is incorrect" << std::endl;
    } else {
        minimize(expressionStr, allContextStr);
    }
    return 0;
}