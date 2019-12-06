#ifndef PROJECT_4_FIBO_PLAYGROUND_H
#define PROJECT_4_FIBO_PLAYGROUND_H

#include <type_traits>
#include <stdexcept>
#include <iostream>

namespace details {
    constexpr unsigned charToInt(char c) {
        if ('0' <= c && c <= '9')
            return c - '0';
        if ('a' <= c && c <= 'z')
            return c - 'a' + 10;
        if ('A' <= c && c <= 'Z')
            return c - 'A' + 10;
        else //TODO: jak usunąć tego elsa, ale żeby się kompilowało?
            throw std::logic_error("Wrong variable name: wrong char");
    }
}

constexpr unsigned Var(const char *s, int n = 0) {
    if (s[n] == '\0') {
        if (n == 0)
            throw std::logic_error("Wrong variable name: empty name");
        return 0;
    }
    if (n > 6)
        throw std::logic_error("Wrong variable name: length > 6");
    return details::charToInt(s[n]) + 1 + (10 + 'z' - 'a' + 1) * Var(s, n + 1);
}

struct True {
};
struct False {
};

template<unsigned i>
struct Ref {
};

template<typename... A>
struct Sum {
};

template<typename Val>
struct Inc1 {
};

template<typename Val>
struct Inc10 {
};

template<typename t1, typename t2>
struct Eq {
};

template<typename Cond, typename IfTrue, typename IfFalse>
struct If {
};

template<unsigned VarNo, typename Val, typename Exp>
struct Let {
};

template<unsigned VarNo, typename Body>
struct Lambda {
};

template<typename Function, typename Param>
struct Invoke {
};

template<typename V>
struct Lit {
};

template<unsigned i>
struct Fib {
};


template<typename T, typename Enable = void>
class Fibin;

template<typename T>
class Fibin<T, typename std::enable_if_t<!std::is_integral_v<T>>> {
public:
    template<typename Exp>
    static void eval() {
        std::cout << "Fibin doesn't support: " << typeid(T).name() << std::endl;
    }
};

// Fibin
template<typename T>
class Fibin<T, typename std::enable_if_t<std::is_integral_v<T>>> {
public:
    static constexpr T fibo(unsigned pos) {
        int a = 0, b = 1, c = 0;
        while (pos-- > 0) {
            c = a + b;
            a = b;
            b = c;
        }
        return a;
    }

    template<typename Exp>
    static constexpr T eval() {
        return Eval<EmptyEnv, Exp>::result::val;
    }

private:

    // Lista środowiska
    struct EmptyEnv {
    };

    template<unsigned VarNo, typename Val, typename Tail>
    struct EnvList {
    };

    // Typ do łączenia list środowisk
    template<typename EnvFirst, typename EnvSecond, typename Rev>
    struct MergeEnv {
    };

    template<typename EnvSecond>
    struct MergeEnv<EmptyEnv, EnvSecond, EmptyEnv>
    {
        using result = EnvSecond;
    };

    template<typename EnvSecond, unsigned VarNo, typename Val, typename RevTail>
    struct MergeEnv<EmptyEnv, EnvSecond, EnvList<VarNo, Val, RevTail>>
    {
        using result = typename MergeEnv<EmptyEnv, EnvList<VarNo, Val, EnvSecond>, RevTail>::result;
    };

    template<unsigned VarNo, typename Val, typename FirstTail, typename EnvSecond, typename Rev>
    struct MergeEnv<EnvList<VarNo, Val, FirstTail>, EnvSecond, Rev>
    {
        using result = typename MergeEnv<FirstTail, EnvSecond, EnvList<VarNo, Val, Rev>>::result;
    };

    // Typ przechowujący funkcję
    template<unsigned VarNo, typename Exp, typename Env>
    struct Function {
        static const int xd = 69;
    };

    // Typ przechowujący wartość
    template<T a>
    struct Value {
        static const T val = a;
    };

    // Typ do ewalacji
    template<typename Env, typename Expr>
    struct Eval {
    };

    // Ewaluacja Value
    template<typename Env, T a>
    struct Eval<Env, Value<a>>
    {
        using result = Value<a>;
    };

