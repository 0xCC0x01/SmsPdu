#ifndef _SMS_SUBMIT_H_
#define _SMS_SUBMIT_H_

#include "SmsAddress.h"
#include "FirstOctet.h"
#include "SmsMr.h"
#include "SmsPid.h"
#include "SmsDcs.h"
#include "SmsVp.h"
#include "SmsUd.h"

class SmsSubmit
{
public:
    SmsAddress SMSC;
    FirstOctet FO;
    SmsMr TP_MR;
    SmsAddress TP_DA;
    SmsPid TP_PID;
    SmsDcs TP_DCS;
    SmsVp TP_VP;
    SmsUd TP_UD;

public:
    SmsSubmit();
    SmsSubmit(string da, wstring ud, SMS_CHARACTER_SET charSet = CHARACTER_SET_UCS2);

    string format(unsigned char segIndex);
    string detailInfo(unsigned char segIndex);
};

#endif /* _SMS_SUBMIT_H_ */