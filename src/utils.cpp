#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "agro_types.h"
#include "utils.h"
#include "count_of.h"
#include "sys/stat.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <string>

#ifdef _EMBEDDED_
const char* varAgro =           "/var/agro";
const char* varNavigOpen =      "/var/agro/navig_open";
const char* var_to_agro =       "/var/agro/to_agro";
const char* var_to_navig =      "/var/agro/to_navig";
const char* var_to_client =     "/var/agro/to_client";
const char* var_to_start =      "/var/agro/to_start";
#else
const char* varAgro =           "/tmp/agro";
const char* varNavigOpen =      "/tmp/agro/navig_open";
const char* var_to_agro =       "/tmp/agro/to_agro";
const char* var_to_navig =      "/tmp/agro/to_navig";
const char* var_to_client =     "/tmp/agro/to_client";
const char* var_to_start =      "/tmp/agro/to_start";
#endif


// Возвращает true, если a==b с точностью до eps
bool double_equal(double a, double b, double eps)
{
    return aAbs(a - b) < eps ? true : false;
}

// Возвращает true, если a==b с точностью до eps
bool float_equal(float a, float b, float eps)
{
    return aAbs(a - b) < eps ? true : false;
}

static int IsNumeric(const char* ps)
{
    for ( ; *ps; ps++)
        if (*ps < '0' || *ps > '9')
            return 0; // false
    return 1; // true
}
//-------------------------------------------------------------------------
// Выполняет разбор строки по адресу p на подстроки через разделитель delim.
// Возвращает количество строк и в массиве str адреса строк
//-------------------------------------------------------------------------
//int str_parse(char* p, char delim, char* str[], int maxStr)
//{
//    int cnt = 0;
//    while (p && *p && cnt < maxStr) {
//        if (p) {
//            while (*p == delim)
//                *p++ = 0;
//        }
//        str[cnt++] = p;
//        p = strchr(p, delim);
//        if (p) {
//            while (*p == delim)
//                *p++ = 0;
//        }
//    }
//    return cnt;
//}

//=============================
bool dirExists(const char* path)
{
    struct stat sb;
    bool ok = (stat(path, &sb) == 0) ? 1 : 0;
    return ok;
}

bool fileExists(const char* name)
{
    FILE* fp = fopen(name, "r");
    if (!fp)
        return false;
    fclose(fp);
    return true;
}
int fileSize(const char* name)
{
    FILE* fp = fopen(name, "rb");
    if (!fp)
        return 0;
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

// Возвращает размер свободной памяти ОЗУ в килобайтах
int getRamFree()
{
    int free = 0;
    const char* nameFile = "/proc/meminfo";
    FILE* fp = fopen(nameFile, "r");
    if (fp) {
        int i = 0;
        char buf[64];
        while(fgets(buf,sizeof(buf),fp) != 0) {
            char* astr[8];
            switch (i) {
            case 0:
                break;
            case 1:
                if (util_split(buf, ' ', astr, COUNT_OF(astr)) >= 3) {
                    free = atoi(astr[1]);
                }
                break;
            }
            if (++i >= 2)
                break;
        }
        fclose(fp);
    }
    return free;
}

// Возвращает pid процесса, если в системе загружен процесс с именем process.
// Возвращает 0, если процесс не найден.
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-result"
int process_found(const char* process)
{
    int pid = 0;
    DIR* dir_proc = opendir("/proc/");
    if (dir_proc) {
        int len = strlen(process);
        struct dirent* dirEntity = 0;
        while ((dirEntity = readdir(dir_proc)) != 0) {
            if (dirEntity->d_type == DT_DIR) {
                if (IsNumeric(dirEntity->d_name)) {
                    char comm[64];
                    snprintf(comm, sizeof(comm), "/proc/%s/comm", dirEntity->d_name);
                    FILE* fp = fopen(comm, "rt");
                    if (!fp) {
                        printf("Error open file %s\n", comm);
                        break;
                    }
                    char *ps = fgets(comm, sizeof(comm), fp);
                    fclose(fp);
                    if (ps && !strncmp(ps, process, len)) {
                        pid = atoi(dirEntity->d_name);
                        break;
                    }
                }
            }
        }
        closedir(dir_proc);
    }
    return pid;
}
int process_Unload(const char* name)
{
    static const char fn[] = "process_Unload";
    printf("%s. %s\n", fn, name);
    int pid = process_found(name);  //"navig22");
    if (!pid) {
        printf("process NOT FOUND\n");
        return 0;
    }
    char    cmd[32];
    snprintf(cmd, sizeof(cmd), "kill %d", pid);
    puts(cmd);      // echo cmd
    system(cmd);
    for (int i = 0; i < 30 && pid; i++) {
        msleep(100);        // pause 100 msec
        pid = process_found(name);
    }
    if (pid) {
        snprintf(cmd, sizeof(cmd), "kill -9 %d", pid);
        puts(cmd);      // echo cmd
        system(cmd);
        msleep(100);        // pause 100 msec
    }
    return 1;
}
//#pragma GCC diagnostic pop

// пауза в мсек
void msleep(int msec)
{
    usleep(msec*1000);
}

std::string extractFileName(const std::string& s)
{
    int i = s.find_last_of('/');
    if (i == -1)
        i = s.find_last_of('\\');
    return i == -1 ? s : s.substr(i + 1);
}

// Возвращает список папок по указанному пути
std::list<std::string> GetDirectories(const char* path)
{
//    printf("GetDirectories %s\n", path);
    std::list<std::string> list;
    DIR* dir = opendir(path);
    if (!dir) {
        printf("Error opendir %s\n", path);
        return list;
    }
    struct dirent* entry;
    while ((entry=readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name,".")) {
            continue;
        }
        if (!strcmp(entry->d_name,"..")) {
            continue;
        }
        std::string fname(path); fname += "/"; fname += entry->d_name;
        struct stat st;
        if (stat(fname.c_str(), &st) == -1) {
            printf("Error stat %s\n", fname.c_str());
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
//            printf("dir: %s\n", fname.toAscii().data());
            list.push_back(fname);
        }
    }
    closedir(dir);
    return list;
}

