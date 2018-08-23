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

/* PRIMITIVE */

bool Prim_type::operator==(const Type &type) const
{
        return type.compare(*this);
}

Type *Prim_type::binds(const std::vector<Type*> &) const
{
        assert(false && "Not a function");
}

bool Prim_type::compare(const Prim_type &other) const
{
        return other.prim_type == prim_type;
}
bool Prim_type::compare(const Fun_type &) const
{
        return false;
}

bool Prim_type::compare(const List_type &) const
{
        return false;
}

void Prim_type::print(std::ostream &os) const
{
        static const std::string printable_types[] = { "int", "bool", "symbol" };
        os << printable_types[prim_type];
}

/* FUNCTION */

bool Fun_type::operator==(const Type &type) const
{
        return type.compare(*this);
}


Type *Fun_type::binds(const std::vector<Type*> &_arg_types) const
{
        std::size_t size = arg_types.size();
        if (_arg_types.size() != size) return NULL;
        for (std::size_t i = 0; i < size; ++i)
                if (*arg_types[i] != *_arg_types[i])
                        return NULL;
        return &ret_type;
}


bool Fun_type::compare(const Prim_type &) const
{
        return false;
}

bool Fun_type::compare(const Fun_type &other) const
{
        Type *ret = other.binds(arg_types);
        return (ret != NULL) && (*ret == ret_type);
}

bool Fun_type::compare(const List_type &) const
{
        return false;
}

void Fun_type::print(std::ostream &os) const
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
List_type::List_type(const List_type &elem_tp) :
        List_type(static_cast<const Type&> (elem_tp)) {}

bool List_type::operator==(const Type &type) const
{
        return type.compare(*this);
}

Type *List_type::binds(const std::vector<Type*> &) const
{
        assert(false && "Not a function");
}

bool List_type::compare(const Prim_type &) const
{
        return false;
}
bool List_type::compare(const Fun_type  &) const
{
        return false;
}
bool List_type::compare(const List_type &other) const
{
        return other.elem_type == elem_type;
}

void List_type::print(std::ostream &os) const
{
        os << "(list " << elem_type << ')';
}


#ifdef DEBUG
int main()
{
        Prim_type a(Type::INT), b(Type::BOOL), c(Type::SYMBOL);
        List_type la(a), lb(b), lla(la);
        Fun_type af(a, {}), bfac(b, { &a, &c }), lblaa(lb, {&la, &a});
        
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
