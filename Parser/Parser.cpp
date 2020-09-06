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
        case '+':
            return Plus();
        case '*':
            return Multiplication();
        case '=':
            return Equals();
        case '\'':
            return Hatch();
        default:
            return Variable("this var will be ignored");
    }
}

ASTree *parse(std::string &expression, bool mode, std::vector<std::string> *variables) {
    std::stack<Expression> operators;
    std::stack<ASTree *> result;
    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];
        if (ch == '&' || ch == '|' ||
            ch == '>' || ch == '!' ||
            ch == '+' || ch == '*' ||
            ch == '=' || (ch == '\'' && mode)) {
            Expression expr = getOperator(ch);
            peekOperations(operators, result, expr.getPriority());
            operators.push(expr);
        } else if (ch == '@' || ch == '?') {
            std::string quantifierStr;
            char tmp = ch;
            while (ch != '.') {
                quantifierStr += ch;
                ch = expression[++i];
            }

            Expression quantifier = (tmp == '@') ? static_cast<Expression>(ForEach(quantifierStr))
                                                 : static_cast<Expression>(Exists(quantifierStr));
            peekOperations(operators, result, quantifier.getPriority());
            operators.push(quantifier);
        } else if (ch == '(') {
            operators.push(OpenBracket());
        } else if (ch == ')') {
            peekBracket(operators, result);
        } else if ((isalpha(ch) || isdigit(ch) || ch == '\'') && !mode) {
            std::string varName;
            while (ch == '\'' || isalpha(ch) || isdigit(ch)) {
                varName += ch;
                ch = expression[++i];
            }
            --i;
            Variable var(varName);
            if (variables != nullptr) {
                if (variables->size() != 3 &&
                    std::find(variables->begin(), variables->end(), varName) == variables->end())
                    variables->push_back(varName);
            }
            addAloneNode(result, var);
        } else if ((isalpha(ch) || isdigit(ch)) && mode) {
            std::string name;
            Expression expr;
            if (isdigit(ch)) {
                expr = Zero();
            } else {
                while (isalpha(ch)) {
                    name += ch;
                    ch = expression[++i];
                }
                --i;
                if (std::all_of(name.begin(), name.end(), [](unsigned char c) { return isupper(c); })) {
                    expr = Predicate(name);
                } else {
                    expr = Variable(name);
                }
            }
            addAloneNode(result, expr);
        } else {
            continue;
        }
    }

    while (!operators.empty()) {
        if (operators.top().getId() != "!" && operators.top().getId() != "\'" &&
            operators.top().getId()[0] != '@' && operators.top().getId()[0] != '?') {
            addNode(result, operators.top());
        } else {
            addUnaryNode(result, operators.top());
        }
        operators.pop();
    }
    return result.top();
}