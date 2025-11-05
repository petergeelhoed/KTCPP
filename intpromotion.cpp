#include <cassert>
#include <climits>
#include <cstdio>

extern const int SIZE = 5;
extern const int CENT = 100;
const long longSIZE = SIZE;
const long longCENT = 100;
int verbose = 0;

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

    // 100.0 is a double literal
    double tmp0 = tmp1 * 100.0;

    // for 7/5 = 1.39999998 will be cast to 139
    int res = (int)(tmp0);

    // better would be to add 0.5 for positive tmp0
    // and subtract for negative
    // that would then still fail for 671088/5 as the representation error would
    // be 13421759.375000 and not the 1342176 expected.
    // And that will no be fixed by +0.5

    double corr = (tmp1 >= 0) ? 0.5 : -0.5;
    int rescorr = (int)(tmp0 + corr);
    if (verbose)
        printf("float %.6f / %.6f  =  %.8f\ndouble:%.15f uncorrected: %d "
               "corrected:%d\n",
               tmp3,
               tmp2,
               tmp1,
               tmp0,
               res,
               rescorr);

    return res;
}

// So this is how you should do it:
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
    verbose = (argc > 1);
    int i = 1;

    while (i < INT_MAX / CENT)
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
        i = (i < 32) ? i + 1 : i * 2;
    }
    return 0;
}
