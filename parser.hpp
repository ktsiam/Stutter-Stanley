#ifndef PARSER_H_
#define PARSER_H_

#include "expression.hpp"
#include "value.hpp"
#include <iostream>
#include <sstream>

struct Token {
        virtual Exp  *make_exp() = 0;
        static Token *parse(std::istream&is);
        static std::istringstream preprocess(const std::string &inp);
        
};

namespace Tok {

/* variable or literal */
struct Unit : Token {
        explicit Unit(std::istream &is);
        explicit Unit(std::string  &&s) : id(s) {}
        Exp *make_exp();

        std::string id;
};

struct Definition : Token {
        explicit Definition(std::istream &is);
        Exp *make_exp();

        std::string id;
        Token *val;
};

struct Application : Token {
        explicit Application(std::istream &is);
        Application(std::string &&s, std::istream &is);
        Exp *make_exp();
        
        std::vector<Token*> operands;
};

struct Lambda : Token {
        explicit Lambda(std::istream &is);
        Exp *make_exp();
        std::vector<std::string> args;
        Token *body;
};

struct ListTok : Token {
        explicit ListTok(std::istream &is);
        Exp *make_exp();
        std::vector<Token*> elements;
};

} // namespace Tok


#endif /* PARSER_H_ */
