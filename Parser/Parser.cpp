//
// Created by Nik Carlson on 17.07.2020.
//

#include "Parser.h"

Expression getOperator(char ch) {
    switch (ch) {
        case '&':
            return Conjunction();
        case '|':
            return Disjunction();
        case '!':
            return Negation();
        case '>':
            return Implication();
        default:
            return Variable("this var will be ignored");
    }
}

ASTree *parse(std::string &expression) {
    std::stack<Expression> operators;
    std::stack<ASTree *> result;
    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];
        if (ch == '&' || ch == '|' || ch == '>' || ch == '!') {
            Expression expr = getOperator(ch);
            peekOperations(operators, result, expr.getPriority());
            operators.push(expr);
        } else if (ch == '(') {
            operators.push(OpenBracket());
        } else if (ch == ')') {
            peekBracket(operators, result);
        } else if (isalpha(ch) || isdigit(ch) || ch == '\'') {
            std::string varName;
            while (ch == '\'' || isalpha(ch) || isdigit(ch)) {
                varName += ch;
                ch = expression[++i];
            }
            --i;
            Variable var(varName);
            addAloneNode(result, var);
            varName.clear();
        } else {
            continue;
        }
    }

    while (!operators.empty()) {
        if (operators.top().getId() != "!") {
            addNode(result, operators.top());
        } else {
            addNegNode(result, operators.top());
        }
        operators.pop();
    }
    return result.top();
}