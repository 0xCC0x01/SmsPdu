#include "SmsPid.h"

SmsPid::SmsPid()
{
    value = 0;
    subGroup = PID_NORMAL;
}

void SmsPid::setPid(unsigned char subGroup, unsigned char value, bool interworking)
{
    this->subGroup = subGroup;
    this->value = value;

    /*
     * In the case where bit 7 = 0 and bit 6 = 0
     * bit 5: indicates telematic interworking
     * 0 : no interworking, but SME-to-SME protocol
     * 1 : telematic interworking
     */
    if (PID_NORMAL == subGroup && interworking)
    {
        this->value |= 0x20;
    }
}

string SmsPid::format()
{
    unsigned char pid = (subGroup<<6)|value;

    return I2H(pid);
}