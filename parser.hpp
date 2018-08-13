#ifndef PARSER_H_
#define PARSER_H_

#include "exp.hpp"
#include <iostream>
#include <sstream>

Val mk_num(const std::string &s)
{        
        char *bad;
        Val v = static_cast<Val>(std::strtol(s.c_str(), &bad, 10));
        return *bad ? -1 : v;
}
struct Token {
        virtual Exp *make_exp() = 0;
        virtual operator std::string() = 0;
        virtual void print()    = 0;
};

struct Unit : Token {
        Unit(std::string _unit) : unit(_unit) {}
        Exp *make_exp() {
                Val val = mk_num(unit);
                if (val != -1)
                        return new Literal(val);
                return new Variable(unit);
        }
        
        std::string unit;

        operator std::string() { return unit; }
        void print() { std::cout << "Unit<" << unit << ">"; }
};

/* Add reference_wrapper !! */
struct Composite : Token {
        Composite(std::string _head, std::vector<Token*> _args) :
                head(_head), args(_args) {}
        Exp *make_exp() {
                if (head == "val")
                        return new VarDef(*args[0], args[1] -> make_exp());
                std::vector<Exp*> params;
                for (Token *arg : args) 
                        params.push_back(arg -> make_exp());

                return new FunctionApp(head, params);
        }
        
        std::string head;
        std::vector<Token*> args;
        
        operator std::string() { return head; }
        void print() {
                std::cout << "Comp<('" << head << "'";
                for (auto &&sub_t : args) { std::cout << ' '; sub_t -> print(); }
                std::cout << ")>";
        }
};

/* PREPROCESSOR ADDS SPACES BETWEEN (, ), abc */

std::istringstream preprocess(std::string inp)
{
        std::string ready;
        
        for (char c : inp) {
                switch(c) {
                        case '(' : case ')' :
                                ready.push_back(' ');
                                ready.push_back(c);
                                ready.push_back(' ');
                                break;
                        default : ready.push_back(c);
                }
        }
        return std::istringstream(ready);
}

Token *parse_composite(std::istream &is)
{
        std::string head, tok;
        is >> head;
        
        std::vector<Token*> args;
        while (is >> tok) {
                if (tok == ")") // exit condition
                        return new Composite(head, args);

                if (tok == "(") // recurse
                        args.push_back(parse_composite(is));

                else args.push_back(new Unit(tok));
        }
        assert(false && "mismatched parentheses");
}

Token *parse(std::istringstream &&is)
{
        std::string tok;
        is >> tok;

        assert(tok != ")" && "seriously?");
        return (tok == "(") ? parse_composite(is) : new Unit(tok);
}

#endif /* PARSER_H_ */
