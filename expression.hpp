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
        Val val_of(Env &rho, Env &ksi, FunEnv &phi);
};

struct Variable : Exp {
        Variable(Name n);
        Name name;
        Val val_of(Env &rho, Env &ksi, FunEnv &phi);
};

struct FunctionApp : Exp {
        FunctionApp(Name n, std::vector<Exp*> _args);
        Name name;
        std::vector<Exp*> args;
        Val val_of(Env &rho, Env &ksi, FunEnv &phi);
};

/* not strictly an expression */
struct VarDef : Exp {
        VarDef(Name n, Exp *e);
        Name name;
        Exp *exp;
        Val val_of(Env &rho, Env &ksi, FunEnv &phi);
};

struct FunDef : Exp {
        FunDef(Name n, std::vector<Name> _arg_names, Exp *_body);
        Name funName;
        std::vector<Name> arg_names;
        Exp *body;
        Val val_of(Env &rho, Env &ksi, FunEnv &phi);
};

/* GLOBAL SCOPE (defs) */
struct Fun {
        virtual Val apply(std::vector<Exp*> args, Env &rho, Env &ksi, FunEnv &phi) = 0;
        enum Prim_type { PLUS, MINUS, IF, BEGIN };
};

struct Primitive : Fun {
        Primitive(Fun::Prim_type _type);
        Fun::Prim_type prim_type;
        Val apply(std::vector<Exp*> args, Env &rho, Env &ksi, FunEnv &phi);
};

struct DefinedFun : Fun {
        DefinedFun(std::vector<Name> _arg_names, Exp *_body);
        std::vector<Name> arg_names;
        //Env rho; // captured vars -- weak bindings
        Exp *body;
        Val apply(std::vector<Exp*> args, Env &rho, Env &ksi, FunEnv &phi);
};


#endif /* EXP_H_ */
