#include "SmsVp.h"

SmsVp::SmsVp()
{
    vp = "";
}

void SmsVp::setVp(unsigned char vpR)
{
    vp = "";
    vp += I2H(vpR);
}

void SmsVp::setVp(SmsVpE vpE)
{
    vp = "";

    vp += I2H(vpE.vpf|(vpE.singleShot<<6));

    if (VPF_ENHANCED_NONE == vpE.vpf || VPF_ENHANCED_RESERVED == vpE.vpf)
    {
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
    }
    else if (VPF_ENHANCED_RELATIVE == vpE.vpf || VPF_ENHANCED_RELATIVE_S == vpE.vpf)
    {
        vp += I2H(vpE.data[0]);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
    }
    else if (VPF_ENHANCED_RELATIVE_HMS == vpE.vpf)
    {
        vp += I2R(vpE.data[0]);
        vp += I2R(vpE.data[1]);
        vp += I2R(vpE.data[2]);
        vp += I2H(0);
        vp += I2H(0);
        vp += I2H(0);
    }
}

void SmsVp::setVp(SmsVpA vpA)
{
    vp = "";
    vp += I2R(vpA.year);
    vp += I2R(vpA.month);
    vp += I2R(vpA.day);
    vp += I2R(vpA.hour);
    vp += I2R(vpA.minute);
    vp += I2R(vpA.second);

    /*
     * Timezone indicates the difference between the local time and GMT
     * expressed in quarters of an hour, so timezone *4
     */
    int timezone = (vpA.timezone >= 0) ? 4*vpA.timezone: 4*(-vpA.timezone);
    vp += I2R(timezone);

    if (vpA.timezone < 0)
    {
		/* bit 3 represents the algebraic sign of this difference (0: positive, 1: negative) */
        vp[13] = D2H((timezone/10) | 0x08);
    }
}

string SmsVp::format()
{
    return vp;
}