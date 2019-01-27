#ifndef BASE_H_
#define BASE_H_

#include <vector>
#include <string>
#include <map>
#include <iostream>

struct Value;
typedef std::string Name;
typedef std::map<Name, Value*>  Env;

struct Exp {
        virtual Value *val_of(Env &rho, Env &ksi) = 0;

        /* Print */
        virtual void print(std::ostream &os) const { os << "<exp>";} // TODO
        friend std::ostream &operator<<(std::ostream &os, const Exp &v);
};

struct Value {
        virtual Value *apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi);

        /* Primitive function application */
        virtual Value *apply_IF(Exp *e1, Exp *e2, Env &rho, Env &ksi);
        virtual Value *apply_MINUS_1(Value *e);
        virtual Value *apply_MINUS_2(int from);
        virtual Value *apply_CONS(Value *head);
        virtual Value *apply_CAR(Env &rho, Env &ksi);
        virtual Value *apply_CDR();
        virtual Value *apply_IS_NIL();

        /* Print */
        virtual void print(std::ostream &os) const = 0;
        friend std::ostream &operator<<(std::ostream &os, const Value &e);

};


#endif // BASE_H_
