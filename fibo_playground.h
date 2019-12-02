#ifndef PROJECT_4_FIBO_PLAYGROUND_H
#define PROJECT_4_FIBO_PLAYGROUND_H

#include <type_traits>

struct True {};
struct False {};

constexpr unsigned Var(const char* s)
{
    return s[0];
}

template<unsigned i> struct Ref {};

template<typename... A> struct Sum {};

template<typename Val> Inc1 {};

template<typename Val> Inc10 {};

template<typename t1, typename t2> struct Eq {};

template<typename Cond, typename IfTrue, typename IfFalse> struct If {};

template<unsigned VarNo, typename Val, typename Exp> struct Let {};

template<unsigned VarNo, typename Body> struct Lambda {};

template<typename Function, typename Param> Invoke {};

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


private:

    struct EmptyEnv {};

    template<unsigned VarNo, typename Val, typename Tail> struct EnvList {};

    template<typename Env, typename Expr> Eval;

    // Ewaluacja Lita
    template<typename Env, unsigned i> Eval<Env, Lit<Fib<i>>
    {
        using result = Value<fibo(i)>;
    };

    // Ewaluacja EQ
    template<typename Env, typename T1, typename T2> Eval<Env, Eq<T1, T2>>
    {
        using result =
        typename std::conditional
        <
            std::is_same
            <
                typename Eval<Env, T1>::result
                typename Eval<Env, T2>::result
            >,
            True,
            False
        >::type;
    };

    // Ewaluacja sumy
    template<typename Env, typename A, typename B> struct Eval<Env, Sum<A, B>>
    {
        using result =
        Value
        <
            Eval<Env, A>::result::value +
            Eval<Env, B>::result::value
        >;
    };

    template<typename Env, typename Head, typename... Tail> struct Eval<Env, Sum<Head, Tail...>>
    {
        using result =
        Eval
        <
            Env,
            Sum
            <
                Eval<Env, Head>::result,
                Eval<Env, Sum<Tail...>>::result
            >
        >::result;
    };

    // Ewaluacja Inc1
    template<typename Env, typename Val> struct Eval<Env, Inc1<Val>>
    {
        using result = Eval<Env, Sum<Val, Lit<Fib<1>>>>::result;
    };

    // Ewaluacja Inc10
    template<typename Env, typename Val> struct Eval<Env, Inc10<Val>>
    {
        using result = Eval<Env, Sum<Val, Lit<Fib<10>>>>::result;
    };

    // Ewaluacja If
    template<typename Env, typename Cond, typename IfTrue, typename IfFalse> struct Eval<Env, If<Cond, IfTrue, IfFalse>>
    {
        using cond_result = Eval<Env, Cond>;

        static_assert
        (
            std::is_same<cond_result, True>::value ||
            std::is_same<cond_resilt, False>::value
        );

        using result =
        std::conditional
        <
            std::is_same
            <
                cond_result,
                True
            >::value,
            Eval<Env, IfTrue>::result,
            Eval<Env, IfFalse>::result
        >::type;
    };

    // Ewaluacja Ref
    template<unsigned VarNo, typename Val, typename EnvTail> struct Eval<EnvList<VarNo, Val, EnvTail>, Ref<VarNo>>
    {
        using result = Val;
    };

    template<unsigned VarNo, unsigned EnvVarNo, typename Val, typename EnvTail> struct Eval<EnvList<EnvVarNo, Val, EnvTail>, Ref<VarNo>>
    {
        using result =
        Eval
        <
            EnvTail,
            Ref<VarNo>
        >::result;
    };

    // Ewaluacja Let
    template<typename Env, unsigned VarNo, typename ValExp, typename ResultExp> struct Eval<Env, Let<VarNo, ValExp, ResultExp>>
    {
        using val_result = Eval<Env, ValExp>::result;

        using result =
        Eval
        <
            EnvList
            <
                VarNo,
                var_result,
                Env
            >,
            ResultExp
        >::result;
    };

    // Ewaluacja Invoke
    template<typename Env, unsigned VarNo, typename Exp, typename Param> struct Eval<Env, Invoke<Lambda<VarNo, Exp>, Param>>
    {
        using param_result = Eval<Env, Param>::result;

        using result =
        Eval
        <
            EnvList
            <
                VarNo
                param_result,
                Env
            >,
            Exp
        >::result;
    };

public:

    template<typename T> eval()
    {
        return Eval<EmptyEnv, T>::result::value;
    }

    void eval()
    {
        static_assert()
    }

};

#endif //PROJECT_4_FIBO_PLAYGROUND_H
