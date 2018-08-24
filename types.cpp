#include "types.hpp"

bool Type::operator!=(const Type &type) const
{
        return ! (type == *this);
}

std::ostream &operator<<(std::ostream &os, const Type &type)
{
        type.print(os);
        return os;
}

/* Implementing Type children */
using namespace Tp;

/* PRIMITIVE */

bool Primitive::operator==(const Type &type) const
{
        return type.compare(*this);
}

Type *Primitive::binds(const std::vector<Type*> &) const
{
        assert(false && "Not a function");
}

bool Primitive::compare(const Primitive &other) const
{
        return other.tag == tag;
}
bool Primitive::compare(const Function &) const
{
        return false;
}

bool Primitive::compare(const List &) const
{
        return false;
}

void Primitive::print(std::ostream &os) const
{
        static const std::string printable_types[] = { "int", "bool", "symbol" };
        os << printable_types[tag];
}

/* FUNCTION */

bool Function::operator==(const Type &type) const
{
        return type.compare(*this);
}


Type *Function::binds(const std::vector<Type*> &_arg_types) const
{
        std::size_t size = arg_types.size();
        if (_arg_types.size() != size) return NULL;
        for (std::size_t i = 0; i < size; ++i)
                if (*arg_types[i] != *_arg_types[i])
                        return NULL;
        return &ret_type;
}


bool Function::compare(const Primitive &) const
{
        return false;
}

bool Function::compare(const Function &other) const
{
        Type *ret = other.binds(arg_types);
        return (ret != NULL) && (*ret == ret_type);
}

bool Function::compare(const List &) const
{
        return false;
}

void Function::print(std::ostream &os) const
{
        std::string sep = "";
        os << "([";
        for (Type *arg : arg_types) {
                os << sep << *arg;
                sep = ", ";
        }
        os << "] -> " << ret_type << ')';
}

/* LIST */

/* redirecting copy ctor to ctor */
List::List(const List &elem_tp) :
        List(static_cast<const Type&> (elem_tp)) {}
        

bool List::operator==(const Type &type) const
{
        return type.compare(*this);
}

Type *List::binds(const std::vector<Type*> &) const
{
        assert(false && "Not a function");
}

bool List::compare(const Primitive &) const
{
        return false;
}
bool List::compare(const Function  &) const
{
        return false;
}
bool List::compare(const List &other) const
{
        return other.elem_type == elem_type;
}

void List::print(std::ostream &os) const
{
        os << "(list " << elem_type << ')';
}


/* TESTING */
#ifdef DEBUG
int main()
{
        Tp::Primitive a(Tp::Primitive::INT), b(Tp::Primitive::BOOL),    \
                      c(Tp::Primitive::SYMBOL);
        Tp::List la(a), lb(b), lla(la);
        Tp::Function af(a, {}), bfac(b, { &a, &c }), lblaa(lb, {&la, &a});
        
        std::cout << "Equality:\n" << std::boolalpha
                  << a << " == " << a << std::endl
                  << (a == a) << std::endl
                  << a << " == " << b << std::endl
                  << (a == b) << std::endl
                  << la << " == " << a << std::endl
                  << (la == a) << std::endl
                  << la << " == " << la << std::endl
                  << (la == la) << std::endl
                  << la << " == " << lb << std::endl
                  << (la == lb) << std::endl
                  << la << " == " << lla << std::endl
                  << (la == lla) << std::endl;
        
        std::cout << af << " binds with {} and returns: " << std::endl;
        Type *ret = af.binds({});
        if (ret) std::cout << *ret << std::endl;
        else     std::cout << "doesn't bind" << std::endl;

        std::cout << bfac << " binds with { int, symbol } and returns: " << std::endl;
        ret = bfac.binds({ &a, &c });
        if (ret) std::cout << *ret << std::endl;
        else     std::cout << "doesn't bind" << std::endl;

        std::cout << lblaa << " binds with { list int, int } and returns: "<<std::endl;
        ret = lblaa.binds({ &la, &a });
        if (ret) std::cout << *ret << std::endl;
        else     std::cout << "doesn't bind" << std::endl;

        
}
#endif
