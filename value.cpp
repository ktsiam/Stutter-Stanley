#include "value.hpp"
#include "expression.hpp"

std::ostream &operator<<(std::ostream &os, const Value &v)
{
        v.print(os);
        return os;
}

/* Value */
Value *Value::apply_self(std::vector<Exp*> &, Env &, Env &) { throw std::runtime_error("Cannot apply non-function!"); }
Value *Value::apply_IF(Exp *, Exp *, Env &, Env &)          { throw std::runtime_error("1st `if` argument must be of type int!"); }
Value *Value::apply_MINUS_1(Value *)                        { throw std::runtime_error("1st argument of `-` must be of type int!"); }
Value *Value::apply_MINUS_2(int)                            { throw std::runtime_error("2nd argument of `-` must be of type int!"); }
Value *Value::apply_CONS(Value *)                           { throw std::runtime_error("2nd argument of `cons` must be of list type!"); }
Value *Value::apply_CAR(Env &, Env &)                       { throw std::runtime_error("argument of `car` must be of list type!"); }
Value *Value::apply_CDR()                                   { throw std::runtime_error("argument of `cdr` must be of list type!"); }
Value *Value::apply_IS_NIL()                                { return new Integer(0); }

/* Integer */
Value *Integer::apply_IF(Exp *e1, Exp *e2, Env &rho, Env &ksi) {
        return (v ? e1 : e2) -> val_of(rho, ksi);
}
Value *Integer::apply_MINUS_1(Value *e) {
        return e -> apply_MINUS_2(v);
}
Value *Integer::apply_MINUS_2(int from) {
        return new Integer(from - v);
}

void Integer::print(std::ostream &os) const {
        os << v;
}

/* List */
Value *List::apply_CONS(Value *head) {        
        List *cons = new List;
        cons -> elements = elements;
        cons -> elements.push_back(new Literal(head));
        return cons;
}
Value *List::apply_CAR(Env &rho, Env &ksi) {
        return elements.back() -> val_of(rho, ksi);
}
Value *List::apply_CDR() {
        List *cdr = new List;
        cdr -> elements = elements;
        cdr -> elements.pop_back();
        return cdr;
}
Value *List::apply_IS_NIL() {
        return new Integer(elements.empty());
}

void List::print(std::ostream &os) const {
        std::string sep = "";
        os << "'(";
        
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
                os << sep << **it;
                sep = ", ";
        }
        os << ')';
}

/* Functions */

Value *Fun::If::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() != 3) {
                std::ostringstream os;
                os << "`if` expected 3 arguments, but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }
        
        Value *fst = args[0] -> val_of(rho, ksi);
        
        return fst -> apply_IF(args[1], args[2], rho, ksi);
}

Value *Fun::Minus::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() != 2) {
                std::ostringstream os;
                os << "`-` expected 2 arguments, but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }
        
        Value *fst = args[0] -> val_of(rho, ksi);
        Value *snd = args[1] -> val_of(rho, ksi);
        
        return fst -> apply_MINUS_1(snd);        
}

Value *Fun::Cons::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() != 2) {
                std::ostringstream os;
                os << "`cons` expected 2 arguments, but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }
        
        Value *x  = args[0] -> val_of(rho, ksi);
        Value *xs = args[1] -> val_of(rho, ksi);
        
        return xs -> apply_CONS(x);
}

Value *Fun::Car::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() != 1) {
                std::ostringstream os;
                os << "`car` expected 1 argument, but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }

        Value *list = args[0] -> val_of(rho, ksi);
        return list -> apply_CAR(rho, ksi);
}

Value *Fun::Cdr::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() != 1) {
                std::ostringstream os;
                os << "`cdr` expected 1 argument, but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }
        
        Value *list = args[0] -> val_of(rho, ksi);
        return list -> apply_CDR();
}

Value *Fun::Is_nil::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() != 1) {
                std::ostringstream os;
                os << "`nil?` expected 1 argument, but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }

        Value *list = args[0] -> val_of(rho, ksi);
        return list -> apply_IS_NIL();
}

Value *Fun::Custom::apply_self(std::vector<Exp*> &args, Env &rho, Env &ksi) {
        if (args.size() == arg_names.size()) {
                std::ostringstream os;
                os << "Function expected "     << arg_names.size()
                   << " argument(s), but got " << args.size() << '!';
                throw std::runtime_error(os.str());
        }

        Env new_rho;
        for (std::size_t i = 0; i < arg_names.size(); ++i)
                new_rho[arg_names[i]] = args[i] -> val_of(rho, ksi);

        return body -> val_of(new_rho, ksi);
}

