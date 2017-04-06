#ifndef _SMS_ADDRESS_H_
#define _SMS_ADDRESS_H_

#include "Util.h"

typedef struct
{
    /* Numbering plan identification */
    unsigned char npi:4;
    /* Type of number */
    unsigned char ton:3;
    /* reserved and set to 1 */
    unsigned char reserved:1;
}AddressType;

/* SmsAddress: SMSC or TP-DA/TP-OA */
class SmsAddress
{
private:
    /* Address length */
    unsigned char len;
    /* Type of Address */
    AddressType type;
    /* Address value */
    string address;

public:
    SmsAddress();

    /* Set Address type */
    void setType(unsigned char npi, unsigned char ton);

    /* Set Address value */
    void setAddress(string addr, bool smsc = false);

    string format();
};

#endif /* _SMS_ADDRESS_H_ */