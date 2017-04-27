#ifndef _SMS_VP_H_
#define _SMS_VP_H_

#include "Util.h"

enum SMS_VPF
{
    /* TP-VP field not present */
    VPF_NONE = 0,
    /* TP-VP field present - relative format */
    VPF_ENHANCED = 1,
    /* TP-VP field present - enhanced format */
    VPF_RELATIVE = 2,
    /* TP-VP field present - absolute format */
    VPF_ABSOLUTE = 3
};

enum SMS_VPF_ENHANCED
{
    /* No Validity Period specified */
    VPF_ENHANCED_NONE = 0x00,
    /* The following 1 octet contains the TP-VP value same as relative format */
    VPF_ENHANCED_RELATIVE = 0x01,
    /* The following 1 octet contains the TP-VP value representing seconds(0~255) */
    VPF_ENHANCED_RELATIVE_S = 0x02,
    /* The following 3 octets contain the relative time in Hours, Minutes and Seconds */
    VPF_ENHANCED_RELATIVE_HMS = 0x03,
    /* 4~7: All reserved */
    VPF_ENHANCED_RESERVED
};

/*
 * Absolute format
 * TP-VP comprises 7 octets giving the absolute time of the validity period termination
 */
typedef struct
{
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    char timezone;
}SmsVpA;

/*
 * Enhanced format
 * TP-VP comprises 7 octets, first octet indicates the way in which the following 6 octets are used
 */
typedef struct
{
    /* SMS_VPF_ENHANCED */
    unsigned char vpf:3;
    /* Reserved and set to 0 */
    unsigned char reserved:3;
    /* Set to 1 if the SC is required to make up to one delivery attempt */
    unsigned char singleShot:1;
    /* Set to 1 if the TP-VP functionality indicator is to be extended to another octet */
    unsigned char extend:1;
    unsigned char data[6];
}SmsVpE;

/* Valid Period */
class SmsVp
{
private:
    string vp;

public:
    SmsVp();

    /* Set TP-VP (Relative VPF) */
    void setVp(unsigned char vpR);

    /* Set TP-VP (Enhanced VPF) */
    void setVp(SmsVpE vpE);

    /* Set TP-VP (Absolute VPF) */
    void setVp(SmsVpA vpA);
    
    string format();
};

#endif /* _SMS_VP_H_ */