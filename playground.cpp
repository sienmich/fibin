//
// Created by piotr on 30.11.19.
//

#include "fibo_playground.h"



int main()
{
    /*Var("kappa") k = 1;

    Let<Var("X"), Lit<Fib<0>>
        Inc10<Ref<Var("X")>>
    >*/

    static_assert(Sum
    <
        Lit<Fib<0>>,
        Lit<Fib<1>>,
        Lit<Fib<2>>,
        Lit<Fib<3>>
    >::value::val == 4);

    static_assert(
        If<
        Eq
        <
            Sum
            <
                    Lit<Fib<4>>,
                    Lit<Fib<5>>
            >,
            Lit<Fib<6>>
        >,
        Lit<Fib<0>>,
        Lit<Fib<1>>
        >::value::val == 0);

    static_assert(
            Let<
                    Var<1>,
                    Fib<1>,
                    Sum
                    <
                            Ref<Var<1>>,
                            Ref<Var<1>>
                    >
            >::value::val == 2
            );
}