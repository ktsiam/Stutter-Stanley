#ifndef TYPE_H_
#define TYPE_H_

#include <vector>
#include <map>
#include <iostream>

#define DEBUG
/* Sub-Classes */

namespace Tp {

struct Primitive;
struct Forall;
struct List;
struct Function;
        
} // namespace Tp


struct Type {

        /* Full bind (Forall unbound) */
        friend Type *operator^(Type &a, Type &b);

        /* Partial bind : shall never return bound Forall */
        virtual Type *bind(Type &tp) = 0;
        virtual void  unbind()       = 0;
        
        virtual Type *binds_with(Tp::Primitive &other);
        virtual Type *binds_with(Tp::List      &other);
        virtual Type *binds_with(Tp::Forall    &other);
        virtual Type *binds_with(Tp::Function  &other);

        friend std::ostream &operator<<(std::ostream &os, Type &t);
        virtual void print(std::ostream& os, std::map<int, char> &bindings) = 0;
};


namespace Tp {

/* PRIMITIVE (INT BOOL SYM) */
struct Primitive : Type {
        enum Tag { INT, BOOL, SYMBOL };        
        Primitive(Tag _tag) : tag(_tag) {}
        
        Tag tag;

        Type *bind(Type &o)                override;
        Type *binds_with(Tp::Primitive &o) override;
        Type *binds_with(Tp::Forall    &o) override;
        void  unbind()                     override;

        void print(std::ostream& os, std::map<int, char> &bindings) override;
};

/* FORALL */
struct Forall : Type {
        explicit Forall() : bound(new Type*(NULL)) {
                static int ID_COUNT = 0; id = ID_COUNT++;
        }
        Forall(const Forall &o) : bound(o.bound), id(o.id) {}

        
        
        Type **bound; // pointer-to-pointer?
        int id;
        
        Type *bind(Type &o)                override;
        Type *binds_with(Tp::Primitive &o) override;
        Type *binds_with(Tp::Function  &o) override;
        Type *binds_with(Tp::List      &o) override;
        Type *binds_with(Tp::Forall    &o) override;
        void  unbind()                     override;

        void print(std::ostream& os, std::map<int, char> &bindings) override;

};

/* LIST */
struct List : Type {
        explicit List(Type *elem_tp) : elem_type(elem_tp) {}

        Type *elem_type;

        Type *bind(Type &o)             override;
        Type *binds_with(Tp::List   &o) override;
        Type *binds_with(Tp::Forall &o) override;
        void  unbind()                  override;

        void print(std::ostream& os, std::map<int, char> &bindings) override;
};

/* FUNCTION */
struct Function : Type {
        Function(Type *ret, const std::vector<Type*> arg_t) :
                ret_type(ret), arg_types(arg_t){}        
           
        Type *ret_type;
        std::vector<Type*> arg_types;

        Type *bind(Type &o)               override;        
        Type *binds_with(Tp::Function &o) override;
        Type *binds_with(Tp::Forall   &o) override;
        void  unbind()                    override;

        void print(std::ostream& os, std::map<int, char> &bindings) override;
};


} // namespace Tp

#endif // TYPE_H_
