#include "parser.hpp"
int main()
{
        Env ksi, rho;        
        ksi["if"]   = new Fun::If;
        ksi["-"]    = new Fun::Minus;
        ksi["cons"] = new Fun::Cons;
        ksi["car"]  = new Fun::Car;
        ksi["cdr"]  = new Fun::Cdr;
        ksi["nil?"] = new Fun::Is_nil;
        ksi["nil"]  = new List(std::vector<Exp*>());

        
        
        // FunctionApp app(new Variable("-"), { new Literal(new Integer(2)),
        //                                      new Literal(new Integer(1)) });
        //  std::cout << *app.val_of(rho, ksi) << std::endl;


        
        std::string line;
        std::cout << ">> ";
        while(std::getline(std::cin, line, ';')) {
                std::istringstream ready = Token::preprocess(line);
                Token *tok = Token::parse(ready);
                assert(tok);
                std::cout << * tok -> make_exp() -> val_of(rho, ksi) << "\n>> ";
        }
}
