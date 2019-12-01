#ifndef _FIBIN_
#define _FIBIN_

template <typename T>
class Fibin {

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

    constexpr typename Var(char* s) {
        return verify(s, 0) ? int : std::invalid
    }

    struct Logical<bool val>{};
    struct Numeric<T val>{};

    using True = Logical<true>;
    using False = Logical<false>;

    /* Literały Lit
    Literałami liczbowymi mogą być tylko liczby Fibonacciego, np. Fib<0> =  0,  Fib<1> = 1 itd.
    Parametry Fib muszą być nieujemne.
    Literałami logicznymi mogą być tylko dwie wartości logiczne: True, False.
    Przykłady poprawnych literałów: Lit<Fib<0>>, Lit<True>.*/
    template<int I> class Fib<I>{}

    template<Fib<I>> class Numeric

    /* Zmienne Var
    Var(const char*) - identyfikator zmiennej; identyfikatory tworzone są na podstawie
    ciągów znaków o długości od 1 do 6 włącznie, zawierających małe i wielkie litery
    alfabetu angielskiego (a-zA-Z) oraz cyfry (0-9); małe i wielkie litery nie są
    rozróżniane.
    Przykłady poprawnych zmiennych: Var("A"), Var("01234"), Var("Cdefg").*/

    template<class name> struct Var;

    template<> struct Var<char*>
    {

    };

    typename Var(char*)
    {

    }

    /* Operacje arytmetyczne Sum, Inc1, Inc10
    Sum<...> - operacja dodawania wielu liczb, wymagane są co najmniej dwa argumenty.
    Inc1<Arg> - specjalizacja dodawania, która zwiększa wartość Arg o Fib<1>.
    Inc10<Arg> - specjalizacja dodawania, która zwiększa wartość Arg o Fib<10>.
    Przykłady poprawnych operacji: Sum<Lit<Fib<0>>, Lit<Fib<1>>, Lit<Fib<3>>>,
    Inc1<Lit<Fib<0>>>.*/

    /* Porównanie Eq
    Eq<Left, Right> - porównuje wartość Left z Right; zwraca True, gdy są równe,
    a False w przeciwnym przypadku.
    Przykład poprawnego porównania: Eq<Lit<Fib<0>>, Lit<Fib<1>>>.*/

    

    /* Odwołanie do zmiennej Ref
    Ref<Var> - zwraca wartość zmiennej identyfikowanej przez Var.
    Przykład poprawnego odwołania do zmiennej: Ref<Var("A")>.*/

    template<class V> struct Ref;

    template<> struct Ref<Var<T>>
    {
        using type = T
    };

    /* Wyrażenie Let
    Let<Var, Value, Expression> - do zmiennej Var przypisuje wartość Value
    i oblicza wartość Expression.
    Przykład poprawnego wyrażenia: Let<Var("A"), Lit<Fib<1>>, Ref<Var("A")>>.*/

    template<class Var, class Val, class Exp> struct Let;

    template<class Var, class Val>
        struct Let<Var<T>, Val<T>, Exp<

    /* Wyrażenie If
    If<Condition, Then, Else> - jeśli Condition jest True, to oblicza wartość Then,
    a w przeciwnym przypadku oblicza wartość Else.
    Przykład poprawnego wyrażenia: If<Lit<True>, Lit<Fib<1>>, Lit<Fib<0>>>.*/

    template<class C, class T, class F> struct If;

    template<class T, class F> struct If<

    /* Wyrażenie Lambda
    Lambda<Var, Body> - reprezentuje anonimową funkcję z jednym parametrem Var
    oraz ciałem Body.
    Przykład poprawnego wyrażenia: Lambda<Var("x"), Ref<Var("x")>>.*/

    /* Wywołanie funkcji
    Invoke<Fun, Param> - oblicza wartość funkcji Fun dla parametru Param.
    Przykład poprawnego wyrażenia: Invoke<Lambda<Var("x"), Ref<Var("x")>>, Lit<Fib<0>>>.*/


}

#endif //_FIBIN_
