#ifndef UTILS_H
#define UTILS_H


#if defined(WIN32)
#include <sys/timeb.h>
#include <time.h>
#else
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "agro_types.h"

#include <list>
#include <string>

template <typename T>
inline T aAbs(const T &t) { return t >= 0 ? t : -t; }

// Возвращает true, если a==b с точностью до eps
bool double_equal(double a, double b, double eps);

// Возвращает true, если a==b с точностью до eps
bool float_equal(float a, float b, float eps);

extern bool dirExists(const char* path);
extern bool fileExists(const char* name);
extern int  fileSize(const char* name);
extern bool process_exists(const char* proc);   // Возвращает true, если указанный процесс существует в системе
extern void msleep(int msec);                   // пауза в мсек
extern std::string extractFileName(const std::string &s);

extern  const   char*   varAgro;
extern  const   char*   varNavigOpen;
extern  const   char*   var_to_agro;
extern  const   char*   var_to_navig;
extern  const   char*   var_to_client;
extern  const   char*   var_to_start;

extern  int64_t copyFile(const char* nameSrc, const char* nameDst);
extern  std::list<std::string> GetDirectories(const char* path);

extern  double  km2h_To_m2s(double km2h);    // Пересчет скорости км/час в м/сек
extern  double  m2s_To_km2h(double m2s);     // Пересчет скорости м/сек в км/час

extern  int     util_split(char* p, char delim, char** a, int max_count, bool oneDelim = true);
extern  void    trim_end(char* str);

extern  void    agroClient_Unload();
extern  void    agroClient_Load();
extern  int     getRamFree();       // Возвращает размер свободной памяти ОЗУ в килобайтах

extern  int     process_found(const char* process);     // Возвращает 1, если в системе загружен процесс с именем process
extern  int     process_Unload(const char* name);

extern  std::string int_toString(int value, int base);
extern  std::string float_toString(float value, int precision);
extern  std::string double_toString(double value, int precision);

extern  std::string SowNormaToString(double normaSet);

#endif // UTILS_H
