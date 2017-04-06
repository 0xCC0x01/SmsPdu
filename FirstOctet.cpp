#include "FirstOctet.h"

FirstOctet::FirstOctet()
{
    mti = MTI_SUBMIT;
    rd = 0;
    vpf = 0;
    srr = 0;
    udhi = 0;
    rp = 0;
}

void FirstOctet::setMti(unsigned char mti)
{
    this->mti = mti;
}

void FirstOctet::setRd(bool rd)
{
    this->rd = rd;
}

void FirstOctet::setVpf(unsigned char vpf)
{
    this->vpf = vpf;
}

void FirstOctet::setSrr(bool srr)
{
    this->srr = srr;
}

void FirstOctet::setUdhi(bool udhi)
{
    this->udhi = udhi;
}

void FirstOctet::setRp(bool rp)
{
    this->rp = rp;
}

string FirstOctet::format()
{
    unsigned char fo = mti|(rd<<2)|(vpf<<3)|(srr<<5)|(udhi<<6)|(rp<<7);

    return I2H(fo);
}