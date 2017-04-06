#ifndef _SMS_DCS_H_
#define _SMS_DCS_H_

#include "Util.h"

/*
 * 00xxxxxx: General Data Coding indication
 * 01xxxxxx: Message Marked for Automatic Deletion Group
 */
typedef struct
{
    /* Message Class */
    unsigned char msgClass:2;
    /* Character set being used: GSM7, 8-bit or UCS2 */
    unsigned char charSet:2;
    /* bits 1 to 0 have message class meaning or not */
    unsigned char setClass:1;
    /* Text is uncompressed or not */
    unsigned char compressed:1;
    /* 00 or 01 */
    unsigned char subGroup:2;
}DcsGeneralCoding;

/*
 * 1000xxxx: Reserved coding group
 * 1001xxxx: Reserved coding group
 * 1010xxxx: Reserved coding group
 * 1011xxxx: Reserved coding group
 */

/*
 * Message Waiting Indication Group
 * 1100xxxx: Discard Message, same as Group 1101, except discard message after presenting an indication
 * 1101xxxx: Store Message, user data is coded in GSM7 default alphabet
 * 1110xxxx: Store Message, user data is coded in uncompressed UCS2 character set
 */
typedef struct
{
    /* Indication type */
    unsigned char type:2;
    /* reserved and set to 0 */
    unsigned char reserved:1;
    /* Indication active or inactive */
    unsigned char active:1;
    /* 1100~1110 */
    unsigned char subGroup:4;
}DcsMessageWaiting;

/*
 * 1111xxxx: Data coding/message class
 */
typedef struct
{
    /* Message Class */
    unsigned char msgClass:2;
    /* Character set being used: GSM7 or 8-bit */
    unsigned char charSet:1;
    /* reserved and set to 0 */
    unsigned char reserved:1;
    /* 1111 */
    unsigned char subGroup:4;
}DcsDataCoding;

/* Data Coding Scheme */
class SmsDcs
{
private:
    unsigned char dcs;

public:
    SmsDcs();

    /* Set TP-DCS (General Data Coding indication) */
    void setDcs(unsigned char charSet, unsigned char msgClass = 0, bool setClass = false, bool compressed = false, bool autoDelete = false);

    /* Set TP-DCS (Message Waiting Indication Group) */
    void setDcs(unsigned char type, bool active, bool Gsm7, bool autoDelete = false);

    /* Set TP-DCS (Data coding/message class) */
    void setDcs(unsigned char msgClass, bool Gsm7);

    string format();
};

#endif /* _SMS_DCS_H_ */

