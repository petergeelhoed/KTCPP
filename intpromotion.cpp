#include <cassert>
#include <climits>
#include <cstdio>

extern const int SIZE = 5;
extern const int CENT = 100;
long longSIZE = SIZE;
long longCENT = 100;
int verbose;

const char red[9] = "\033[31m";
const char green[9] = "\033[32m";
const char equal[3] = "==";
const char inequal[3] = "!=";

int f(int value)
{
    auto result = (int)((float)value / (float)SIZE * 100.0);

    float tmp3 = (float)value;
    float tmp2 = (float)SIZE;

    // division of two floats gives a float
    // in the example of 7/5 this division will be
    // off at the 8'th digit.
    float tmp1 = tmp3 / tmp2;
    if (verbose)
        printf("float %.6f / %.6f  =  %.8f\n\n", tmp3, tmp2, tmp1);

    // 100.0 is a double literal
    double tmp0 = tmp1 * 100.0;

    // 1.39999998 will be cast to 139
    int res = (int)tmp0;
    return res;
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

int main(int argc, char** argv)
{
    verbose = (argc != 1);
    int i = 30;

    while (i > -30)
    {
        long longi = i;
        long li = longCENT * longi / longSIZE;

        int fi = f(i);
        int gi = g(i);
        bool good = (fi == gi);

        printf("val=%11d SIZE=%11d %sf(val)=%11d %s "
               "g(val)=%11d\033[0m ==> "
               "(calc as long)%ld\n",
               i,
               SIZE,
               good ? green : red,
               fi,
               good ? equal : inequal,
               gi,
               li);
        i -= 1;
    }
    return 0;
}
