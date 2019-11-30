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

    static_assert(Fibin<int>::Sum
    <
        Fibin<int>::Lit<Fibin<int>::Fib<0>>,
        Fibin<int>::Lit<Fibin<int>::Fib<1>>,
        Fibin<int>::Lit<Fibin<int>::Fib<2>>,
        Fibin<int>::Lit<Fibin<int>::Fib<3>>
    >::value::val == 4);

    static_assert(
        Fibin<int>::If<
        Fibin<int>::Eq
        <
            Fibin<int>::Sum
            <
                    Fibin<int>::Lit<Fibin<int>::Fib<4>>,
                    Fibin<int>::Lit<Fibin<int>::Fib<5>>
            >,
            Fibin<int>::Lit<Fibin<int>::Fib<6>>
        >,
        Fibin<int>::Lit<Fibin<int>::Fib<0>>,
        Fibin<int>::Lit<Fibin<int>::Fib<1>>
        >::value::val == 0);
}