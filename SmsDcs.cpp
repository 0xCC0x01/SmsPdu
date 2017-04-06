#include "SmsDcs.h"

SmsDcs::SmsDcs()
{
    dcs = 0;
}

void SmsDcs::setDcs(unsigned char charSet, unsigned char msgClass, bool setClass, bool compressed, bool autoDelete)
{
    DcsGeneralCoding gc;
    gc.msgClass = msgClass;
    gc.charSet = charSet;
    gc.setClass = setClass;
    gc.compressed = compressed;

    if (autoDelete)
    {
        gc.subGroup = 0x01;
    }
    else
    {
        gc.subGroup = 0x00;
    }

    dcs = *(unsigned char *)&gc;
}

void SmsDcs::setDcs(unsigned char type, bool active, bool Gsm7, bool autoDelete)
{
    DcsMessageWaiting mw;
    mw.type = type;
    mw.active = active;
    mw.reserved = 0;

    if (autoDelete)
    {
        mw.subGroup = 0x0C;
    }
    else if (Gsm7)
    {
        mw.subGroup = 0x0D;
    }
    else
    {
        mw.subGroup = 0x0E;
    }

    dcs = *(unsigned char *)&mw;
}

void SmsDcs::setDcs(unsigned char msgClass, bool Gsm7)
{
    DcsDataCoding dc;
    dc.msgClass = msgClass;
    /* ture: GSM7 or false: 8-bit */
    dc.charSet = Gsm7;
    dc.reserved = 0;
    dc.subGroup = 0x0F;

    dcs = *(unsigned char *)&dc;
}

string SmsDcs::format()
{
    return I2H(dcs);
}