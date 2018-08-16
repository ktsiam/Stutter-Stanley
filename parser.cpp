#include "parser.hpp"

Token::Token(std::string _head) : head(_head) {}

UnitTok::UnitTok(std::string _head) : Token(_head) {}

Exp *UnitTok::make_exp()
{
        Val val = str_to_val(head);
        if (val != -1)
                return new Literal(val);
        return new Variable(head);
}

MultiTok::MultiTok(std::string _head, std::vector<Token*> _args) :
        Token(_head), args(_args) {}

Exp *MultiTok::make_exp()
{
        if (head == "val") {
                assert(args.size() == 2);
                return new VarDef(args[0] -> head, args[1] -> make_exp());
        }
        if (head == "def") {
                assert(args.size() == 3);

                // name
                Name funName = args[0] -> head;

                // argument names
                MultiTok *funDef = dynamic_cast<MultiTok*>(args[1]);
                assert(funDef && "invalid argument list");

                // body
                Exp *body = args[2] -> make_exp();

                if (funDef -> head == "void")
                        return new FunDef(funName, {}, body);
                
                std::vector<Name> arg_names = { funDef -> head };
                for (Token *arg : funDef -> args)
                        arg_names.push_back(arg -> head);

                
                
                return new FunDef(funName, arg_names, body);
        }
        
        std::vector<Exp*> params;
        for (Token *arg : args) 
                params.push_back(arg -> make_exp());

        return new FunctionApp(head, params);
}



/* PARSING FUNCTIONS */


Val str_to_val(const std::string &s)
{        
        char *bad;
        Val v = static_cast<Val>(std::strtol(s.c_str(), &bad, 10));
        return *bad ? -1 : v;
}

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

/* Parses expression from given stream */
Token *parse_composite(std::istream &is)
{
        std::string head, tok;
        is >> head;
        
        std::vector<Token*> args;
        while (is >> tok) {
                if (tok == ")") // exit condition
                        return new MultiTok(head, args);

                if (tok == "(") // recurse
                        args.push_back(parse_composite(is));

                else args.push_back(new UnitTok(tok));
        }
        assert(false && "mismatched parentheses");
}

/* Wrapper of parse_composite */
Token *parse(std::istringstream &&is)
{
        std::string tok;
        is >> tok;

        assert(tok != ")" && "seriously?");
        return (tok == "(") ? parse_composite(is) : new UnitTok(tok);
}


