#ifndef _SMS_DCS_H_
#define _SMS_DCS_H_

#include "Util.h"

enum SMS_DCS
{
    DCS_GENERAL_CODING = 0,
    DCS_MESSAGE_WAITING = 1,
    DCS_MESSAGE_CLASS = 2
};

enum MESSAGE_WAITING
{
    MESSAGE_WAITING_VOICE = 0,
    MESSAGE_WAITING_FAX = 1,
    MESSAGE_WAITING_EMAIL = 2,
    MESSAGE_WAITING_OTHER = 3
};

enum MESSAGE_CLASS
{
    MESSAGE_CLASS_0 = 0,
    /* Default meaning: ME-specific */    
    MESSAGE_CLASS_1 = 1,
    /* (U)SIM-specific message */
    MESSAGE_CLASS_2 = 2,
    /* Default meaning: TE specific */
    MESSAGE_CLASS_3 = 3
};

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
}DcsMessageClass;

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
    void setDcs(MESSAGE_WAITING type, bool active = true, bool Gsm7 = false, bool autoDelete = false);

    /* Set TP-DCS (Data coding/message class) */
    void setDcs(MESSAGE_CLASS msgClass, bool Gsm7 = false);

    string format();
};

#endif /* _SMS_DCS_H_ */

