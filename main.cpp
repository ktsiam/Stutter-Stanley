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

        
        std::string line;
        std::cout << ">> ";
        while(std::getline(std::cin, line, ';')) {
                try {
                        std::istringstream ready = Token::preprocess(line);
                        Token *tok = Token::parse(ready);
                        if (!tok) throw std::runtime_error("Mismatched parentheses");
                        std::cout << * tok -> make_exp() -> val_of(rho, ksi) << "\n>> ";
                } catch (const std::runtime_error &exc) {
                        std::cerr << exc.what() << std::endl;
                        std::cout << "\n>> ";
                }
        }
}
