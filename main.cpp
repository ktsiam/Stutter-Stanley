#include "parser.hpp"
int main()
{
        Env ksi, rho;
        FunEnv phi;
        
        phi["+"]   = new Primitive(Fun::PLUS);
        phi["-"]   = new Primitive(Fun::MINUS);
        
        std::string line;
        while(std::getline(std::cin, line, ';')) {
                parse(preprocess(line)) -> print();
                std::cout << " = " <<  parse(preprocess(line)) ->
                        make_exp() -> val_of(rho, ksi, phi);
                std::cout << std::endl;
        }
}
