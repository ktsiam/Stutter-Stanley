#ifndef PARSER_H_
#define PARSER_H_

#include "expression.hpp"
#include <iostream>
#include <sstream>

struct Token {
        Token(std::string _head);
        std::string head;

        virtual Exp *make_exp() = 0;
};

struct UnitTok : Token { 
        UnitTok(std::string _head);        
        Exp *make_exp();
};

struct FunAppTok : Token {
        FunAppTok(std::string _head, std::vector<Token*> _args);
        Exp *make_exp();
        
        std::vector<Token*> args;
};

struct FunDefTok : Token {
        std::vector<std::string> arg_names;
        
        
};

/* Converts string to Val if possible (else -1)*/
Val str_to_val(const std::string &s);
std::istringstream preprocess(std::string inp); // adds extra spaces
Token *parse_composite(std::istream &is); // parses recursively
Token *parse(std::istringstream &&is);    // wrapper


#endif /* PARSER_H_ */
