#include "helper.h"

float
roundTwoDecimals(float number)
{
    return static_cast<float>(static_cast<int>(number * 100 + 0.5) / 100.0);
}
