#include "fibo_playground.h"
#include <cstdint>
#include <iostream>

int main() {
    // Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
    static_assert(59 == Fibin<uint64_t>::eval<Invoke<Lambda<Var("x"),
                  Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>>>, Lit<Fib<3>>>>());

    // Testing: if False then Fib(0) else Fib(1)
    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());

    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval<Let<Var("zz"), Lit<Fib<0>>, Inc1<Ref<Var("zZ")>>>>());

    static_assert(2 == Fibin<int>::eval<Lit<Fib<3>>>());

//    std::cout << Fibin<int>::eval<Lit<Fib<-4294967290>>>() << "\n";

    static_assert(1  == Fibin<int>::eval<
            Let
            <
                Var("X"),
                Lit<Fib<0>>,
                Invoke
                <
                    Lambda
                    <
                        Var("X"),
                        Ref<Var("X")>
                    >,
                    Lit<Fib<1>>
                >
            >
    >());

    // Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char*>::eval<Lit<Fib<0>>>();
    Fibin<double>::eval<Lit<Fib<0>>>();
    Fibin<void>::eval<Lit<Fib<0>>>();
    Fibin<float>::eval<Lit<Fib<0>>>();

    static_assert(1  == Fibin<int>::eval<
        Let
        <
            Var("f"),

            Lambda
            <
                Var("x"),
                Inc1<Ref<Var("x")>>
            >,

            Invoke
            <
                Ref<Var("f")>,
                Lit<Fib<0>>
            >
        >
    >());

    std::cout << "Powinno byc 1:\n";
    std::cout <<
    Fibin<int>::eval<
            Let<
                    Var("const"),
                    Lit<Fib<1>>,
                    Let<
                            Var("f"),
                            Lambda<
                                    Var("x"),
                                    Sum<
                                            Ref<Var("const")>,
                                            Ref<Var("x")>
                                    >
                            >,
                            Let<
                                    Var("const"),
                                    Lit<Fib<3>>,
                                    Invoke<
                                            Ref<Var("f")>,
                                            Lit<Fib<0>>
                                    >
                            >
                    >
            >
    >() << "\n";
/*
    static_assert(1  == Fibin<int>::eval<
            Let<
                    Var("const"),
                    Lit<Fib<1>>,
                    Let<
                            Var("f"),
                            Lambda<
                                    Var("x"),
                                    Sum<
                                            Ref<Var("const")>,
                                            Ref<Var("x")>
                                    >
                            >,
                            Let<
                                    Var("const"),
                                    Lit<Fib<3>>,
                                    Invoke<
                                            Ref<Var("f")>,
                                            Lit<Fib<0>>
                                    >
                            >
                    >
            >
    >());


*/


    std::cout << "Fibin works fine!" << std::endl;
}
