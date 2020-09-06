//
// Created by Nik Carlson on 31.08.2020.
//

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>

#include "ASTree/ASTree.h"
#include "Parser/Parser.h"
#include "PrimitiveProofs/PrimitiveProofs.h"
#include "RulesOfInference/RulesOfInference.h"
#include "Axioms/Axioms.h"


struct Proofs {
    int first{}, second{};
    std::vector<int> values;//0 - not used, -1 -false, 1 - true
};

std::vector<std::vector<bool>> logicPermutations;
std::vector<std::string> variables;
std::vector<std::vector<bool>> hypotheses;
std::vector<int> pair1Indexes, pair2Indexes, pair3Indexes;
std::string outStr;
PrimitiveProofs proofs;

std::vector<Proofs> pairs1, pairs2, pairs3;

bool eval(const ASTree *tree, int argumentsIndex) {
    return eval(tree, logicPermutations[argumentsIndex][0], logicPermutations[argumentsIndex][1],
                logicPermutations[argumentsIndex][2], variables);
}

int getElementaryProofNumber(const bool &left, const bool &right, const std::string &operation) {
    if (operation == "&") {
        if (left && right) return 0;
        if (left && !right) return 1;
        if (!left && right) return 2;
        if (!left && !right) return 3;
    }
    if (operation == "->") {
        if (left && right) return 4;
        if (left && !right) return 5;
        if (!left && right) return 6;
        if (!left && !right) return 7;
    }
    if (operation == "|") {
        if (left && right) return 8;
        if (left && !right) return 9;
        if (!left && right) return 10;
        if (!left && !right) return 11;
    }
    if (operation == "!") {
        if (left) return 12;
        return 13;
    }
    return -1;
}

void generateSubProof(ASTree *tree, std::vector<ASTree *> &subProof, int argumentsIndex) {
    if (tree->data.isVariable()) {
        subProof.push_back(tree);
        return;
    }
    std::vector<ASTree *> tmpProof;
    int proofNumber;
    bool left, right;
    if (tree->data.getId() == "!") {
        if (tree->left->data.isVariable()) {
            tmpProof = proofs.getWithReplace(13, tree->left);
        } else if (tree->left->data.getId() == "!") {
            generateSubProof(tree->left->left, subProof, argumentsIndex);
            tmpProof = proofs.getWithReplace(12, tree->left->left);
        } else {
            left = eval(tree->left->left, argumentsIndex);
            right = eval(tree->left->right, argumentsIndex);
            if (!left) {
                auto *noLeft = new ASTree(Negation(), tree->left->left, nullptr);
                generateSubProof(noLeft, subProof, argumentsIndex);
                delete noLeft;
            } else {
                generateSubProof(tree->left->left, subProof, argumentsIndex);
            }

            if (!right) {
                auto *noRight = new ASTree(Negation(), tree->left->right, nullptr);
                generateSubProof(noRight, subProof, argumentsIndex);
                delete noRight;
            } else {
                generateSubProof(tree->left->right, subProof, argumentsIndex);
            }
            proofNumber = getElementaryProofNumber(left, right, tree->left->data.getId());
            tmpProof = proofs.getWithReplace(proofNumber, tree->left->left, tree->left->right);
        }
    } else {
        left = eval(tree->left, argumentsIndex);
        right = eval(tree->right, argumentsIndex);
        if (!left) {
            generateSubProof(new ASTree(Negation(), tree->left, nullptr), subProof, argumentsIndex);
        } else {
            generateSubProof(tree->left, subProof, argumentsIndex);
        }

        if (!right) {
            generateSubProof(new ASTree(Negation(), tree->right, nullptr), subProof, argumentsIndex);
        } else {
            generateSubProof(tree->right, subProof, argumentsIndex);
        }
        proofNumber = getElementaryProofNumber(left, right, tree->data.getId());
        tmpProof = proofs.getWithReplace(proofNumber, tree->left, tree->right);
        for (auto i: tmpProof) {
            std::string a = toStringInfix(i);
        }
    }
    subProof.reserve(subProof.size() + tmpProof.size());
    subProof.insert(subProof.end(), tmpProof.begin(), tmpProof.end());
}

bool checkEquals(const std::vector<bool> &v1, const std::vector<bool> &v2) {
    int counter = 0;
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) ++counter;
    }
    return counter == 1;
}

