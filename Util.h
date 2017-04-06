#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
using std::string;
using std::wstring;


/* Convert a digit (0~15) to a hex char */
char D2H(unsigned char n);

/* Convert integer(0~255) to 2 hex chars, eg. 86->"56" */
string I2H(unsigned char n);

/* Convert integer(double-digit, 0~99) to 2 reversed decimal chars, eg. 86->"68" */
string I2R(unsigned char n);

#endif /* _UTIL_H_ */