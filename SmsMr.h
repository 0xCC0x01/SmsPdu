#ifndef _SMS_MR_H_
#define _SMS_MR_H_

#include "Util.h"

/* Message Reference */
class SmsMr
{
private:
    unsigned mr;

public:
    SmsMr();

    /* Set TP-MR */
    void setMr(unsigned char mr);

    string format();
};

#endif /* _SMS_MR_H_ */