static char cmd[1024];
//----------------------------------------------------------------
int64_t copyFile(const char* nameSrc, const char* nameDst)
{
    struct stat st;
    if(stat(nameSrc, &st) == -1) {
        return -1;  // error copy
    }
    int64_t sizeSrc = st.st_size;
    for(int i = 0; i < 5; i++) {
        snprintf(cmd, sizeof(cmd), "cp %s %s", nameSrc, nameDst);
        system(cmd);
        sync();
        int64_t sizeDst = 0;
        if(stat(nameSrc, &st) != -1) {
            sizeDst = st.st_size;
        }
        if (sizeDst == sizeSrc) {
            return sizeSrc;
        }
    }
    return -1;  // error copy
}

// Пересчет скорости км/час в м/сек
double km2h_To_m2s(double km2h) { return km2h * (qreal(1000.0) / qreal(3600.0)); }

// Пересчет скорости м/сек в км/час
double m2s_To_km2h(double m2s) {  return m2s * (qreal(3600.0) / qreal(1000.0)); }

std::string SowNormaToString(double normaSet)
{
    char    ss[32];
    snprintf(ss, sizeof(ss), "%.*f", normaSet < qreal(10) ? 1 : 0, normaSet);
    return ss;
}

// Разделить строку "p" на лексемы, если разделитель один символ.
// delim        -символ разделителя
// a            -массив адресов лексем
// max_count    -максимальное количество лексем
// oneDelim     -разделитель только в одном экземпляре
// Возвращает количество лексем.
int util_split(char* p, char delim, char** a, int max_count, bool oneDelim)
{
    int count = 0;
    while (count < max_count) {
        if (!oneDelim) {        // не один разделитель
            while (*p == delim) // пропускаем все разделители
                p++;
        }
        if (*p == 0)
            break;
        a[count++] = p; // начало параметра
        while (*p && *p != delim)
            p++;
        if (*p == 0)
            break;
        *p++ = 0;       // удалить разделитель
    }
    return count;
}

static  bool charForDel(char c)
{
    static  char dels[3] = { '\r', '\n', ' '};
    for (int i = 0; i < 3; i++) {
        if (c == dels[i]) {
            return true;
        }
    }
    return false;
}
void trim_end(char* str)
{
    int len = strlen(str);
    while (len > 0 && charForDel(str[len-1])) {
        str[len-1] = 0;
        len--;
    }
}

void agroClient_Unload()
{
    process_Unload("client22");
}
void agroClient_Load()
{
    char cmd[100];
    strcpy(cmd, "/home/user/client22");
    if (fileExists(cmd) == false) {
        printf("ERROR. Can't find %s\n", cmd);
        return;
    }
    if (process_found("client22")) {
        printf("ERROR. client22  ALREADY LOADED\n");
        return;
    }
    strcat(cmd, " &");
    puts(cmd);      // echo cmd
    system(cmd);
}

std::string int_toString(int value, int base)
{
    char buf[32];
    switch (base) {
    case 8:     snprintf(buf, sizeof(buf), "0%o",   value);     break;
    case 16:    snprintf(buf, sizeof(buf), "0x%X",  value);     break;
    default:    snprintf(buf, sizeof(buf), "%d",    value);     break;
    }
    return buf;
}

std::string float_toString(float value, int precision)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%.*f", precision, value);
    return buf;
}

std::string double_toString(double value, int precision)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%.*f", precision, value);
    return buf;
}
