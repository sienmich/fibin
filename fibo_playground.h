#ifndef PROJECT_4_FIBO_PLAYGROUND_H
#define PROJECT_4_FIBO_PLAYGROUND_H

#include <type_traits>
#include <iostream>

struct True {};
struct False {};

static constexpr bool charToInt(char c) {
    return  ('0' <= c && c <= '9') ? c - '0':
            ('a' <= c && c <= 'z') ? c - 'a' + 10:
            ('A' <= c && c <= 'Z') ? c - 'A' + 10:
            throw std::logic_error("Wrong variable name: wrong char");
}

static constexpr unsigned Var(const char* s, int n = 0) {
    return s[n] == '\0' ?
           (n == 0 ? throw std::logic_error("Wrong variable name: length = 0") : 0):
           (n > 6 ? throw std::logic_error("Wrong variable name: length > 6") :
            charToInt(s[n]) + 1 + (10 + 'z' - 'a' + 1) * Var(s, n+1)
           );
}

template<unsigned i> struct Ref {};

template<typename... A> struct Sum {};

template<typename Val> struct Inc1 {};

template<typename Val> struct Inc10 {};

template<typename t1, typename t2> struct Eq {};

template<typename Cond, typename IfTrue, typename IfFalse> struct If {};

template<unsigned VarNo, typename Val, typename Exp> struct Let {};

template<unsigned VarNo, typename Body> struct Lambda {};

template<typename Function, typename Param> struct Invoke {};

template<typename V> struct Lit {};

template<unsigned i> struct Fib {};


// Fibin
template <typename T>
class Fibin
{
public:

    /// Wewnętrzne wartości
    template<T a> struct Value
    {
        static const T val = a;
    };


    static constexpr T fibo(unsigned pos) {
        int a = 0, b = 1, c = 0;
        while(pos --> 0) {
            c = a + b;
            a = b;
            b = c;
        }
        return a;
    }


private:

    struct EmptyEnv {};

    template<unsigned VarNo, typename Val, typename Tail> struct EnvList {};

    template<typename Env, typename Expr> struct Eval {};

    // Ewaluacja Lita
    template<typename Env, unsigned i> struct Eval<Env, Lit<Fib<i>>>
    {
        using result = Value<fibo(i)>;
    };

    template <typename  Env> struct Eval<Env, Lit<False>>
    {
        using result = False;
    };

    template <typename  Env> struct Eval<Env, Lit<True>>
    {
        using result = True;
    };

    // Ewaluacja Value
    template<typename Env, T i> struct Eval<Env, Value<i>>
    {
        using result = Value<i>;
    };


    // Ewaluacja Eq
    template<typename Env, typename T1, typename T2> struct Eval<Env, Eq<T1, T2>>
    {
        using result = typename std::conditional
        <
            std::is_same
            <
                typename Eval<Env, T1>::result,
                typename Eval<Env, T2>::result
            >::result,
            True,
            False
        >::type;
    };

    // Ewaluacja sumy
    template<typename Env, typename A, typename B> struct Eval<Env, Sum<A, B>>
    {
        using result = Value
        <
            Eval<Env, A>::result::val +
            Eval<Env, B>::result::val
        >;
    };

    template<typename Env, typename Head, typename... Tail> struct Eval<Env, Sum<Head, Tail...>>
    {
        using result = typename Eval
        <
            Env,
            Sum
            <
                typename Eval<Env, Head>::result,
                typename Eval<Env, Sum<Tail...>>::result
            >
        >::result;
    };

    // Ewaluacja Inc1
    template<typename Env, typename Val> struct Eval<Env, Inc1<Val>>
    {
        using result = typename Eval<Env, Sum<Val, Lit<Fib<1>>>>::result;
    };

    // Ewaluacja Inc10
    template<typename Env, typename Val> struct Eval<Env, Inc10<Val>>
    {
        using result = typename Eval<Env, Sum<Val, Lit<Fib<10>>>>::result;
    };

    // Ewaluacja If
    template<typename Env, typename Cond, typename IfTrue, typename IfFalse>
        struct Eval<Env, If<Cond, IfTrue, IfFalse>>
    {
        using cond_result = typename Eval<Env, Cond>::result ;

        static_assert
        (
            std::is_same<cond_result, True>::value ||
            std::is_same<cond_result, False>::value
        );

        using result = typename std::conditional
        <
            std::is_same
            <
                cond_result,
                True
            >::value,
            typename Eval<Env, IfTrue>::result,
            typename Eval<Env, IfFalse>::result
        >::type;
    };

    // Ewaluacja Ref
    template<unsigned VarNo, typename Val, typename EnvTail>
        struct Eval<EnvList<VarNo, Val, EnvTail>, Ref<VarNo>>
    {
//        using result = Val;
        using result = typename Eval
        <
            EnvList<VarNo, Val, EnvTail>,
            Val
        >::result;
    };

    template<unsigned VarNo, unsigned EnvVarNo, typename Val, typename EnvTail>
        struct Eval<EnvList<EnvVarNo, Val, EnvTail>, Ref<VarNo>>
    {
        using result = typename Eval
        <
            EnvTail,
            Ref<VarNo>
        >::result;
    };

    // Ewaluacja Let
    template<typename Env, unsigned VarNo, typename ValExp, typename ResultExp>
        struct Eval<Env, Let<VarNo, ValExp, ResultExp>>
    {
//        using val_result = typename Eval<Env, ValExp>::result;

        using result = typename Eval
        <
            EnvList
            <
                VarNo,
                ValExp,
                Env
            >,
            ResultExp
        >::result;
    };

    // Ewaluacja Invoke
    template<typename Env, unsigned VarNo, typename Exp, typename Param>
        struct Eval<Env, Invoke<Lambda<VarNo, Exp>, Param>>
    {
        using param_result = typename Eval<Env, Param>::result;

        using result = typename Eval
        <
            EnvList
            <
                VarNo,
                param_result,
                Env
            >,
            Exp
        >::result;
    };


    template<typename Env, typename Func, typename Param>
    struct Eval<Env, Invoke<Func, Param>>
    {
        using result = typename Eval
                <
                    Env,
                    Invoke
                    <
                    typename Eval<Env, Func>::result,
                    Param
                    >
                >::result;
    };

    // Ewaluacja Lambda
    template<typename Env, unsigned VarNo, typename Exp>
    struct Eval<Env, Lambda<VarNo, Exp>>
    {
        using result = Lambda<VarNo, Exp>;
    };



public:

    template<typename Exp>
    static constexpr
    std::enable_if_t<std::is_integral_v<T>, T>
    eval()
    {
            return Eval<EmptyEnv, Exp>::result::val;
    }
/*
    template<typename Exp>
    static
    std::enable_if_t<!std::is_integral_v<T>>
    eval()
    {
        std::cout << "Fibin doesn't support: PKc" << std::endl;
        return 0;
    }
*/
    /*template<typename Exp> static constexpr typename std::enable_if<!std::is_integral<T>::value, void>::type eval()
    {
    }*/
};

#endif //PROJECT_4_FIBO_PLAYGROUND_H
