#ifndef FIBIN_H
#define FIBIN_H

#include <type_traits>
#include <stdexcept>
#include <iostream>

namespace details {
    /// Checks if char is a letter or digit and converts it into unsigned.
    /// \param c - checked char
    /// \return - converted into int
    constexpr unsigned charToInt(char c) {
        if ('0' <= c && c <= '9')
            return c - '0';
        if ('a' <= c && c <= 'z')
            return c - 'a' + 10;
        if ('A' <= c && c <= 'Z')
            return c - 'A' + 10;
        else
            throw std::logic_error("Wrong variable name: wrong char");
    }
}

/// Hashes string into unsigned. Checks if its length is in [1, 6].
/// \param s - pointer to the string
/// \param n - index of first char to hash
/// \return - unsigned hashed value
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

/// Structs defined in problem statement:
struct True {
};

struct False {
};

template<unsigned VarNo>
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

template<typename T1, typename T2>
struct Eq {
};

template<typename Cond, typename IfTrue, typename IfFalse>
struct If {
};

template<unsigned VarId, typename Val, typename Exp>
struct Let {
};

template<unsigned VarId, typename Body>
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


/// Main Fibin class
/// \tparam T - value type used for calculations
/// \tparam Enable - parameter used for type differentiation
template<typename T, typename Enable = void>
class Fibin;

/// Specialized Fibin class for non integer types
template<typename T>
class Fibin<T, typename std::enable_if_t<!std::is_integral_v<T>>> {
public:
    template<typename Exp>
    static void eval() {
        std::cout << "Fibin doesn't support: " << typeid(T).name() << std::endl;
    }
};

/// Specialized Fibin class for integer types
template<typename T>
class Fibin<T, typename std::enable_if_t<std::is_integral_v<T>>> {
public:
    template<typename Exp>
    static constexpr T eval() {
        return Eval<EmptyEnv, Exp>::result::val;
    }

private:
    /// Returns n-th Fibonacci number
    static constexpr T fibo(unsigned n) {
        if (n == 0)
            return 0;
        int a = 0, b = 1, c = 0;
        while (n-- > 1) {
            c = a + b;
            a = b;
            b = c;
        }
        return b;
    }

    /// Empty environment list
    struct EmptyEnv {
    };

    /// Not empty environment list
    /// \tparam VarId - variable id (hashed value)
    /// \tparam Val - variable value
    /// \tparam Tail - rest of the environment list
    template<unsigned VarId, typename Val, typename Tail>
    struct EnvList {
    };

    /// Function type (lambda)
    /// \tparam VarId - variable id (hashed value)
    /// \tparam Exp - expression (function value)
    /// \tparam Env - function declaration environment
    template<unsigned VarId, typename Exp, typename Env>
    struct Function {
    };

    /// Value type
    template<T a>
    struct Value {
        static const T val = a;
    };

    /// Type for evaluating all expressions
    template<typename Env, typename Expr>
    struct Eval {
    };

    /// Value evaluation
    template<typename Env, T a>
    struct Eval<Env, Value<a>> {
        using result = Value<a>;
    };

    /// Lit evaluation for Fib<unsigned>
    template<typename Env, unsigned i>
    struct Eval<Env, Lit<Fib<i>>> {
        using result = Value<fibo(i)>;
    };

    /// Lit evaluation for False
    template<typename Env>
    struct Eval<Env, Lit<False>> {
        using result = False;
    };

    /// Lit evaluation for True
    template<typename Env>
    struct Eval<Env, Lit<True>> {
        using result = True;
    };

    /// Eq evaluation
    template<typename Env, typename T1, typename T2>
    struct Eval<Env, Eq<T1, T2>> {
        using result = typename std::conditional_t<
                std::is_same_v<typename Eval<Env, T1>::result,
                        typename Eval<Env, T2>::result>, True, False>;
    };

    /// Sum evaluation for two components
    template<typename Env, typename A, typename B>
    struct Eval<Env, Sum<A, B>> {
        using result = Value<
                Eval<Env, A>::result::val + Eval<Env, B>::result::val>;
    };

    /// Sum evaluation for more than two components
    template<typename Env, typename Head, typename... Tail>
    struct Eval<Env, Sum<Head, Tail...>> {
        using result = typename Eval<Env, Sum<typename Eval<Env, Head>::result,
                typename Eval<Env, Sum<Tail...>>::result>>::result;
    };

    /// Inc1 evaluation
    template<typename Env, typename Val>
    struct Eval<Env, Inc1<Val>> {
        using result = typename Eval<Env, Sum<Val, Lit<Fib<1>>>>::result;
    };

    /// Inc10 evaluation
    template<typename Env, typename Val>
    struct Eval<Env, Inc10<Val>> {
        using result = typename Eval<Env, Sum<Val, Lit<Fib<10>>>>::result;
    };

    /// If evaluation
    template<typename Env, typename Cond, typename IfTrue, typename IfFalse>
    struct Eval<Env, If<Cond, IfTrue, IfFalse>> {
        using cond_result = typename Eval<Env, Cond>::result;

        static_assert(std::is_same_v<cond_result, True> ||
                      std::is_same_v<cond_result, False>);

        using result = typename Eval<Env, std::conditional_t<
                std::is_same_v<cond_result, True>,
                IfTrue,
                IfFalse
        >>::result;
    };

    /// Ref evaluation for fitting environment list head
    template<unsigned VarId, typename Val, typename EnvTail>
    struct Eval<EnvList<VarId, Val, EnvTail>, Ref<VarId>> {
        using result = Val;
    };

    /// Ref evaluation for unfitting environment list head
    template<unsigned VarId, unsigned EnvVarId, typename Val, typename EnvTail>
    struct Eval<EnvList<EnvVarId, Val, EnvTail>, Ref<VarId>> {
        using result = typename Eval<EnvTail, Ref<VarId>>::result;
    };

    /// Let evaluation
    template<typename Env, unsigned VarId, typename ValExp, typename ResultExp>
    struct Eval<Env, Let<VarId, ValExp, ResultExp>> {
        using result = typename Eval<
                EnvList<VarId, typename Eval<Env, ValExp>::result, Env>,
                ResultExp>::result;
    };

    /// Invoke evaluation for basic function
    template<typename Env, unsigned VarId, typename Exp, typename Param,
            typename FEnv>
    struct Eval<Env, Invoke<Function<VarId, Exp, FEnv>, Param>> {
        using param_result = typename Eval<Env, Param>::result;

        using result = typename Eval<EnvList<VarId, param_result, FEnv>,
                Exp>::result;
    };

    /// Invoke evaluation for complex function
    template<typename Env, typename Func, typename Param>
    struct Eval<Env, Invoke<Func, Param>> {
        using func_result = typename Eval<Env, Func>::result;

        using result = typename Eval<Env,
                Invoke<func_result, Param>>::result;
    };

    /// Lambda evaluation
    template<typename Env, unsigned VarId, typename Exp>
    struct Eval<Env, Lambda<VarId, Exp>> {
        using result = Function<VarId, Exp, Env>;
    };
};


#endif //FIBIN_H
