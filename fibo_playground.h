//
// Created by piotr on 30.11.19.
//

#ifndef PROJECT_4_FIBO_PLAYGROUND_H
#define PROJECT_4_FIBO_PLAYGROUND_H


#include <type_traits>


template <typename T>
class Fibin {
private:

    /// Wewnętrzne wartości
    template<T a> struct Value
    {
        using value = Value<a>;
        static const T val = a;
    };


    static constexpr T fibo(unsigned pos)
    {
        return pos < 2 ? pos : fibo(pos - 1) + fibo(pos - 2);
    }


    /// Próby robienia zmiennych które serio mają nazwe
    constexpr bool isLegal(char c) {
        return
                ('a' <= c && c <= 'z') ||
                ('A' <= c && c <= 'Z') ||
                ('0' <= c && c <= '9');
    }

    constexpr bool verify(char* s, int n) {
        return s[n] == '\0' ?
               n > 0 && n < 7 :
               isLegal(s[n]) && verify(s, n + 1);
    }

    template<char... C> struct Variable;


    /*struct Var
    {
        //typename type;
        constexpr Var(char* s)
        {
            static_assert(verify(s, 0));
            using type = Variable<s[0], s[1], s[2], s[3], s[4], s[5]>
        }
    };*/


public:

    /// Zewętrzne wartości
    struct True {};
    struct False {};

    template<unsigned i> struct Fib
    {
        using value = Value<fibo(i)>;
    };

    template<typename t> using Lit = typename t::value;

    /// Zmienne (na razie zmienna nie ma nazwy tylko numer bo nie umiem
    template<unsigned i> struct Var {};

    template<typename V> struct Ref {};


    /// Suma
    template<typename... A> struct Sum;

    template<T a,T b> struct Sum<Value<a>, Value<b>>
    {
        using value = Value<a + b>;
    };

    template<T head, typename... tail> struct Sum<Value<head>, tail...>
    {
        using value = typename Sum<Value<head>, typename Sum<tail...>::value>::value;
    };

    ///Eq
    template<typename t1, typename t2> struct Eq
    {
        using value = typename std::conditional<
                std::is_same<typename t1::value, typename t2::value>::value,
                True, False>::type;
    };

    template<unsigned v1, unsigned v2> struct Eq<Value<v1>, Value<v2>>
    {
        using value = typename std::conditional<v1 == v2, True, False>::type;
    };

    /// If
    template<typename Cond, typename IfTrue, typename IfFalse> struct If
    {
        using value = typename std::conditional<
                std::is_same<typename Cond::value, True>::value,
                IfTrue, IfFalse>::type;
    };


    /// Let
    template<class var, class value, class exp> struct Let;

    template<unsigned VarNumber, typename V, typename Exp> struct Let<Var<VarNumber>, V, Exp>
    {
        using Ref<Var<VarNumber>> = V::value;
        using value = typename Exp::value;
    };
};


#endif //PROJECT_4_FIBO_PLAYGROUND_H
