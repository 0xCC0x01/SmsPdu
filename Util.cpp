#include "Util.h"

char D2H(unsigned char n)
{
    const char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    return hex[n];
}

string I2H(unsigned char n)
{
    string ret = "";

    ret += D2H(n>>4);
    ret += D2H(n&0x0F);
    return ret;
}

string I2R(unsigned char n)
{
    string ret = "";

    ret += D2H(n%10);
    ret += D2H(n/10);
    return ret;
}