bool checkEquals(const std::vector<int> &v1, const std::vector<int> &v2) {
    int counter = 0;
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] == 0 && v2[i] == 0) continue;
        if ((v1[i] == 0 && v2[i] != 0) || (v1[i] != 0 && v2[i] == 0)) counter = 10;
        else if (v1[i] != v2[i]) ++counter;
    }
    return counter == 1;
}

void bruteForce(std::vector<Proofs> &result) {
    for (int i = 0; i < hypotheses.size(); ++i) {
        for (int j = i; j < hypotheses.size(); ++j) {
            if (i != j)
                if (checkEquals(hypotheses[i], hypotheses[j])) {
                    result.emplace_back();
                    (result.end() - 1)->first = i;
                    (result.end() - 1)->second = j;
                    for (int k = 0; k < hypotheses[i].size(); ++k) {
                        if (hypotheses[i][k] != hypotheses[j][k])
                            (result.end() - 1)->values.push_back(0);
                        else
                            (result.end() - 1)->values.push_back(hypotheses[i][k] ? 1 : -1);
                    }
                }
        }
    }
}

void bruteForce(const std::vector<Proofs> &prevIteration, std::vector<Proofs> &result) {
    for (int i = 0; i < prevIteration.size(); ++i) {
        for (int j = i; j < prevIteration.size(); ++j) {
            if (i != j)
                if (checkEquals(prevIteration[i].values, prevIteration[j].values)) {
                    result.emplace_back();
                    (result.end() - 1)->first = i;
                    (result.end() - 1)->second = j;
                    for (int k = 0; k < prevIteration[i].values.size(); ++k) {
                        if (prevIteration[i].values[k] != prevIteration[j].values[k])
                            (result.end() - 1)->values.push_back(0);
                        else
                            (result.end() - 1)->values.push_back(prevIteration[i].values[k]);
                    }
                }
        }
    }
}

bool isUniqueVector(std::vector<int> &tmpVector) {
    for (int j = 0; j < tmpVector.size(); ++j) {
        for (int k = j + 1; k < tmpVector.size(); ++k) {
            if (tmpVector[j] == tmpVector[k])
                return false;
        }
    }
    return true;
}

bool isMode(const std::vector<int> &vector, int mode) {
    for (auto j:vector) {
        if (j != mode && j != 0)
            return false;
    }
    return true;
}

void getIndexes(std::vector<int> &argumentsIndex, bool mode) {
    int variableQuantity = hypotheses[0].size();
    bruteForce(pairs1);
    --variableQuantity;
    if (variableQuantity != 0) {
        bruteForce(pairs1, pairs2);
        --variableQuantity;
    }
    if (variableQuantity != 0)
        bruteForce(pairs2, pairs3);

    int newMode = mode ? 1 : -1;
    bool found = false;
    if (!pairs3.empty()) {
        std::vector<int> tmpVector(8);
        for (int i = 0; i < pairs3.size(); ++i) {
            tmpVector[0] = pairs1[pairs2[pairs3[i].first].first].first;
            tmpVector[1] = pairs1[pairs2[pairs3[i].first].first].second;
            tmpVector[2] = pairs1[pairs2[pairs3[i].first].second].first;
            tmpVector[3] = pairs1[pairs2[pairs3[i].first].second].second;
            tmpVector[4] = pairs1[pairs2[pairs3[i].second].first].first;
            tmpVector[5] = pairs1[pairs2[pairs3[i].second].first].second;
            tmpVector[6] = pairs1[pairs2[pairs3[i].second].second].first;
            tmpVector[7] = pairs1[pairs2[pairs3[i].second].second].second;

            if (isUniqueVector(tmpVector)) {
                pair3Indexes.push_back(i);
                pair2Indexes.push_back(pairs3[i].first);
                pair2Indexes.push_back(pairs3[i].second);
                pair1Indexes.push_back(pairs2[pairs3[i].first].first);
                pair1Indexes.push_back(pairs2[pairs3[i].first].second);
                pair1Indexes.push_back(pairs2[pairs3[i].second].first);
                pair1Indexes.push_back(pairs2[pairs3[i].second].second);
                found = true;
                break;
            }
        }
    } else if (!pairs2.empty()) {
        std::vector<int> tmpVector(4);
        for (int i = 0; i < pairs2.size(); ++i) {
            tmpVector[0] = pairs1[pairs2[i].first].first;
            tmpVector[1] = pairs1[pairs2[i].first].second;
            tmpVector[2] = pairs1[pairs2[i].second].first;
            tmpVector[3] = pairs1[pairs2[i].second].second;
            if (!isMode(pairs2[i].values, newMode)) continue;
            if (isUniqueVector(tmpVector)) {
                for (int j = 0; j < pairs2[i].values.size(); ++j) {
                    if (pairs2[i].values[j] == 1) {
                        outStr += variables[j] + ", ";
                    } else if (pairs2[i].values[j] == -1) {
                        outStr += "(!" + variables[j] + ")" + ", ";
                    }
                }
                pair2Indexes.push_back(i);
                pair1Indexes.push_back(pairs2[i].first);
                pair1Indexes.push_back(pairs2[i].second);
                if (!outStr.empty()) {
                    outStr.resize(outStr.size() - 2);
                    outStr += " ";
                }
                found = true;
                break;
            }
        }
    }
    if (!found && !pairs1.empty()) {
        std::vector<int> tmpVector(2);
        int q = 0;
        for (auto &i : pairs1) {
            tmpVector[0] = i.first;
            tmpVector[1] = i.second;

            if (!isMode(i.values, newMode)) {
                ++q;
                continue;
            }
            if (isUniqueVector(tmpVector)) {
                pair1Indexes.push_back(q);
                for (int j = 0; j < i.values.size(); ++j) {
                    if (i.values[j] == 1) {
                        outStr += variables[j] + ", ";
                    } else if (i.values[j] == -1) {
                        outStr += "(!" + variables[j] + ")" + ", ";
                    }
                }
                if (!outStr.empty()) {
                    outStr.resize(outStr.size() - 2);
                    outStr += " ";
                }
                break;
            }
        }
    }
}

