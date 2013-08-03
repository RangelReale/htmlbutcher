#include <cppcomp/ccustring.h>

#include <cstdio>
#include <cstdarg>

namespace cppcomp
{

string ccustring_format(const string format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    int asz=format.length()*5;
    char *rstr=new char[asz];

    int sz=vsnprintf(rstr, asz, format.c_str(), argptr);
    if (sz<0)
    {
        rstr[0]=0;
    }
    else if (sz>=asz)
    {
        delete [] rstr;
        rstr=new char[sz+1];
        sz=vsnprintf(rstr, sz+1, format.c_str(), argptr);
        if (sz<0)
            rstr[0]=0;
    }

    va_end(argptr);

    string ret(rstr);
    delete [] rstr;
    return ret;
}


string ccustring_numberformat(long number, int flags)
{
    if ((number != 0) && (flags && NF_THOUSANDS) == NF_THOUSANDS) {
        long curval=number;
        long curt;
        string ret="";
        do {
            curt=curval%1000;
            if (!ret.empty()) ret=","+ret;
            if (curt>=curval)
                ret=ccustring_format("%ld", curt)+ret;
            else
                ret=ccustring_format("%03ld", curt)+ret;
        } while ((curval=curval/1000)!=0);
        return ret;

    } else
        return ccustring_format("%d", number);
}

string ccustring_trim(string str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
  return str;
}

};
