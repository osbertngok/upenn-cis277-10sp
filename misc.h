#ifndef _MISC_H_
#define _MISC_H_
#include <sstream>

template <class T>
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}

#endif