std::vector<ASTree *> getHypotheses(Proofs &pair1, bool mode) {
    std::vector<bool> left = hypotheses[pair1.first];
    std::vector<bool> right = hypotheses[pair1.second];

    std::vector<ASTree *> result;
    if (!mode) {
        for (int i = 0; i < left.size(); ++i) {
            if (!left[i]) {
                std::string tmp = "!" + variables[i];
                result.push_back(parse(tmp));
            } else if (left[i])
                result.push_back(parse(variables[i]));
        }
    } else {
        for (int i = 0; i < right.size(); ++i) {
            if (!right[i]) {
                std::string tmp = "!" + variables[i];
                result.push_back(parse(tmp));
            } else if (right[i])
                result.push_back(parse(variables[i]));
        }
    }
    return result;
}

ASTree *getVarToDeduction(Proofs &pair1, bool mode) {
    ASTree *result = nullptr;
    std::vector<bool> left = hypotheses[pair1.first];
    std::vector<bool> right = hypotheses[pair1.second];

    for (int i = 0; i < left.size(); ++i) {
        if (left[i] != right[i]) {
            if (!mode) {
                if (!left[i]) {
                    std::string tmp = "!" + variables[i];
                    result = parse(tmp);
                } else if (left[i])
                    result = parse(variables[i]);
            } else {
                if (!left[i]) {
                    result = parse(variables[i]);
                } else if (left[i]) {
                    std::string tmp = "!" + variables[i];
                    result = parse(tmp);
                }
            }
            break;
        }
    }
    return result;
}

std::vector<ASTree *> getHypotheses2(Proofs &pair, bool mode, int index) {
    std::vector<int> left = pairs1[pair.first].values;
    std::vector<int> right = pairs1[pair.second].values;
    std::vector<ASTree *> result;
    if (!mode) {
        for (int i = 0; i < left.size(); ++i) {
            if (i == index) continue;
            if (left[i] != 0) {
                if (left[i] == -1) {
                    std::string tmp = "!" + variables[i];
                    result.push_back(parse(tmp));
                } else {
                    result.push_back(parse(variables[i]));
                }
            }
        }
    } else {
        for (int i = 0; i < right.size(); ++i) {
            if (i == index) continue;
            if (right[i] != 0) {
                if (right[i] == -1) {
                    std::string tmp = "!" + variables[i];
                    result.push_back(parse(tmp));
                } else {
                    result.push_back(parse(variables[i]));
                }
            }
        }
    }
    return result;
}

