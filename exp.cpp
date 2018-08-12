#include "exp.hpp"
#include <iostream>
// EXPRESSIONS

Literal::Literal(Val v) : value(v) {}
Val Literal::val_of(Env&, Env&, FunEnv&) { return value; }

Variable::Variable(Name n) : name(n) {}
Val Variable::val_of(Env &rho, Env &ksi, FunEnv &)
{
        if (rho.find(name) != rho.end()) return rho[name];
        if (ksi.find(name) != ksi.end()) return ksi[name];
        assert(false && "Variable not found");
}

FunctionApp::FunctionApp(Name n, std::vector<Exp*> _args) : name(n), args(_args) {}
Val FunctionApp::val_of(Env&, Env &ksi, FunEnv &phi)
{
        if (phi.find(name) == phi.end()) std::cerr << "[" << name <<"]" << std::endl;
        assert(phi.find(name) != phi.end() && "Function not found");

        Env rho;
        std::vector<Val> arg_vals;
        for (Exp *arg : args)
                arg_vals.push_back(arg -> val_of(rho, ksi, phi));
        return phi[name] -> apply(arg_vals, ksi, phi);
}

VarDef::VarDef(Name n, Exp *e) : name(n), exp(e) {}
Val VarDef::val_of(Env &rho, Env &ksi, FunEnv &phi)
{
        return ksi[name] = exp -> val_of(rho, ksi, phi);
}

// FUNCTIONS
Primitive::Primitive(Fun::Prim_type _type) : prim_type(_type) {}
Val Primitive::apply(std::vector<Val> args, Env&, FunEnv&)
{
        assert(args.size() == 2);
        switch(prim_type) {
                case PLUS  : return args[0] + args[1];
                case MINUS : return args[0] - args[1];
        }
}

Val DefinedFun::apply(std::vector<Val> args, Env &ksi, FunEnv &phi)
{
        assert(args.size() == arg_names.size());

        for (std::size_t i = 0; i < arg_names.size(); ++i)
                rho[arg_names[i]] = args[i];

        Env rho;
        return body -> val_of(rho, ksi, phi);
}


