#ifndef TYPES_H_
#define TYPES_H_

#include <vector>
#include <iostream>

#define DEBUG
/* Sub-Classes */
namespace Tp {

struct Primitive;
struct Function;
struct List;
        
} // namespace Tp


struct Type {
        /* Returns Function return type if it can bind with arguments */
        virtual Type *binds(const std::vector<Type*> &arg_types) const = 0;


        /* Protected functions (accessed only in class hierarchy) */
        virtual
        bool operator==(const Type &type) const = 0;
        bool operator!=(const Type &type) const;
        
        friend std::ostream &operator<<(std::ostream &os, const Type &type);
        
        
        
        /* Double dispatch methods */
        virtual bool compare(const Tp::Primitive &other) const = 0;
        virtual bool compare(const Tp::Function  &other) const = 0;
        virtual bool compare(const Tp::List      &other) const = 0;
        
        virtual void print(std::ostream &os) const = 0;
};

namespace Tp {

/* PRIMITIVE (INT BOOL SYM) */
struct Primitive : Type {
        enum Tag { INT, BOOL, SYMBOL };
        
        Primitive(Tag _tag) : tag(_tag) {}
        
        bool operator==(const Type &type) const;
        Type *binds(const std::vector<Type*> &) const;


private:
        
        const Tag tag;

        bool compare(const Primitive &other) const;
        bool compare(const Function  &other) const;
        bool compare(const List      &other) const;
        
        void print(std::ostream &os) const;
};

/* FUNCTION */
struct Function : Type {
        Function(Type &ret, const std::vector<Type*> arg_t) :
                ret_type(ret), arg_types(arg_t){}
        
        bool operator==(const Type &type) const;
        Type *binds(const std::vector<Type*> &_arg_types) const;

private:
        bool compare(const Primitive &other) const;
        bool compare(const Function  &other) const;
        bool compare(const List     &other) const;
        
        void print(std::ostream &os) const;
                        
        Type &ret_type;
        std::vector<Type*> arg_types;
};

/* LIST */
struct List : Type {
        explicit List(const Type &elem_tp) : elem_type(elem_tp) {}
        explicit List(const List  &elem_tp);
        explicit List(const List &&elem_tp) = delete;

        
        bool operator==(const Type &type) const;
        Type *binds(const std::vector<Type*> &) const;

public:
        const Type &elem_type;
        bool compare(const Primitive &other) const;
        bool compare(const Function  &other) const;
        bool compare(const List      &other) const;

        void print(std::ostream &os) const;
};

} // namespace Tp

#endif // TYPES_H_
