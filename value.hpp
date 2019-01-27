#ifndef VALUE_H_
#define VALUE_H_

#include "base.hpp"
#include <iostream>

struct Integer : Value {
        explicit Integer(int v_) : v(v_) {}
        int v;
        Value *apply_IF(Exp *e1, Exp *e2, Env &rho, Env &ksi) override;
        Value *apply_MINUS_1(Value *e)                        override;
        Value *apply_MINUS_2(int from)                        override;

        void print(std::ostream &os) const override;
};
struct List : Value {
        List() = default;
        explicit List(std::vector<Exp*> _elements) : elements(_elements) {}
        
        std::vector<Exp*> elements;
        Value *apply_CONS(Value *head);
        Value *apply_CAR(Env &rho, Env &ksi);
        Value *apply_CDR();
        Value *apply_IS_NIL();

        void print(std::ostream &os) const;
};


namespace Fun {

struct Fun : Value {void print(std::ostream &os) const override{ os << "<Function>"; } };

struct If     : Fun { Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi); };
struct Minus  : Fun { Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi); };
struct Cons   : Fun { Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi); };
struct Car    : Fun { Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi); };
struct Cdr    : Fun { Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi); };
struct Is_nil : Fun { Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi); };
struct Custom : Fun {
        Custom(std::vector<Name> _arg_names, Exp *_body) : arg_names(_arg_names), body(_body) {}
        Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) override;
        std::vector<Name> arg_names;
        Exp *body;
        void print(std::ostream &os) const override{ os << "<UsrFunction>"; }
};

} // namespace Fun

#endif /* VALUE_H_ */
