#include "type2.hpp"

Type *operator^(Type &a, Type &b)
{
        Type *result = a.bind(b);
        a.unbind();
        b.unbind();
        return result; // be careful (copies forall pointers)
}


Type *Type::binds_with(Tp::Primitive &) { return NULL; }
Type *Type::binds_with(Tp::List      &) { return NULL; }
Type *Type::binds_with(Tp::Forall    &) { return NULL; }
Type *Type::binds_with(Tp::Function  &) { return NULL; }


using namespace Tp;

Type *Primitive::bind(Type &o)  { return o.binds_with(*this); }
Type *Function ::bind(Type &o)  { return o.binds_with(*this); }
Type *List     ::bind(Type &o)  { return o.binds_with(*this); }
Type *Forall   ::bind(Type &o)  { return o.binds_with(*this); }

void Primitive::unbind()  {  }
void List     ::unbind()  { elem_type -> unbind(); }
void Forall   ::unbind()  { *bound = NULL; }
void Function ::unbind()
{
        for (auto &&arg : arg_types) arg -> unbind();
        ret_type -> unbind();
}



/* Primitive */
Type *Primitive::binds_with(Tp::Primitive &o) 
{
        return (tag == o.tag) ? this : NULL;
}
Type *Primitive::binds_with(Tp::Forall   &o)
{
        return o.binds_with(*this);
}

/* Forall */
Type *Forall::binds_with(Tp::Primitive &o) 
{
        if (*bound)  return (*bound) -> binds_with(o);
        else         return  *bound = &o;
}

Type *Forall::binds_with(Tp::Function  &o) 
{
        if (*bound)  return (*bound) -> binds_with(o);
        else         return  *bound = &o;
}

Type *Forall::binds_with(Tp::List      &o) 
{
        if (*bound)  return (*bound) -> binds_with(o);
        else         return  *bound = &o;
}
Type *Forall::binds_with(Tp::Forall    &o) 
{
        if (*bound)   return (*bound) -> binds_with(o);
        if (*o.bound) return  *bound = *o.bound;
        return this;
}

/* List */
Type *List::binds_with(Tp::List &o) 
{
        Type *sub_type = elem_type -> bind(*o.elem_type);
        return sub_type ? new List(sub_type) : NULL;
}

Type *List::binds_with(Tp::Forall &o) 
{
        return o.binds_with(*this);
}

/* Function */
Type *Function::binds_with(Tp::Function &o) 
{
        std::size_t size = arg_types.size();
        if (o.arg_types.size() != size) return NULL;

        std::vector<Type*> args;
        for (std::size_t i = 0; i < size; ++i) {
                Type *arg = arg_types[i] -> bind(*o.arg_types[i]);
                if (!arg) return NULL;
                args.push_back(arg);
        }        

        Type *ret = ret_type -> bind(*o.ret_type);
        if (!ret) return NULL;

        return new Function(ret, args);
}
Type *Function::binds_with(Tp::Forall &o) 
{
        return o.binds_with(*this);
}

/* PRINTING */

std::ostream &operator<<(std::ostream &os, Type &t)
{
        std::map<int, char> bindings;
        t.print(os, bindings);
        return os;
}

void Primitive::print(std::ostream &os, std::map<int, char> &)
{
        std::string prims[] = { "int", "bool", "sym" };
        os << prims[tag];
}
       
void List::print(std::ostream &os, std::map<int, char> &bindings)
{
        elem_type -> print(os, bindings);
        os << " list";
}

void Function::print(std::ostream &os, std::map<int, char> &bindings)
{
        os << '(';
        std::string sep = "";
        for (Type *arg : arg_types) {
                os << sep;
                arg -> print(os, bindings);
                sep = " * ";
        }
        os << " -> ";
        ret_type -> print(os, bindings);
        os << ')';
}

