#include <stdio.h>
#include "SmsSubmit.h"

void test00()
{
    SmsSubmit pdu =  SmsSubmit("86112233445566", L"Simple constructor", CHARACTER_SET_GSM7);

    string result = "0001000E8068112233445566000012D3741BCE2E83C66FF79C2EAF8FE96F39";

    printf("********** PDU TEST 00 **********\n");
    for (int i = 0; i < pdu.TP_UD.getSegCount(); i++)
    {
        printf("PDU(%d/%d): %s\n%s\n\n", i+1, pdu.TP_UD.getSegCount(), (pdu.format(i).compare(result) == 0) ? "Correct":"Error", pdu.detailInfo(i).data());
    }
}

void test01()
{
    SmsSubmit pdu;

    pdu.SMSC.setType(1, 1);
    pdu.SMSC.setAddress("+1234567", 1);

    pdu.FO.setRd(true);
    pdu.FO.setSrr(true);
    pdu.FO.setVpf(VPF_ENHANCED);

    pdu.TP_MR.setMr(135);

    pdu.TP_DA.setType(0, 1);
    pdu.TP_DA.setAddress("8698765432101");

    pdu.TP_PID.setPid(1, 0);

    pdu.TP_DCS.setDcs(0);

    SmsVpE vpE = {0};
    vpE.vpf = 0;
    pdu.TP_VP.setVp(vpE);

    wstring ud = L"Hello@World!";
    pdu.TP_UD.setUd(ud, CHARACTER_SET_GSM7);

    string result = "0591214365F72D870D90688967452301F14000000000000000000CC8329BFD065CDF72363904";

    printf("********** PDU TEST 01 **********\n");
    for (int i = 0; i < pdu.TP_UD.getSegCount(); i++)
    {
        printf("PDU(%d/%d): %s\n%s\n\n", i+1, pdu.TP_UD.getSegCount(), (pdu.format(i).compare(result) == 0) ? "Correct":"Error", pdu.detailInfo(i).data());
    }
}

void test02()
{
    SmsSubmit pdu;

    pdu.FO.setSrr(true);
    pdu.FO.setRp(true);
    pdu.FO.setUdhi(true);
    pdu.FO.setVpf(VPF_ABSOLUTE);

    pdu.TP_MR.setMr(234);

    pdu.TP_DA.setType(1, 2);
    pdu.TP_DA.setAddress("8698765432101");

    pdu.TP_PID.setPid(0, 2, true);

    pdu.TP_DCS.setDcs(1);

    SmsVpA vpA;
    vpA.year = 17;
    vpA.month = 3;
    vpA.day = 29;
    vpA.hour = 9;
    vpA.minute = 48;
    vpA.second = 20;
    vpA.timezone = -8;
    pdu.TP_VP.setVp(vpA);

    wstring ud = L"Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]Hello World![@]";

    pdu.TP_UD.setPortUdh(191, 16, true);

    pdu.TP_UD.setUd(ud, CHARACTER_SET_8BIT);

    string result[2] = 
    {
        "00F9EA0DA1688967452301F122047130929084022B8C090003000201040210BF48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C",
        "00F9EA0DA1688967452301F122047130929084022B2D090003000202040210BF64215B405D48656C6C6F20576F726C64215B405D48656C6C6F20576F726C64215B405D"
    };

    printf("********** PDU TEST 02 **********\n");
    for (int i = 0; i < pdu.TP_UD.getSegCount(); i++)
    {
        printf("PDU(%d/%d): %s\n%s\n\n", i+1, pdu.TP_UD.getSegCount(), (pdu.format(i).compare(result[i]) == 0) ? "Correct":"Error", pdu.detailInfo(i).data());
    }
}

void test03()
{
    SmsSubmit pdu;

    pdu.FO.setUdhi(true);

    pdu.TP_MR.setMr(255);

    pdu.TP_DA.setAddress("8698765432101");

    pdu.TP_PID.setPid(PID_SC_SPECIFIC, 37);

    pdu.TP_DCS.setDcs(2, 1, true, false, false);

    wstring ud = L"ÌìµØÐþ»Æ ÓîÖæºé»Ä ÈÕÔÂÓ¯ê¾ ³½ËÞÁÐÕÅ º®À´ÊîÍù ÇïÊÕ¶¬²Ø ÈòÓà³ÉËê ÂÉÂÀµ÷Ñô ÔÆÌÚÖÂÓê Â¶½áÎªËª ½ðÉúÀöË® Óñ³öÀ¥¸Ô ½£ºÅ¾ÞãÚ Öé³ÆÒ¹¹â ¹ûÕäÀîèÍ ²ËÖØ½æ½ª º£ÏÌºÓµ­ ÁÛÇ±ÓðÏè ÁúÊ¦»ðµÛ Äñ¹ÙÈË»Ê Ê¼ÖÆÎÄ×Ö ÄË·þÒÂÉÑ ÍÆÎ»ÈÃ¹ú ÓÐÓÝÌÕÌÆ";

    pdu.TP_UD.setIndicationUdh(INDICATION_FAX, 0, 15);

    pdu.TP_UD.setPortUdh(1601, 395, false);

    pdu.TP_UD.setUd(ud, CHARACTER_SET_UCS2);

    string result[2] = 
    {
        "0041FF0D80688967452301F1E5198C0F00030002010102810F0504018B06415929573073849EC400205B875B996D2A8352002065E5670876C8660300208FB05BBF52175F2000205BD2676566915F80002079CB653651AC85CF002095F04F5962105C8100205F8B54158C03963300204E91817E81F496E8002097327ED34E3A971C002091D1751F4E3D6C340020738951FA660651880020525153F7",
        "0041FF0D80688967452301F1E519820F00030002020102810F0504018B06415DE89619002073E079F0591C51490020679C73CD674E67F0002083DC91CD82A559DC00206D7754B86CB36DE100209CDE6F5C7FBD7FD400209F995E08706B5E1D00209E1F5B984EBA7687002059CB523665875B5700204E43670D886388F3002063A84F4D8BA956FD00206709865E96765510"
    };

    printf("********** PDU TEST 03 **********\n");
    for (int i = 0; i < pdu.TP_UD.getSegCount(); i++)
    {
        printf("PDU(%d/%d): %s\n%s\n\n", i+1, pdu.TP_UD.getSegCount(), (pdu.format(i).compare(result[i]) == 0) ? "Correct":"Error", pdu.detailInfo(i).data());
    }
}

int main()
{
    test00();
    test01();
    test02();
    test03();

    getchar();

    return 0;
}
