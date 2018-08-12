#ifndef EXP_H_
#define EXP_H_

#include <vector>
#include <string>
#include <map>

/* IDEAS 
 * shared_ptr for fun and var pointers
 * ρ, ξ, φ
 * Switch to lazy evaluation
 * Add pointer lookup (not name)
 */

struct Fun;

typedef int                  Val;
typedef std::string          Name;
typedef std::map<Name, Val>  Env;
typedef std::map<Name, Fun*> FunEnv;


/* LOCAL SCOPE (exps) */
struct Exp {
        virtual Val val_of(Env &rho, Env &ksi, FunEnv &phi) = 0;
};

struct Literal : Exp {
        Literal(Val v);
        Val value;
        Val val_of(Env&, Env&, FunEnv&);
};

struct Variable : Exp {
        Variable(Name n);
        Name name;
        Val val_of(Env &rho, Env &ksi, FunEnv&);
};

struct FunctionApp : Exp {
        FunctionApp(Name n, std::vector<Exp*> _args);
        Name name;
        std::vector<Exp*> args;
        Val val_of(Env&, Env&, FunEnv &phi);
};

/* not strictly an expression */
struct VarDef : Exp {
        VarDef(Name n, Exp *e);
        Name name;
        Exp *exp;
        Val val_of(Env &rho, Env &ksi, FunEnv &phi);
};


/* GLOBAL SCOPE (defs) */
struct Fun {
        virtual Val apply(std::vector<Val> args, Env &ksi, FunEnv &phi) = 0;
        enum Prim_type { PLUS, MINUS };
};

struct Primitive : Fun {
        Primitive(Fun::Prim_type _type);
        Fun::Prim_type prim_type;
        Val apply(std::vector<Val> args, Env &ksi, FunEnv &phi);
};

struct DefinedFun : Fun {
        std::vector<Name> arg_names;
        Env rho; // captured vars
        Exp *body;
        Val apply(std::vector<Val> args, Env &ksi, FunEnv &phi);
};


#endif /* EXP_H_ */