void Forall::print(std::ostream &os, std::map<int, char> &bindings)
{
        os << '\'';
        if (bindings.find(id) != bindings.end()) {
                os << bindings[id];
                return;
        }
        char c = 'a' - 1;
        for (auto &&pair : bindings)
                if (bindings[pair.first] > c)
                        c = pair.second;
        bindings[id] = ++c;
        os << c;
}

#ifdef DEBUG
int main()
{
        std::cout << "SAME BIND TESTS\n";
        
        Primitive a = Tp::Primitive::INT;
        Primitive b = Tp::Primitive::INT;
        Primitive c = Tp::Primitive::SYMBOL;
        
        std::cout << a << " ^ " << b << " = " <<  *(a ^ b) << std::endl;
        std::cout << a << " ^ " << c << " = " <<   (a ^ c) << std::endl;

        List la(&a), lc(&c), lla(&la);

        std::cout << a   << " ^ " << la  << " = " <<   (a ^ la)    << std::endl;
        std::cout << la  << " ^ " << lla << " = " <<   (la ^ lla)  << std::endl;
        std::cout << lla << " ^ " << lla << " = " <<  *(lla ^ lla) << std::endl;

        Function fa(&a, {&a, &b, &c}), fb(&b, {&b, &a, &c}), fc(&a, {&b, &c, &c});
        std::cout << fa << " ^ " << fb << " = " <<  *(fa ^ fb) << std::endl;
        std::cout << fa << " ^ " << fc << " = " <<   (fa ^ fc) << std::endl;

        Forall e, f, g;
        std::cout << e << " ^ " << f << " = " <<  *(e ^ f) << std::endl;

        std::cout << "\nFORALL SINGLE BIND TESTS:\n";

        std::cout << a << " ^ " << e << " = " <<  *(a  ^ e) << std::endl;
        std::cout << f << " ^ " << b << " = " <<  *(f  ^ b) << std::endl;
        std::cout << la << " ^ " << e << " = " << *(la ^ e) << std::endl;

        List lf(&e);
        std::cout << la << " ^ " << lf << " = " << *(la ^ lf) << std::endl;
        std::cout << a  << " ^ " << lf << " = " <<  ( a ^ lf) << std::endl;

        Function fd(&e, {&a, &b}), fe(&a, {&e, &e}), ff(&e, {&e, &c}), fg(&c, {&a, &f});
        std::cout << fd  << " ^ " << fe << " = " <<  *(fd ^ fe) << std::endl;
        std::cout << ff  << " ^ " << fg << " = " <<   (ff ^ fg) << std::endl;
        
        Function cons(&e, {&e, &lf}), cons2(&c, {&a, &la}), cons3(&la, {&la, &lla});
        std::cout << cons  << " ^ " << cons2 << " = " <<   (cons ^ cons2) << std::endl;
        std::cout << cons  << " ^ " << cons3 << " = " <<  *(cons ^ cons3) << std::endl;

        std::cout << "Must fail (these 2 are bound together)\n";
        std::cout << "Need to handle ('a ^ 'a list) and ('a ^ ('a -> 'a)) cases\n";
        std::cout << e  << " ^ " << lf << " = " <<  (e ^ lf) << std::endl;
        std::cout << "Must work fine\n";
        std::cout << f  << " ^ " << lf << " = " <<  (f ^ lf) << std::endl;

        std::cout << "\nFORALL MULTI-BIND TESTS: \n";
        
        Function v2(&f, {&e, &f}), good(&a, {&c, &a}), okay{&a, {&c, &g}},
                        bad(&a, {&c, &c});
        std::cout << v2  << " ^ " << good << " = " <<  *(v2 ^ good) << std::endl;
        std::cout << "FIX THIS SHOULD BE (sym * int -> int)\n";
        std::cout << v2  << " ^ " << okay  << " = " << *(v2 ^ okay)  << std::endl;
        std::cout << v2  << " ^ " << bad  << " = " <<   (v2 ^ bad)  << std::endl;
}

#endif
