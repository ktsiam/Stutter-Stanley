#include "parser.hpp"
#include <algorithm>
#include <string>

static void validate_id(const std::string &s) {
        if (s != "(" && s != ")" && s != "'" && s != "") return;
        
        throw std::runtime_error((std::ostringstream() << "Syntax error : " << s) .str());
}

/* PREPROCESSOR ADDS SPACES BETWEEN PARENTHESES AND REMOVES COMMENTS */
std::istringstream Token::preprocess(const std::string &inp)
{
        std::string ready;
        bool comment = false;
        
        for (char c : inp) {
                switch(c) {
                        case '\n': ready.push_back(' ');
                                comment = false;
                                break;
                        case '#' : comment = true;
                                break;
                        case '(' : case ')' : case '\'' :
                                if (comment) break;
                                ready.push_back(' ');
                                ready.push_back(c);
                                ready.push_back(' ');
                                break;
                         default : if (comment) break;
                                ready.push_back(c);
                }
        }
        return std::istringstream(ready);
}

/* responsibility of constructor to remove ')' */
Token *Token::parse(std::istream &is) {
        std::string s;
        is >> s;

        if (s == ")") return nullptr;        
        if (s == "'") return new Tok::ListTok(is);
        if (s != "(") return new Tok::Unit(std::move(s));


        /* Extracting first non-space character */
        is >> std::noskipws;
        char c = is.peek();
        while (std::isspace(c)) {
                is >> c;
                c = is.peek();
        }
        is >> std::skipws;

        switch (c) {
        case '\'' : throw std::runtime_error("Cannot apply expression of list type!");
        case ')'  : throw std::runtime_error("Empty application is invalid!");
        case '('  :
                return new Tok::Application(is); break;
        default:
                is >> s;
                if (s == "val" || s == "def")    return new Tok::Definition(is);
                if (s == "fun" || s == "lambda") return new Tok::Lambda(is);

                return new Tok::Application(std::move(s), is);
        }
}


Tok::Unit::Unit(std::istream &is) {
        is >> id;
        validate_id(id);
}


Tok::Definition::Definition(std::istream &is) {
        is >> id;
        validate_id(id);
        
        val = Token::parse(is);

        // closing parenthesis
        char c;;
        is >> c;
        if (c != ')')
                throw std::runtime_error("Definitions (`val`) must contain "
                                         "only a name and an expression!");
}

Tok::Application::Application(std::string &&fst, std::istream &is) {
        operands.push_back(new Tok::Unit(std::move(fst)));
        
        Token *tok;        
        while ((tok = parse(is)))
                operands.push_back(tok);
} 

Tok::Application::Application(std::istream &is) {
        Token *tok;        
        while ((tok = parse(is)))
                operands.push_back(tok);
}

Tok::Lambda::Lambda(std::istream &is) {
        std::string id;
        is >> id;
        if (id != "(")
                throw std::runtime_error("`fun` should be followed by an "
                                         "argument list! -- (fun (a b ...) body)");

        while (is >> id && id != ")") {
                validate_id(id);
                args.push_back(std::move(id));
        }

        body = Token::parse(is);

        // closing parenthesis
        char c;
        is >> c;
}

Tok::ListTok::ListTok(std::istream &is) {
        std::string s;
        is >> s;
        if (s != "(")
                throw std::runtime_error("tick (`) must be followed by a parenthesized "
                                         "list expression! -- '(a b ...)");

        Token *elem;
        while ((elem = Token::parse(is)))
               elements.push_back(elem);
}


Exp *Tok::Unit::make_exp() {
        if (std::all_of(id.begin(), id.end(), [](char c) { return std::isdigit(c); })
            && !id.empty())
                return new Literal(new Integer(std::stoi(id)));
        return new Variable(id);
}

Exp *Tok::Definition::make_exp() {
        return new VarDef(id, val -> make_exp());
}

Exp *Tok::Application::make_exp() {

        assert(operands.size() > 0 && "This should not happen!");
        Exp *funExp = operands[0] -> make_exp();
        
        std::vector<Exp*> args;
        std::transform(operands.begin()+1, operands.end(),
             std::back_inserter(args), [](Token *t) { return t -> make_exp(); });

        return new FunctionApp(funExp, args);
}

Exp *Tok::Lambda::make_exp() {
        return new Literal(new Fun::Custom(args, body -> make_exp()));
}

Exp *Tok::ListTok::make_exp() {
        std::vector<Exp*> exps;

        for (auto it = elements.rbegin(); it != elements.rend(); ++it)
                exps.push_back((*it) -> make_exp());
                                                                                      
        return new Literal(new List(exps));
}