ASTree *getVarToDeduction2(Proofs &pair, bool mode, int &index) {
    ASTree *result = nullptr;
    std::vector<int> left = pairs1[pair.first].values;
    std::vector<int> right = pairs1[pair.second].values;

    for (int i = 0; i < left.size(); ++i) {
        if (left[i] != right[i] && left[i] != 0 && right[i] != 0) {
            if (!mode) {
                if (left[i] == -1) {
                    index = i;
                    std::string tmp = "!" + variables[i];
                    result = parse(tmp);
                } else if (left[i]) {
                    index = i;
                    result = parse(variables[i]);
                }
            } else {
                if (left[i] == -1) {
                    index = i;
                    result = parse(variables[i]);
                } else if (left[i]) {
                    index = i;
                    std::string tmp = "!" + variables[i];
                    result = parse(tmp);
                }
            }
            break;
        }
    }
    return result;
}

void minimize(ASTree *tree, std::vector<ASTree *> &proof, std::vector<std::vector<ASTree *>> &subProofs,
              std::vector<int> &argumentsIndex, bool mode) {

    getIndexes(argumentsIndex, mode);
    if (pair1Indexes.empty()) {
        if (mode) {
            for (const auto &i: variables) outStr += i + ", ";
            outStr.resize(outStr.size() - 2);
            outStr += " |- " + toStringInfix(tree);
            proof = *(subProofs.end() - 1);
        } else {
            for (const auto &i: variables) outStr += std::string("(!") + i + "), ";
            outStr.resize(outStr.size() - 2);
            outStr += " |- " + toStringInfix(tree);
            proof = *subProofs.begin();
        }
        return;
    }
    outStr += "|- " + toStringInfix(tree);

    std::vector<std::vector<ASTree *>> tmpProofs;
    std::vector<std::vector<ASTree *>> tmpProofs1;
    std::vector<std::vector<ASTree *>> tmpProofs2;
    for (auto i: pair1Indexes) {
        tmpProofs.emplace_back();
        deduction(subProofs[pairs1[i].first], getHypotheses(pairs1[i], false), getVarToDeduction(pairs1[i], false),
                  *(tmpProofs.end() - 1));

        tmpProofs.emplace_back();
        deduction(subProofs[pairs1[i].second], getHypotheses(pairs1[i], true), getVarToDeduction(pairs1[i], true),
                  *(tmpProofs.end() - 1));

        tmpProofs1.emplace_back(tmpProofs[0]);
        (tmpProofs1.end() - 1)->reserve((tmpProofs1.end() - 1)->size() + tmpProofs[1].size());
        (tmpProofs1.end() - 1)->insert((tmpProofs1.end() - 1)->end(), tmpProofs[1].begin(), tmpProofs[1].end());

        ASTree *var = getVarToDeduction(pairs1[i], false);
        var = var->data.getId() == "!" ? var->left : var;
        gluing(tree, var, *(tmpProofs1.end() - 1));
        tmpProofs.clear();
    }

    if (pair2Indexes.empty()) {
        proof = tmpProofs1[0];
    } else {
        int pairNumber = 0, o;
        for (int i = 0; i < tmpProofs1.size(); i += 2) {
            tmpProofs.emplace_back();
            ASTree *var1 = getVarToDeduction2(pairs2[pair2Indexes[pairNumber]], false, o);
            deduction(tmpProofs1[i], getHypotheses2(pairs2[pair2Indexes[pairNumber]], false, o),
                      var1, *(tmpProofs.end() - 1));

            tmpProofs.emplace_back();
            ASTree *var2 = getVarToDeduction2(pairs2[pair2Indexes[pairNumber]], true, o);
            deduction(tmpProofs1[i + 1], getHypotheses2(pairs2[pair2Indexes[pairNumber]], true, o),
                      var2, *(tmpProofs.end() - 1));


            tmpProofs2.emplace_back(tmpProofs[0]);
            (tmpProofs2.end() - 1)->reserve((tmpProofs2.end() - 1)->size() + tmpProofs[1].size());
            (tmpProofs2.end() - 1)->insert((tmpProofs2.end() - 1)->end(), tmpProofs[1].begin(), tmpProofs[1].end());

            ASTree *var = getVarToDeduction2(pairs2[pair2Indexes[pairNumber]], false, o);
            var = var->data.getId() == "!" ? var->left : var;
            gluing(tree, var, *(tmpProofs2.end() - 1));
            tmpProofs.clear();
            ++pairNumber;
        }
        if (pair3Indexes.empty()) {
            proof = tmpProofs2[0];
        } else {
            tmpProofs.emplace_back();
            ASTree *var1 = getVarToDeduction2(pairs3[pair3Indexes[0]], false, o);
            deduction(tmpProofs2[0], getHypotheses2(pairs2[pair3Indexes[0]], false, o),
                      var1, proof);

            tmpProofs.emplace_back();
            ASTree *var2 = getVarToDeduction2(pairs3[pair3Indexes[0]], true, o);
            deduction(tmpProofs2[1], getHypotheses2(pairs3[pair3Indexes[0]], true, o),
                      var2, proof);


            ASTree *var = getVarToDeduction2(pairs3[pair3Indexes[0]], false, o);
            var = var->data.getId() == "!" ? var->left : var;
            gluing(tree, var, proof);
            tmpProofs.clear();
        }
    }
}

