#ifndef _SMS_UD_H_
#define _SMS_UD_H_

#include "Util.h"

#include <vector>
using std::vector;

#define SMS_UD_MAX_LEN        (160)
#define SMS_IED_MAX_LEN       (10)


enum SMS_CHARACTER_SET
{
    CHARACTER_SET_GSM7 = 0,
    CHARACTER_SET_8BIT = 1,
    CHARACTER_SET_UCS2 = 2
};

/* Information Elements Identifier */
enum SMS_IEI
{
    /* Short messages concatenated to form a longer message: reference number 8 bit */
    IEI_CONCATENATED_8BIT = 0x00,
    /* Short messages indicated number and type of message waiting */
    IEI_SPECIAL_MESSAGE_INDICATION = 0x01,
    /* Short messages routed to one of multiple applications: port address 8 bit */
    IEI_APPLICATION_PORT_8BIT = 0x04,
    /* Short messages routed to one of multiple applications: port address 16 bit */
    IEI_APPLICATION_PORT_16BIT = 0x05,
    /* Short messages concatenated to form a longer message: reference number 16 bit */
    IEI_CONCATENATED_16BIT = 0x08
};

enum MESSAGE_INDICATION
{
    INDICATION_VOICE = 0,
    INDICATION_FAX = 1,
    INDICATION_EMAIL = 2,
    INDICATION_EXTENDED = 3
};

enum EXTENDED_MESSAGE_INDICATION
{
    EXTENDED_INDICATION_NO = 0,
    EXTENDED_INDICATION_VIDEO = 1
};

typedef struct
{
    /* Basic message indication type */
    unsigned char type:2;
    /* Extended message indication type */
    unsigned char extendedType:3;
    /* Profile ID of the Multiple Subscriber Profile */
    unsigned char profileId:2;
    /* Message shall be stored or not(0: discard, 1: store) */
    unsigned char store:1;
}IndicationHeader;

/* Information Element */
typedef struct
{
    /* Information Elements Identifier */
    unsigned char iei;
    /* Information Elements Data Length */
    unsigned char iedl;
    /* Information Elements Data */
    unsigned char ied[SMS_IED_MAX_LEN + 1];
}SmsUdhIe;

/* User Data Header */
typedef struct
{
    /* User Data Header Length */
    unsigned char TP_UDHL;
    vector <SmsUdhIe> ie;
}SmsUdh;

typedef struct
{
    /* User Data Length */
    unsigned char TP_UDL;
    /* User Data Header */
    SmsUdh UDH;
    unsigned char encodedLen;
    char content[SMS_UD_MAX_LEN + 1];
}SmsSeg;

/* User Data */
class SmsUd
{
private:
    vector <SmsSeg> segs;
    SmsUdh origUdh;

public:
    SmsUd();

    /* Get segment count of concatenated SMS */
    unsigned char getSegCount();

    /* Add an application port IE to UDH (addressing 8 bit or 16 bit) */
    void setPortUdh(int origPort, int destPort, bool use8bit = true);

    /* Add a special message indication IE to UDH */
    void setIndicationUdh(unsigned char indicationType, unsigned char extendedType, unsigned char messageCount, unsigned char profileId = 0, bool store = true);

    /* Add an IE to UDH */
    void setUdh(SmsUdhIe ie, int pos = -1);

    /* Set User data content */
    void setUd(wstring ud, SMS_CHARACTER_SET charSet, int refType = IEI_CONCATENATED_8BIT, int refNum = 0);

    string format(unsigned char segIndex);
};

#endif /* _SMS_UD_H_ */