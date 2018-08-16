#include "expression.hpp"
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
Val FunctionApp::val_of(Env &rho, Env &ksi, FunEnv &phi)
{
        assert(phi.find(name) != phi.end() && "Function not found");

        return phi[name] -> apply(args, rho, ksi, phi);
}

VarDef::VarDef(Name n, Exp *e) : name(n), exp(e) {}
Val VarDef::val_of(Env &rho, Env &ksi, FunEnv &phi)
{
        return ksi[name] = exp -> val_of(rho, ksi, phi);
}

FunDef::FunDef(Name n, std::vector<Name> _arg_names, Exp *_body) :
        funName(n), arg_names(_arg_names), body(_body) {}

Val FunDef::val_of(Env &, Env &, FunEnv &phi)
{
        phi[funName] = new DefinedFun(arg_names, body);
        return 0;
}

// FUNCTIONS
Primitive::Primitive(Fun::Prim_type _type) : prim_type(_type) {}
Val Primitive::apply(std::vector<Exp*> args, Env &rho, Env &ksi, FunEnv &phi)
{
        switch(prim_type) {
                case PLUS  : assert(args.size() == 2);
                        return args[0] -> val_of(rho, ksi, phi) +
                               args[1] -> val_of(rho, ksi, phi);
                case MINUS : assert(args.size() == 2);
                        return args[0] -> val_of(rho, ksi, phi) -
                               args[1] -> val_of(rho, ksi, phi);
                case IF : assert(args.size() == 3);
                        return (args[0] -> val_of(rho, ksi, phi)) ?
                                args[1] -> val_of(rho, ksi, phi) :
                                args[2] -> val_of(rho, ksi, phi);
                case BEGIN : Val v = 0;
                        for (Exp *arg : args)
                                v = arg -> val_of(rho, ksi, phi);
                        return v;
        }
}

DefinedFun::DefinedFun(std::vector<Name> _arg_names, Exp *_body) :
        arg_names(_arg_names), body(_body) {}

#include <iostream>
Val DefinedFun::apply(std::vector<Exp*> args, Env &rho, Env &ksi, FunEnv &phi)
{
        assert(args.size() >= arg_names.size());

        Env new_rho;
        for (std::size_t i = 0; i < arg_names.size(); ++i)
                new_rho[arg_names[i]] = args[i] -> val_of(rho, ksi, phi);

        return body -> val_of(new_rho, ksi, phi);
}


