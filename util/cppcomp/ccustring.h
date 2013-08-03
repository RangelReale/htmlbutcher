#ifndef __CCUSTRING_H__
#define __CCUSTRING_H__

#include <string>

using namespace std;

namespace cppcomp
{

enum numberformat_t { NF_THOUSANDS = 1 };


string ccustring_format(const string format, ...);
string ccustring_numberformat(long number, int flags);
string ccustring_trim(string str);

};

#endif //__CCUSTRING_H__
