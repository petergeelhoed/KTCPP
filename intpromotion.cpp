#include <cassert>
#include <climits>
#include <cstdio>
extern const int SIZE = 3;
extern const int CENT = 100;

int f(int value)
{
    auto result = (int)((float)value / (float)SIZE * 100.0);

    float tmp3 = (float)value;
    float tmp2 = (float)SIZE;

    // division of two floats gives a float
    float tmp1 = tmp3 / tmp2;

    // 100.0 is a double literal
    double tmp0 = tmp1 * 100.0;
    int res = (int)tmp0;
    return result;
}

int g(int value)
{
    // f() seems to calculate a percentage
    // this can also be achieved by integer calculation
    // if we multiply first with CENT

    // safeguarding against under and overflow
    assert(SIZE != 0);
    assert(value <= INT_MAX / CENT);
    assert(value >= INT_MIN / CENT);

    int result = CENT * value / SIZE;
    return result;
}

int main()
{
    int i = INT_MAX / CENT;

    while (i > 0)
    {
        long longi = i;
        long longSIZE = SIZE;
        long longCENT = 100;
        const char red[9] = "\033[31m";
        const char green[9] = "\033[32m";
        const char equal[3] = "==";
        const char inequal[3] = "!=";

        printf("val=%11d SIZE=%11d %sf(val)=%11d %s "
               "g(val)=%11d\033[0m ==> "
               "(calc as long)%ld\n",
               i,
               SIZE,
               (f(i) != g(i)) ? red : green,
               f(i),
               (f(i) != g(i)) ? inequal : equal,
               g(i),
               longCENT * longi / longSIZE);
        i /= 2;
    }
    return 0;
}
