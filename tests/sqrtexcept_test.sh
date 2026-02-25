#!/bin/bash
exec="../sqrtexcept"
[[ $(echo 10 | $exec) != "sqrt(10.000000000000) = 3.162277660168" ]] && exit -1
[[ $(echo -10 | $exec 2>&1 ) != "Domain error: Square root of a negative number is undefined for real numbers." ]] && exit -1
[[ $(echo NaN | $exec 2>&1 ) != "Domain error: Input is NaN." ]] && exit -1
[[ $(echo 1e499 | $exec 2>&1 ) != "Range error: Input number is too large or too small. stod" ]] && exit -1
[[ $(echo inf | $exec 2>&1 ) != "Overflow error: Input is infinite." ]] && exit -1
exit 0