void generateProof(ASTree *tree, const std::vector<bool> &values, std::vector<ASTree *> &proof, bool mode, int coef) {
    int index = 0;
    std::vector<std::vector<ASTree *>> subProofs;
    std::vector<int> argumentsIndex;
    for (auto i: values) {
        if (i == mode) {
            argumentsIndex.push_back(index);
            subProofs.emplace_back();
            generateSubProof(tree, *(subProofs.end() - 1), index);
        }
        index += coef;
    }
    minimize(tree, proof, subProofs, argumentsIndex, mode);
    std::cout << outStr << "\n";
    for (auto i: proof) {
        std::cout << toStringInfix(i) << '\n';
    }
}

void printAns(ASTree *tree) {
    std::vector<bool> values;
    std::vector<ASTree *> proof;
    values.reserve(8);
    int coef;
    std::vector<std::vector<bool>> tmpHypotheses;
    if (variables.size() == 1) {
        coef = 6;
        for (int i = 0; i < 8; i += coef) {
            values.push_back(eval(tree, i));
            tmpHypotheses.push_back({logicPermutations[i][0]});
        }

    } else if (variables.size() == 2) {
        coef = 2;
        for (int i = 0; i < 8; i += coef) {
            values.push_back(eval(tree, i));
            tmpHypotheses.push_back({logicPermutations[i][0], logicPermutations[i][1]});
        }
    } else {
        coef = 1;
        for (int i = 0; i < 8; ++i) {
            values.push_back(eval(tree, i));
            tmpHypotheses.push_back({logicPermutations[i][0], logicPermutations[i][1], logicPermutations[i][2]});
        }
    }
    bool mode = false;//false-with!, true - without!
    if ((variables.size() == 1 && (values[0] && !values[1])) ||
        (variables.size() == 2 && values[0] && !values[3]) ||
        (variables.size() == 3 && values[0] && !values[7])) {
        std::cout << ":(";
        return;
    }
    if ((variables.size() == 1 && values[1]) ||
        (variables.size() == 2 && values[3]) ||
        (variables.size() == 3 && values[7])) {
        mode = true;
    }
    else {
        tree = new ASTree(Negation(), tree, nullptr);
    }
    int index = 0;
    for (const auto &i: tmpHypotheses) {
        if (values[index] == mode) {
            hypotheses.push_back(i);
        }
        ++index;
    }
    generateProof(tree, values, proof, mode, coef);
    for (auto &i : proof) {
        std::cout << toStringInfix(i) << std::endl;
    }
}

void fillVectors() {
    logicPermutations.push_back({false, false, false});
    logicPermutations.push_back({false, false, true});
    logicPermutations.push_back({false, true, false});
    logicPermutations.push_back({false, true, true});
    logicPermutations.push_back({true, false, false});
    logicPermutations.push_back({true, false, true});
    logicPermutations.push_back({true, true, false});
    logicPermutations.push_back({true, true, true});
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
//    freopen("test", "r", stdin);
//    freopen("test.out", "w", stdout);
    fillVectors();
    std::string str;
    std::getline(std::cin, str);
    ASTree *tree = parse(str, false, &variables);
    printAns(tree);
    return 0;
}