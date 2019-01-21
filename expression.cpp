#include "expression.hpp"
#include "value.hpp"

Literal::Literal(Value *v) : value(v) {}
Value *Literal::val_of(Env&, Env&) { return value; }

Variable::Variable(Name n) : name(n) {}
Value *Variable::val_of(Env &rho, Env &ksi)
{
        if (rho.find(name) != rho.end()) return rho[name];
        if (ksi.find(name) != ksi.end()) return ksi[name];
        assert(false && "Variable not found");
}

FunctionApp::FunctionApp(Exp *_funExp, std::vector<Exp*> _args) : funExp(_funExp), args(_args) {}
Value *FunctionApp::val_of(Env &rho, Env &ksi)
{
        Value *fun = funExp -> val_of(rho, ksi);
        return fun -> apply_self(args, rho, ksi);        
}

VarDef::VarDef(Name n, Exp *e) : name(n), exp(e) {}
Value *VarDef::val_of(Env &rho, Env &ksi)
{
        return ksi[name] = exp -> val_of(rho, ksi);
}

FunDef::FunDef(Name n, std::vector<Name> _arg_names, Exp *_body) :
        funName(n), arg_names(_arg_names), body(_body) {}

Value *FunDef::val_of(Env &, Env &ksi)
{
        return ksi[funName] = new Fun::Custom(arg_names, body);
}