    /*// Ewaluacja Funkcji
    template<typename Env, unsigned VarNo, typename Exp, typename FEnv>
    struct Eval<Env, Function<VarNo, Exp, FEnv>>
    {
        using result = Function<VarNo, Exp, FEnv>;
    };*/

    // Ewaluacja Lita
    template<typename Env, unsigned i>
    struct Eval<Env, Lit<Fib<i>>> {
        using result = Value<fibo(i)>;
    };

    template<typename Env>
    struct Eval<Env, Lit<False>> {
        using result = False;
    };

    template<typename Env>
    struct Eval<Env, Lit<True>> {
        using result = True;
    };

    // Ewaluacja Eq
    template<typename Env, typename T1, typename T2>
    struct Eval<Env, Eq<T1, T2>> {
        using result = typename std::conditional_t<
                std::is_same_v<typename Eval<Env, T1>::result,
                               typename Eval<Env, T2>::result>, True, False>;
    };

    // Ewaluacja sumy
    template<typename Env, typename A, typename B>
    struct Eval<Env, Sum<A, B>> {
        using result = Value<
                Eval<Env, A>::result::val + Eval<Env, B>::result::val>;
    };

    template<typename Env, typename Head, typename... Tail>
    struct Eval<Env, Sum<Head, Tail...>> {
        using result = typename Eval<Env, Sum<typename Eval<Env, Head>::result,
                typename Eval<Env, Sum<Tail...>>::result> >::result;
    };

    // Ewaluacja Inc1
    template<typename Env, typename Val>
    struct Eval<Env, Inc1<Val>> {
        using result = typename Eval<Env, Sum<Val, Lit<Fib<1>>>>::result;
    };

    // Ewaluacja Inc10
    template<typename Env, typename Val>
    struct Eval<Env, Inc10<Val>> {
        using result = typename Eval<Env, Sum<Val, Lit<Fib<10>>>>::result;
    };

    // Ewaluacja If
    template<typename Env, typename Cond, typename IfTrue, typename IfFalse>
    struct Eval<Env, If<Cond, IfTrue, IfFalse>> {
        using cond_result = typename Eval<Env, Cond>::result;

        static_assert(std::is_same_v<cond_result, True> ||
                      std::is_same_v<cond_result, False>);

        using result = typename Eval<Env, std::conditional_t<
                std::is_same_v<cond_result, True>,
                IfTrue,
                IfFalse
                >>::result ;
    };

    // Ewaluacja Ref
    template<unsigned VarNo, typename Val, typename EnvTail>
    struct Eval<EnvList<VarNo, Val, EnvTail>, Ref<VarNo>> {
        using result = Val;
    };

    template<unsigned VarNo, unsigned EnvVarNo, typename Val, typename EnvTail>
    struct Eval<EnvList<EnvVarNo, Val, EnvTail>, Ref<VarNo>> {
        using result = typename Eval<EnvTail, Ref<VarNo> >::result;
    };

    // Ewaluacja Let
    template<typename Env, unsigned VarNo, typename ValExp, typename ResultExp>
    struct Eval<Env, Let<VarNo, ValExp, ResultExp>> {
        using result = typename Eval<EnvList<VarNo, typename Eval<Env, ValExp>::result, Env>,
                ResultExp>::result;
    };

    // Ewaluacja Invoke
    template<typename Env, unsigned VarNo, typename Exp, typename Param, typename FEnv>
    struct Eval<Env, Invoke<Function<VarNo, Exp, FEnv>, Param>> {
        using param_result = typename Eval<Env, Param>::result;

        using result = typename Eval<EnvList<VarNo, param_result, typename MergeEnv<FEnv, Env, EmptyEnv>::result>,
                Exp>::result;
    };

    template<typename Env, typename Func, typename Param>
    struct Eval<Env, Invoke<Func, Param>> {
        using func_result = typename Eval<Env, Func>::result;

        static_assert(func_result::xd == 69);

        using result = typename Eval<Env,
                Invoke<func_result, Param> >::result;
    };

    // Ewaluacja Lambda
    template<typename Env, unsigned VarNo, typename Exp>
    struct Eval<Env, Lambda<VarNo, Exp>> {
        using result = Function<VarNo, Exp, Env>;
    };
};


#endif //PROJECT_4_FIBO_PLAYGROUND_H
