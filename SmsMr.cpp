#include "SmsMr.h"

SmsMr::SmsMr()
{
    this->mr = 0;
}

void SmsMr::setMr(unsigned char mr)
{
    this->mr = mr;
}

string SmsMr::format()
{
    return I2H(mr);
}