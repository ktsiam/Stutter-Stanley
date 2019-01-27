#ifndef EXP_H_
#define EXP_H_

#include "base.hpp"


struct Literal : Exp {
        Literal(Value *v);
        Value *value;
        Value *val_of(Env &rho, Env &ksi);
};

struct Variable : Exp {
        Variable(Name n);
        Name name;
        Value *val_of(Env &rho, Env &ksi);
};

struct FunctionApp : Exp {
        FunctionApp(Exp *_funExp, std::vector<Exp*> _args);
        Exp *funExp;
        std::vector<Exp*> args;
        Value *val_of(Env &rho, Env &ksi);
};

/* not strictly an expression */
struct VarDef : Exp {
        VarDef(Name n, Exp *e);
        Name name;
        Exp *exp;
        Value *val_of(Env &rho, Env &ksi);
};

struct FunDef : Exp {
        FunDef(Name n, std::vector<Name> _arg_names, Exp *_body);
        Name funName;
        std::vector<Name> arg_names;
        Exp *body;
        Value *val_of(Env &rho, Env &ksi);
};

#endif /* EXP_H_ */
