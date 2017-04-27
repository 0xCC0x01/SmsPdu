#ifndef _FIRST_OCTET_H_
#define _FIRST_OCTET_H_

#include "Util.h"

enum SMS_MTI
{
    /* SMS-DELIVER (in the direction SC to MS) */
    MTI_DELIVER = 0,
    /* SMS-DELIVER REPORT (in the direction MS to SC) */
    MTI_DELIVER_REPORT = 0,
    /* SMS-STATUS-REPORT (in the direction SC to MS) */
    MTI_SUBMIT = 1,
    /* SMS-COMMAND (in the direction MS to SC) */
    MTI_SUBMIT_REPORT = 1,
    /* SMS-SUBMIT (in the direction MS to SC) */
    MTI_STATUS_REPORT = 2,
    /* SMS-SUBMIT-REPORT (in the direction SC to MS) */
    MTI_COMMAND = 2,
    /* Reserved */
    MTI_RESERVED = 3
};

class FirstOctet
{
private:
    /* Message Type Indicator */
    unsigned char mti:2;
    /* Reject Duplicates */
    unsigned char rd:1;
    /* Validity Period Format */
    unsigned char vpf:2;
    /* Status Report Request */
    unsigned char srr:1;
    /* User Data Header Indicator */
    unsigned char udhi:1;
    /* Reply Path */
    unsigned char rp:1;

public:
    FirstOctet();

    /* Set TP-RD */
    void setRd(bool rd = true);

    /* Set TP-SRR */
    void setSrr(bool srr = true);

    /* Set TP-UDHI */
    void setUdhi(bool udhi = true);

    /* Set TP-RP */
    void setRp(bool rp = true);

    /* Set TP-MTI */
    void setMti(unsigned char mti);

    /* Set TP-VPF */
    void setVpf(unsigned char vpf);

    string format();
};

#endif /* _FIRST_OCTET_H_ */