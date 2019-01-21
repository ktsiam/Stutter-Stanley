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
        ksi["nil"]  = new List({});
        
        std::string line;
        std::cout << ">> ";
        while(std::getline(std::cin, line, ';')) {
                std::cout << 
                        *parse(preprocess(line))
                            -> make_exp()
                            -> val_of(rho, ksi)
                          << "\n>> ";
        }
}
