#ifndef PROJECT_4_FIBO_PLAYGROUND_H
#define PROJECT_4_FIBO_PLAYGROUND_H

#include <type_traits>

struct True {};
struct False {};

/// Zmienne (na razie zmienna nie ma nazwy tylko numer bo nie umiem
template<unsigned i>
    struct Var {};

template<typename V>
    struct Ref {};

template<typename... A>
    struct Sum {};

template<typename t1, typename t2>
    struct Eq
{
    template<typename T> using value =
        typename std::conditional
        <
            std::is_same
            <
                typename t1::template value<T>,
                typename t2::template value<T>
            >::template value<T>,
            True,
            False
        >::type;
};


/// If
template<typename Cond, typename IfTrue, typename IfFalse>
    struct If
{
    template<typename T> using value =
        typename std::conditional
        <
            std::is_same
            <
                typename Cond::value,
                True
            >::template value<T>,
            IfTrue,
            IfFalse
        >::type;
};

template<class var, class value, class exp> struct Let {};

template <typename T>
    class Fibin
{
public:

    /// Wewnętrzne wartości
    template<T a> struct Value
    {
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
};

/// Zewętrzne wartości
template<unsigned i>
    struct Fib
{
    template<typename T> using value = typename Fibin<T>::template Value<Fibin<T>::fibo(i)>;
};

template<typename V>
    struct Lit;

template<unsigned  i>
    struct Lit<Fib<i>>
{
    template<typename T> using value = typename Fib<i>::template value<T>;
};

/// Suma
//todo - add other types

template<int a, int b>
    struct Sum<Fibin<int>::Value<a>, Fibin<int>::Value<b>>
{
    template<typename T> using value = typename Fibin<T>::template Value<a + b>::template value<T>;
};

template<int head, typename... tail>
    struct Sum<Fibin<int>::Value<head>, tail...>
{
    template<typename T> using value =
    typename Sum<typename Fibin<T>::template Value<head>, typename Sum<tail...>::template value<T>>::template value<T>;
};

///Eq
template<>
    struct Eq<True, True>
{
    template<typename T> using value = True;
};

template<>
    struct Eq<False, True>
{
    template<typename T> using value = False;
};


template<>
    struct Eq<True, False>
{
    template<typename T> using value = False;
};


template<>
    struct Eq<False, False>
{
    template<typename T> using value = True;
};


template<int v1, int v2>
    struct Eq<Fibin<int>::Value<v1>, Fibin<int>::Value<v2>>
{
    template<typename T> using value = typename std::conditional<v1 == v2, True, False>::type;
};

/// Rekursja leta
template<typename From, typename To, typename In> struct Change {};

template<typename From, typename To, typename... Params>
    struct Change<From, To, Sum<Params...>>
{
    template<typename T> using value =
    typename Sum<typename Change<From, To, Params>::template value<T>...>::template value<T>;
};

template<typename From, typename To, typename... Params>
    struct Change<From, To, Eq<Params...>>
{
    template<typename T> using value =
    typename Eq<typename Change<From, To, Params>::template value<T>...>::template value<T>;
};

template<typename From, typename To, typename... Params>
    struct Change<From, To, If<Params...>>
{
    template<typename T> using value =
    typename If<typename Change<From, To, Params>::template value<T>...>::template value<T>;
};

template<typename From, typename To, typename... Params>
    struct Change<From, To, Let<Params...>>
{
    template<typename T> using value =
    typename Let<typename Change<From, To, Params>::template value<T>...>::template value<T>;
};

template<typename From, typename To>
    struct Change<From, To, Ref<From>>
{
    template<typename T> using value =
    typename To::template value<T>;
};

template<unsigned VarNumber, typename To, typename Other, typename Exp>
    struct Change<Var<VarNumber>, To, Let<Var<VarNumber>, Other, Exp>>
{
    template<typename T> using value =
    typename Let<Var<VarNumber>, Other, Exp>::template value<T>;
};

/// Let
template<unsigned VarNumber, typename V, typename Exp>
    struct Let<Var<VarNumber>, V, Exp>
{
    template<typename T> using value =
    typename Change<Var<VarNumber>, typename V::template value<T>, Exp>::template value<T>;
};

#endif //PROJECT_4_FIBO_PLAYGROUND_H
