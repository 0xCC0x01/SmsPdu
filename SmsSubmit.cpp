#include "SmsSubmit.h"

SmsSubmit::SmsSubmit()
{
}

SmsSubmit::SmsSubmit(string da, wstring ud, SMS_CHARACTER_SET charSet)
{
    TP_DA.setAddress(da, false);
    TP_DCS.setDcs(charSet);
    TP_UD.setUd(ud, charSet);
}

string SmsSubmit::format(unsigned char segIndex)
{
    string format = "";

    format += SMSC.format();
    format += FO.format();
    format += TP_MR.format();
    format += TP_DA.format();
    format += TP_PID.format();
    format += TP_DCS.format();
    format += TP_VP.format();
    format += TP_UD.format(segIndex);

    return format;
}

string SmsSubmit::detailInfo(unsigned char segIndex)
{
    string detail = "";

    detail += "SMSC: " + SMSC.format() + "\n";
    detail += "FO: " + FO.format() + "\n";
    detail += "TP-MR: " + TP_MR.format() + "\n";
    detail += "TP-DA: " + TP_DA.format() + "\n";
    detail += "TP-PID: " + TP_PID.format() + "\n";
    detail += "TP-DCS: " + TP_DCS.format() + "\n";
    detail += "TP-VP: " + TP_VP.format() + "\n";
    detail += "TP-UD: " + TP_UD.format(segIndex);

    return detail;
}