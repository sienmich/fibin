#ifndef PROJECT_4_FIBO_PLAYGROUND_H
#define PROJECT_4_FIBO_PLAYGROUND_H


#include <type_traits>

struct True {};
struct False {};

/// Zmienne (na razie zmienna nie ma nazwy tylko numer bo nie umiem
template<unsigned i> struct Var {};

template<typename V> struct Ref {};

template<typename... A> struct Sum {};

template<typename t1, typename t2> struct Eq
{
    using value = typename std::conditional<
            std::is_same<typename t1::value, typename t2::value>::value,
            True, False>::type;
};


/// If
template<typename Cond, typename IfTrue, typename IfFalse> struct If
{
    using value = typename std::conditional<
            std::is_same<typename Cond::value, True>::value,
            IfTrue, IfFalse>::type;
};

template<class var, class value, class exp> struct Let {};

template <typename T>
class Fibin
{
public:

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

    /// Rekursja leta
    template<typename From, typename To, typename In> struct Change {};

    template<typename From, typename To, typename... Params> struct Change<From, To, Sum<Params...>>
    {
        using value = typename Sum<typename Change<From, To, Params>::value...>::value;
    };

    template<typename From, typename To, typename... Params> struct Change<From, To, Eq<Params...>>
    {
        using value = typename Eq<typename Change<From, To, Params>::value...>::value;
    };

    template<typename From, typename To, typename... Params> struct Change<From, To, If<Params...>>
    {
        using value = typename If<typename Change<From, To, Params>::value...>::value;
    };

    template<typename From, typename To, typename... Params> struct Change<From, To, Let<Params...>>
    {
        using value = typename Let<typename Change<From, To, Params>::value...>::value;
    };

    template<typename From, typename To> struct Change<From, To, Ref<From>>
    {
        using value = typename To::value;
    };

    template<unsigned VarNumber, typename To, typename Other, typename Exp> struct Change<Var<VarNumber>, To, Let<Var<VarNumber>, Other, Exp>>
    {
        using value = typename Let<Var<VarNumber>, Other, Exp>::value;
    };
};

/// Zewętrzne wartości
template<unsigned i> struct Fib
{
    using value = Fibin<int>::Value<Fibin<int>::fibo(i)>;
};

template<typename t> using Lit = typename t::value;

/// Suma
//todo - add other types
template<int a, int b> struct Sum<Fibin<int>::Value<a>, Fibin<int>::Value<b>>
{
    using value = typename Fibin<int>::Value<a + b>::value;
};

template<int head, typename... tail> struct Sum<Fibin<int>::Value<head>, tail...>
{
    using value = typename Sum<Fibin<int>::Value<head>, typename Sum<tail...>::value>::value;
};

///Eq
template<> struct Eq<True, True>
{
    using value = True;
};

template<> struct Eq<False, True>
{
    using value = False;
};


template<> struct Eq<True, False>
{
    using value = False;
};


template<> struct Eq<False, False>
{
    using value = True;
};


template<int v1, int v2> struct Eq<Fibin<int>::Value<v1>, Fibin<int>::Value<v2>>
{
    using value = typename std::conditional<v1 == v2, True, False>::type;
};

/// Let
template<unsigned VarNumber, typename V, typename Exp> struct Let<Var<VarNumber>, V, Exp>
{
    using value = typename Fibin<int>::Change<Var<VarNumber>, typename V::value, Exp>::value;
};


#endif //PROJECT_4_FIBO_PLAYGROUND_H
