#include "SmsUd.h"
#include <windows.h>

#define SEPTET_BITS_NUM		(7)
#define OCTET_BITS_NUM		(8)
#define UD_MAX_OCTETS		(140)
#define GSM7_ALPHBET_NUM	(128)


static int CEIL(int total, int each)
{
    return (total + each - 1)/each;
}

static const unsigned int Gsm7_Default_Alphbet[GSM7_ALPHBET_NUM] =
{
  '@', 0xa3,  '$', 0xa5, 0xe8, 0xe9, 0xf9, 0xec, 0xf2, 0xc7, '\n', 0xd8, 0xf8, '\r', 0xc5, 0xe5,
0x394,  '_',0x3a6,0x393,0x39b,0x3a9,0x3a0,0x3a8,0x3a3,0x398,0x39e,    0, 0xc6, 0xe6, 0xdf, 0xc9,
  ' ',  '!',  '"',  '#', 0xa4,  '%',  '&', '\'',  '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
 0xa1,  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z', 0xc4, 0xd6,0x147, 0xdc, 0xa7,
 0xbf,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
  'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z', 0xe4, 0xf6, 0xf1, 0xfc, 0xe0
};

static const unsigned int Gsm7_Extend_Alphbet[GSM7_ALPHBET_NUM] =
{
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,'\f',   0,   0,   0,   0,   0,
    0,   0,   0,   0, '^',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0, '{', '}',   0,   0,   0,   0,   0,'\\',
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, '[', '~', ']',   0,
  '|',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,0x20ac, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

static int DefaultGsm7(const unsigned int c)
{
    for (int n = 0; n < GSM7_ALPHBET_NUM; n++)
    {
        if (Gsm7_Default_Alphbet[n] == c)
        {
            return n;
        }
    }
    return -1;
}

static int ExtendGsm7(const unsigned int c)
{
    for (int n = 0; n < GSM7_ALPHBET_NUM; n++)
    {
        if (Gsm7_Extend_Alphbet[n] == c)
        {
            return n;
        }
    }
    return -1;
}

static bool CheckGsm7(const wstring ud)
{
    for (unsigned int i = 0; i < ud.length(); i++)
    {
        unsigned int c = ud[i];

        if (DefaultGsm7(c) >= 0)
        {
            continue;
        }

        if (ExtendGsm7(c) >= 0)
        {
            continue;
        }

        return false;
    }

    return true;
}

/*
 * Convert chars in the string to GSM 7 bit alphabet
 */
static string ConvertGsm7(const wstring ud)
{
    string content = "";

    for (unsigned int i = 0; i < ud.length(); i++)
    {
        unsigned int c = ud[i];

        /* For default alphabet chars: replace ACSII value with default alphabet value */
        int n = DefaultGsm7(c);
        if (n >= 0)
        {
            content += n;
            continue;
        }

        /* For extension alphabet chars: add an escape(0x1B) and replace ACSII value with extension alphabet value */
        n = ExtendGsm7(c);
        if (n >= 0)
        {
            content += 0x1B;
            content += n;
            continue;
        }
    }

    return content;
}

/*
 * Encode GSM 7 bit alphabet
 * Lower 7 bits of each chars to be concatenated to form new octets
 */
int EncodeGsm7(const string ud, int currLen, SmsSeg *currSeg)
{
    int fillBits = 0;
    int shiftBits = 0;
    int encodeLen = 0;
    unsigned int c1, c2;

    int udhl = currSeg->UDH.TP_UDHL;

    /*
     * 1) If 7 bit data is used and the TP-UD-Header does not finish on a septet boundary
     * then fill bits are inserted after the last Information Element Data octet up to the next septet boundary
     * This is to ensure that the SM itself starts on an septet boundary
     * 2) If 8 bit or UCS2 is used then fill bits are not necessary. The SM shall start on an octet boundary
     */
    if (udhl > 0)
    {
        int udh_bits_num = (udhl + 1)*OCTET_BITS_NUM;
        int septet_num = CEIL(udh_bits_num, SEPTET_BITS_NUM);

        fillBits = septet_num*SEPTET_BITS_NUM - udh_bits_num;
    }

    if (fillBits > 0)
    {
        currSeg->content[encodeLen++] = ud[0] << fillBits;
    }

    for (int i = 0; i < currLen; i++)
    {
        shiftBits = (7 + fillBits)%8;
        fillBits = shiftBits;

        if (shiftBits == 0)
        {
            continue;
        }

        c1 = (ud[i]>>(7 - shiftBits));

        if (i+1 < currLen)
        {
            c2 = (ud[i+1]<<shiftBits);
        }
        else
        {
            c2 = 0;
        }

        currSeg->content[encodeLen++] = (c1|c2);
    }

    return encodeLen;
}

/*
 * Convert wstring/UCS2 to string/ANSI
 */
static string Convert8Bit(const wstring ud)
{
    string content = "";

    int len = WideCharToMultiByte(CP_ACP, 0, ud.data(), -1, NULL, 0, NULL, NULL);
    char *ansi = new char[len + 1];
    memset(ansi, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, ud.c_str(), -1, (LPSTR)ansi, len, NULL, NULL);
    content = ansi;
    delete []ansi;

    return content;
}

/*
 * Convert wstring/UCS2 to string
 */
static string ConvertUcs2(const wstring ud)
{
    string content = "";

    for (unsigned int i = 0; i < ud.length(); i++)
    {
        content += (ud[i]&0xFF00)>>8;
        content += ud[i]&0x00FF;
    }

    return content;
}

SmsUd::SmsUd()
{
    origUdh.TP_UDHL = 0;
    setUd(L"", CHARACTER_SET_GSM7);
}

unsigned char SmsUd::getSegCount()
{
    return segs.size();
}

/*
 * Add an IE to UDH:
 * 1) push_back: default
 * 2) replace: with same iei
 * 3) insert: with different iei
 */
void SmsUd::setUdh(SmsUdhIe ie, int pos)
{
    if (pos < 0 || origUdh.ie.empty())
    {
        origUdh.ie.push_back(ie);
    }
    else if (pos < origUdh.ie.size())
    {
        if (origUdh.ie[pos].iei == ie.iei)
        {
            origUdh.ie[pos] = ie;
        }
        else
        {
            origUdh.ie.insert(origUdh.ie.begin() + pos, ie);
        }
    }

    /* Calculate TP-UDHL value */
    origUdh.TP_UDHL = 0;
    for (unsigned int i = 0; i < origUdh.ie.size(); i++)
    {
        /* UDHL = ied length(idel bytes) + iei(1 byte) + idel itself(1 byte) */
        origUdh.TP_UDHL += (origUdh.ie[i].iedl + 2);
    }
}

void SmsUd::setPortUdh(int origPort, int destPort, bool use8bit)
{
    SmsUdhIe ie = {0};

    if (use8bit)
    {
        ie.iei = IEI_APPLICATION_PORT_8BIT;
        ie.iedl = 2;
        ie.ied[0] = destPort;
        ie.ied[1] = origPort;
    }
    else
    {
        ie.iei = IEI_APPLICATION_PORT_16BIT;
        ie.iedl = 4;
        ie.ied[0] = destPort>>8;
        ie.ied[1] = destPort&0xFF;
        ie.ied[2] = origPort>>8;
        ie.ied[3] = origPort&0xFF;
    }

    setUdh(ie);
}

void SmsUd::setIndicationUdh(unsigned char indicationType, unsigned char extendedType, unsigned char messageCount, unsigned char profileId, bool store)
{
    SmsUdhIe ie = {0};
    IndicationHeader header = {0};

    header.type = (extendedType > 0) ? INDICATION_EXTENDED : indicationType;
    header.extendedType = extendedType;
    header.profileId = profileId;
    header.store = store;

    ie.iei = IEI_SPECIAL_MESSAGE_INDICATION;
    ie.iedl = 2;
    ie.ied[0] = *(int *)&header;
    ie.ied[1] = messageCount;

    setUdh(ie);
}

void SmsUd::setUd(wstring ud, SMS_CHARACTER_SET charSet, int refType, int refNum)
{
    int bits = 0;
    int segCount = 0;
    int udhLen = 0, maxTextLen = 0, currLen = 0, totalLen = 0;
    string content = "";

    /*
     * Original ud is encoded in UCS2/wstring, convert to multibyte/string first
     * Especially for GSM7 character set, need to convert ACSII char to GSM7 alphabet
     */
    if (CHARACTER_SET_GSM7 == charSet)
    {
        if (CheckGsm7(ud))
        {
            content = ConvertGsm7(ud);
            bits = 7;
        }
        else
        {
            /* Exist non Gsm7 character, set charSet to UCS2 */
            charSet = CHARACTER_SET_UCS2;
            bits = 16;
        }
    }
    else if (CHARACTER_SET_8BIT == charSet)
    {
        content = Convert8Bit(ud);
        bits = 8;
    }
    else
    {
        content = ConvertUcs2(ud);
        bits = 8;
    }

    totalLen = content.length();
    udhLen = (origUdh.TP_UDHL > 0) ? origUdh.TP_UDHL + 1 : 0;
    maxTextLen = (UD_MAX_OCTETS - udhLen)*OCTET_BITS_NUM/bits;

    /* More than one segment: concatenated sms */
    if (totalLen > maxTextLen)
    {
        int concatedLen = (IEI_CONCATENATED_8BIT == refType) ? 5 : 6;

        /* Update udhLen and maxTextLen when added concatenated ie */
        udhLen = origUdh.TP_UDHL + concatedLen + 1;
        maxTextLen = (UD_MAX_OCTETS - udhLen)*OCTET_BITS_NUM/bits;
    }

    segCount = CEIL(totalLen, maxTextLen);

    /* 1 segment for empty text */
    if (segCount == 0)
    {
        segCount = 1;
    }

    /* Max 255 segments */
    if (segCount > 0xFF)
    {
        segCount = 0xFF;
    }

    segs.clear();

    for (int segIndex = 0; segIndex < segCount; segIndex++)
    {
        SmsSeg currSeg = {0};

        /* Add concatenated ie in each segment */
        if (segCount > 1)
        {
            SmsUdhIe ie = {0};

            if (IEI_CONCATENATED_16BIT == refType)
            {
                ie.iei = IEI_CONCATENATED_16BIT;
                ie.iedl = 4;
                ie.ied[0] = refNum>>8;
                ie.ied[1] = refNum&0xFF;
                ie.ied[2] = segCount;
                ie.ied[3] = segIndex + 1;
            }
            else
            {
                ie.iei = IEI_CONCATENATED_8BIT;
                ie.iedl = 3;
                ie.ied[0] = refNum;
                ie.ied[1] = segCount;
                ie.ied[2] = segIndex + 1;
            }

            setUdh(ie, 0);
        }
        currSeg.UDH = origUdh;

        currLen = (totalLen > maxTextLen) ? maxTextLen : totalLen;

        if (CHARACTER_SET_GSM7 == charSet)
        {
            currSeg.TP_UDL = currLen + CEIL(udhLen*OCTET_BITS_NUM, SEPTET_BITS_NUM);
            currSeg.encodedLen = EncodeGsm7(content, currLen, &currSeg);
        }
        else
        {
            currSeg.TP_UDL = currLen + udhLen;
            currSeg.encodedLen = currLen;
            memcpy(currSeg.content, content.c_str(), currLen);
        }

        segs.push_back(currSeg);
        content.erase(0, currLen);
        totalLen -= currLen;
    }
}

string SmsUd::format(unsigned char segIndex)
{
    string format = "";

    if (segIndex < getSegCount())
    {
        SmsSeg currSeg = segs[segIndex];

        format += I2H(currSeg.TP_UDL);

        if (currSeg.UDH.TP_UDHL > 0)
        {
            format += I2H(currSeg.UDH.TP_UDHL);
            for (unsigned int i = 0; i < currSeg.UDH.ie.size(); i++)
            {
                format += I2H(currSeg.UDH.ie[i].iei);
                format += I2H(currSeg.UDH.ie[i].iedl);

                for (int j = 0; j < currSeg.UDH.ie[i].iedl; j++)
                {
                    format += I2H(currSeg.UDH.ie[i].ied[j]);
                }
            }
        }

        for (int i = 0; i < currSeg.encodedLen; i++)
        {
            format += I2H(currSeg.content[i]);
        }

        return format;
    }

    return "";
}