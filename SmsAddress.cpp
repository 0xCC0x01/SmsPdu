#include "SmsAddress.h"

#define ADDRESS_TYPE_LEN    (1)


SmsAddress::SmsAddress()
{
    len = 0;
    type.npi = 0;
    type.ton = 0;
    type.reserved = 1;
    address = "";
}

void SmsAddress::setType(unsigned char npi, unsigned char ton)
{
    type.npi = npi;
    type.ton = ton;
}

void SmsAddress::setAddress(string addr, bool smsc)
{
    address = addr;

    /* ignore the beginning '+' */
    if (addr[0] == '+')
    {
        addr.erase(0, 1);
    }

    len = addr.length();

    /* for SMSC: len = type(1 octet) + address length in octet format */
    /* for TP-DA: len = address length in semi-octet format */
    if (smsc)
    {
        len = (len + 1)/2 + ADDRESS_TYPE_LEN;
    }
}

string SmsAddress::format()
{
    string format = "";
    int i = 0, curr_len = address.length();

    format += I2H(len);
    if (len > 0)
    {
        format += I2H(*(unsigned char*)&type);

        if (address[0] == '+')
        {
            i = 1;
        }

        for (; i < curr_len; i+=2)
        {
            if (i+1 < curr_len)
            {
                format += address[i+1];
            }
            else
            {
                format += 'F';
            }
            format += address[i];
        }
    }

    return format;
}