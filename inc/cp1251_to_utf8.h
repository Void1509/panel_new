#ifndef CP1251_TO_UTF8_H
#define CP1251_TO_UTF8_H

#include <string>

char*       cp1251_to_utf8(const char *str, char* res, int size);
std::string std_cp1251_to_utf8(const char* ps);
std::string std_utf8_to_cp1251(const char* ps);


#endif // CP1251_TO_UTF8_H
