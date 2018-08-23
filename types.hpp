#ifndef TYPES_H_
#define TYPES_H_

#include <vector>
#include <iostream>

/* Sub-Classes */
struct Prim_type;
struct Fun_type;
struct List_type;

struct Type {
        virtual Type *binds(const std::vector<Type*> &arg_types) const = 0;
        virtual
        bool operator==(const Type &type) const = 0;
        bool operator!=(const Type &type) const;
        
        friend std::ostream &operator<<(std::ostream &os, const Type &type);
        

        enum Primitive { INT, BOOL, SYMBOL };
        
        /* Double dispatch methods */
        virtual bool compare(const Prim_type &other) const = 0;
        virtual bool compare(const Fun_type  &other) const = 0;
        virtual bool compare(const List_type &other) const = 0;
        
        virtual void print(std::ostream &os) const = 0;
};

/* PRIMITIVE (INT BOOL SYM) */
struct Prim_type : Type {
        Prim_type(Type::Primitive prim) : prim_type(prim) {}
        
        bool operator==(const Type &type) const;
        Type *binds(const std::vector<Type*> &) const;

private:
        const Type::Primitive prim_type;

        bool compare(const Prim_type &other) const;
        bool compare(const Fun_type  &other) const;
        bool compare(const List_type &other) const;
        
        void print(std::ostream &os) const;
};

/* FUNCTION */
struct Fun_type : Type {
        Fun_type(Type &ret, const std::vector<Type*> arg_t) :
                ret_type(ret), arg_types(arg_t){}
        
        bool operator==(const Type &type) const;
        Type *binds(const std::vector<Type*> &_arg_types) const;

private:
        bool compare(const Prim_type &other) const;
        bool compare(const Fun_type  &other) const;
        bool compare(const List_type &other) const;
        
        void print(std::ostream &os) const;
                        
        Type &ret_type;
        std::vector<Type*> arg_types;
};

/* LIST */
struct List_type : Type {
        explicit List_type(const List_type &elem_tp);
        explicit List_type(const Type &elem_tp) : elem_type(elem_tp) {}
        
        bool operator==(const Type &type) const;
        Type *binds(const std::vector<Type*> &) const;

public:
        const Type &elem_type;
        bool compare(const Prim_type &other) const;
        bool compare(const Fun_type  &other) const;
        bool compare(const List_type &other) const;

        void print(std::ostream &os) const;
};
#endif // TYPES_H_
