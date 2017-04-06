#ifndef _SMS_PID_H_
#define _SMS_PID_H_

#include "Util.h"

enum SMS_PID
{
    PID_NORMAL = 0,
    PID_NETWORK = 1,
    PID_RESERVED = 2,
    PID_SC_SPECIFIC = 3
};

/* Protocal Indentifier */
class SmsPid
{
private:
    unsigned char value:6;
    unsigned char subGroup:2;

public:
    SmsPid();

    /* Set TP-PID */
    void setPid(unsigned char subGroup, unsigned char value, bool interworking = false);

    string format();
};

#endif /* _SMS_PID_H_ */