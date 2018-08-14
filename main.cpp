#include "parser.hpp"
int main()
{
        Env ksi, rho;
        FunEnv phi;
        
        phi["+"]  = new Primitive(Fun::PLUS);
        phi["-"]  = new Primitive(Fun::MINUS);
        phi["if"] = new Primitive(Fun::IF);
        
        std::string line;
        std::cout << ">> ";
        while(std::getline(std::cin, line, ';')) {
                std::cout << 
                        parse(preprocess(line))
                            -> make_exp()
                            -> val_of(rho, ksi, phi)
                          << "\n>> ";
        }
}
