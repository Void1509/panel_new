#ifndef OPTIONS_H
#define OPTIONS_H

#include "agro_types.h"
#include "count_of.h"
#include "xml.h"

#include <string>

#define MODE_DEMO

const double cSpeed_Stop= qreal(1.0);   // Если скорость(км/час) ниже указанной, считаем, что агрегат стоит и можно делать ПОПРАВКУ координат перед началом движения
const double cSpeed_Fix = qreal(1.5);   // Если скорость(км/час) ниже указанной, считаем, что агрегат стоит и координату НЕ МЕНЯЕМ
const double cSpeed_0   = qreal(2.0);   // Если скорость(км/час) ниже указанной, считаем, что агрегат не движется (скорость 0 км/час).

const int   cDebug = 0;

const int   cBunkersMaxCount    = 3;        // Максимальное количество бункеров, режим  "Посевной комплекс"
const int   cTowersMaxCount     = 16;       // Максимальное количество башень  (до 6.2.2016 было 12)
const int   cSemensMaxCount     = 16;       // Максимальное количество датчиков зерна

const int   cSectionsMaxCount   = 6;        // Максимальное количество секций механической сеялки
const int   cSectionItemCount   = 4;        // Количество элементов в секции
const int   cValsSectionsMaxCount   = (cSectionsMaxCount*2);  // Максимальное количество валов в секциях, режим "Механические сеялки"
const int   cValsMaxCount           = cValsSectionsMaxCount;  // Максимальное количество валов дозатора
const int   cSectionBunkersMaxCount = (cSectionsMaxCount*2);  // Максимальное количество бункеров в секциях, режим "Механические сеялки"
const int   cValsMaxCountCmd    = 3;        // Максимальное количество валов дозатора в команде управления
const int   cFansMaxCount       = 2;        // Максимальное количество вентиляторов
const int   cMaxCountCulture    = 10;       // Максимальное количество культур для автоматической нормы высева
const int   cMaxSensorDepth     = 4;        // Максимальное количество датчиков глубины

const int   cMaxCountVolumeLiquid   = 3;    // Максимальное количество емкостей с жидкими удобрениями
const int   cMaxCountLiquid         = 10;   // Максимальное количество наименований жидких удобрений

const int   cMaxModules         = 3;        // Максимальное количество модулей  (зерна или удобрений, точный высев)
const int   cMaxSensors         = 12;       // Максимальное количество датчиков (зерна или удобрений, точный высев)
const int   cMaxSensorsTv2      = 24;       // Максимальное количество датчиков зерна (точный высев 2)

const int   cTimeOutMuteSec         = 5;    // При частых тревогах голос не чаще, чем данный тайм-аут (одновременно несколько тревог одного вида)
const int   cTimeOutRepeatLostSec   = 30;   // Если потеря устройства не уходит, надпоминать голосом через данный тайм-аут
const int   cTimeOutRepeatSec       = 180;  // old:60. Если тревога не уходит, четыре надпоминания голосом через данный тайм-аут
const int   cTimeOutRepeatSec5      = 300;  // Если тревога не уходит, пятое и далее надпоминание голосом через данный тайм-аут

const int   cTimeOutValStopSec      = 10;   // Таймаут голоса "останов вала дозатора"

const int   cCountConfirmAlarm      = 2;    // кол-во тревожных подряд идущих сообщений, чтобы объявить тревогу
const int   cCountConfirmAlarmSem   = 10;   // кол-во подряд идущих засорений, чтобы засорение показать на экране и зафиксировать в протоколе
const int   cCountConfirmAlarmSemTv = 3;    // old:10 (2020.03) кол-во подряд идущих засорений, чтобы засорение показать на экране и зафиксировать в протоколе
const int   cCountConfirmAlarmVal   = 10;   // кол-во подряд идущих остановов вала дозатора, чтобы останов показать на экране и зафиксировать в протоколе

const int   cMaskaRealTimeCoord1sec = 0x01; // передавать координаты раз в сек
const int   cMaskaRealTimeCoord2sec = 0x80; // передавать координаты раз в 2 сек
const int   cMaskaRealTimeCoord5sec = 0x02; // передавать координаты раз в 5 сек
const int   cMaskaRealTimeState     = 0x04; // передавать состояние
const int   cMaskaRealTimeValue     = 0x08; // передавать значение
const int   cMaskaRealTimeDepth     = 0x10; // передавать датчики глубины
const int   cMaskaRealTimeGisto     = 0x20; // передавать гистограмму
const int   cMaskaRealTimeOptions   = 0x40; // передавать настройки

const int   cIntervals = 10;            // Количество интервалов, на которое делим значение энкодера от максимума до минимума
const double cSowNormaMax = qreal(350); // Максимальная норма высева в кг   old:qreal(300) 2023.05.10

//==============
class Options
{
private:
    std::string diskData;
    std::string nameFile;           // имя файла хранения настроек
    std::string nameFileForSend;    // имя файла настроек для передачи на сервер
    DXMLObject* createXmlRoot();
    void    xmlObject_parse_SemensOff(      DXMLObject *ob);
    void    xmlObject_parse_SemensCropsOff( DXMLObject *ob);
    void    xmlObject_parse_SemensManureOff(DXMLObject *ob);
    void    xmlObject_parse(const char* name, const char* value);
    int     xml_Parse(char* ps, int size);
    int     xml_read();

public:
    enum Agro_Mode { AgroModeSow, AgroModeTv, AgroModeNavig1, AgroModeNavig2, AgroModeTv2, AgroModeSprayer, AgroModeCombine, AgroModeCombine2, AgroModeMechSow };
    // 0 -Посевной_комплекс, 1 -Точный_высев1, 2 -Точный_высев2, 3 -Навигатор1, 4-Навигатор2, 5 -Комбайн(датчики урожайности)
    enum ModeOptEdit { ModeNone, ModeCommon, ModeSpecial, ModeCounts, ModeSowNorma, ModeSowNormaOptions, ModeMenu, ModeDepth,
                       ModeLiquidOptions, ModeLiquidName0changed, ModeLiquidName1changed, ModeLiquidName2changed };
    enum Speed_Method {  SpeedNone=0, SpeedGPS=1, SpeedEncoder=2, SpeedFixing7=3 };
    enum Type_Work { TypeSow, TypePlough, TypeHarrow}; // сев, вспашка, боронование
    enum Scheme_Agregate {  Scheme_AgrCultBunk, Scheme_AgrBunkCult, Scheme_Agr, Scheme_SpraySelf, Scheme_SprayTrailer };
    enum SowNormaMode { SowNorma_Off, SowNorma_Base, SowNorma_Auto, SowNorma_Auto2 };
    enum SendRealTime { SendRealTime_off, SendRealTime_internet, SendRealTime_autograph };

    static const char* Speed_Method_ToString(int speedMethod)
    {
        if (speedMethod == Options::SpeedGPS)       return "GPS";
        if (speedMethod == Options::SpeedEncoder)   return "Encoder";
        if (speedMethod == Options::SpeedFixing7)   return "Fixing7";
        return "None";
    }
    static const char* NameFileXml()                    {   return "agro22.xml";            }
    std::string NameFile()                              {   return nameFile;                }   // полное имя файла хранения настроек
    std::string NameFileForSend()                       {   return nameFileForSend;         }   // полное имя файла для передачи на сервер
    void    setNameFile(const std::string& name)        {   this->nameFile = name;          }   // установить полное имя файла хранения настроек
    void    setNameFileForSend(const std::string& name) {   this->nameFileForSend = name;   }   // установить полное имя файла для передачи на сервер

    int     AgroMode;
    int     AgroDemo;       // Режим "Демонстрация с использованием симулятора"
    bool AgroSow()      {   return AgroMode == AgroModeSow;     }   // Режим "Посевной комплекс"
    bool AgroTv()       {   return AgroMode == AgroModeTv;      }   // Режим "Точный высев"
    bool AgroTv2()      {   return AgroMode == AgroModeTv2;     }   // Режим "Точный высев2"
    bool AgroNavig1()   {   return AgroMode == AgroModeNavig1;  }   // Режим "Навигатор 1"
    bool AgroNavig2()   {   return AgroMode == AgroModeNavig2;  }   // Режим "Навигатор 2"  (Опрыскиватель по старому)
    bool AgroMechSow()  {   return AgroMode == AgroModeMechSow; }   // Режим "Механические сеялки"
    bool AgroCombine2() {   return AgroMode == AgroModeCombine2;}   // Режим "Комбайн 2"

    bool SowNormaOn()       {   return sowNormaMode != SowNorma_Off;    }
    bool SowNormaOff()      {   return sowNormaMode == SowNorma_Off;    }
    bool SowNormaBase()     {   return sowNormaMode == SowNorma_Base;   }
    bool SowNormaAuto()     {   return sowNormaMode == SowNorma_Auto;   }   // Автоматическая, скорость по GPS
    static  const char* ModeSowNorma_toString(int mode);
    std::string SowNormaKg_toString(double kg);
    std::string getNameCulture( int val);                                   // Возвращает имя посевной культуры
    std::string NameCulture(    int val, int index);                        // Возвращает имя посевной культуры (для автоматической нормы высева)
    void        setNameCulture( int val, int index, const std::string &name);   // Установить имя посевной культуры (для автоматической нормы высева)

    std::string NameLiquid(int index);                          // Возвращает имя жидких удобрений
    void        setNameLiquid(int index, const std::string& name);  // Установить имя жидких удобрений

    bool IsEncoderTv2()         {   return isEncoderTv2;                }
    bool IsFan1Tv2()            {   return isFan1Tv2;                   }
    bool IsFan2Tv2()            {   return isFan1Tv2 && isFan2Tv2;      }
    bool IsPress1Tv2()          {   return isPressTv2;                  }
    bool IsPress2Tv2()          {   return isPressTv2 && isPress2Tv2;   }
    bool IsActuatorTv2()        {   return isActuatorTv2;               }
    bool StreemToEncoderTv2()   {   return SpeedMethodEncoder() && !StreemToMsTv2; }  // Управление потоком семян по Энкодеру (по импульсам), иначе интервал задаем в мс

// Common
    int     BunkersCount;                   // кол-во бункеров
    int     SectionsCount;                  // кол-во секций (Механические сеялки)
    int     TowersCount;                    // кол-во башень
    int     SemensCount[cTowersMaxCount];   // кол-во семяпроводов модуля(башни)
    int     CropsType[cTowersMaxCount];     // тип зерна (удобрения)
    int     FansCount;                      // Кол-во вентиляторов
    int     FanImpulsCount[cFansMaxCount];  // кол-во импульсов на оборот вентилятора
    int     ValsCount;                      // Кол-во валов (высевающих) "Посевной комплекс"
    int     ModCropsCount;                  // кол-во модулей зерна     (точный высев)
    int     ModManureCount;                 // кол-во модулей удобрений (точный высев)
    int     SensorCropsCount[cMaxModules];  // кол-во датчиков зерна в модуле       (максимум: 3 модуля * 12 датчиков)
    int     SensorManureCount[cMaxModules]; // кол-во датчиков удобрений в модуле   (максимум: 3 модуля * 12 датчиков)
    bool    ControlLevelCrops;              // контролировать уровень зерна
    bool    ControlLevelManure;             // контролировать уровень удобрений

    int     SensorCountTv2;     // Кол-во датчиков зерна, максимум:24   (точный высев2)
    bool    isEncoderTv2;       // Есть Енкодер                         (точный высев2)
    bool    isFan1Tv2;          // Есть вентилятор 1                    (точный высев2)
    bool    isFan2Tv2;          // Есть вентилятор 2                    (точный высев2)
    int     imp2cycleFanTv2;    // Импульсов на оборот вентилятора 1    (точный высев2)
    int     imp2cycleFan2Tv2;   // Импульсов на оборот вентилятора 2    (точный высев2)
    bool    isPressTv2;         // Есть датчик разряжения               (точный высев2)
    bool    isPress2Tv2;        // Есть 2-ой датчик разряжения          (точный высев2)
    bool    isActuatorTv2;      // Есть актуатор                        (точный высев2)
    bool    calcDoubleTv2;      // Расчетные двойники                   (точный высев2)
    bool    StreemToMsTv2;      // Управление потоком семян в мс        (точный высев2)

    bool    isSowWidth;         // Есть ширина высева (метр)
    double  SowWidth;           // Ширина высева (метр)
    double  LeftWidth;          // Ширина левой стороны высева (метр)
    double  RightWidth;         // Ширина правой стороны высева (метр)
    double  SowOver;            // Ширина перекрытия (метр)
    bool    Gisto_Show;
    bool    GistoYellowExist;   // Желтая зона есть
    int     GistoYellowProcent; // Желтая зона гистограммы (%)
    bool    GistoRedExist;      // Красная зона есть
    int     GistoRedProcent;    // Красная зона гистограммы (%)
    int     GistoSizeAverageIdx;// Индекс разсера усреднения гистограммы
    static  int GistoSizeAverageIdx_toSize(int idx) {
        switch (idx) {
        case 0:     return 1;
        case 1:     return 5;
        case 2:     return 10;
        case 3:     return 20;
        case 4:
        default:    return 30;
        }
    }
#define cGISTO_LEVEL_GREEN    1
#define cGISTO_LEVEL_YELLOW   2
#define cGISTO_LEVEL_RED      3
    int     Gisto_forProcent(double procent) {
        if (procent > GistoRedProcent) {
            return GistoRedExist ? cGISTO_LEVEL_RED : cGISTO_LEVEL_GREEN;
        }
        if (procent > GistoYellowProcent) {
            return GistoYellowExist ? cGISTO_LEVEL_YELLOW : cGISTO_LEVEL_GREEN;
        }
        return cGISTO_LEVEL_GREEN;
    }

    bool    PlugDownInversion;  // Концевик заглубления инверсный
    bool    PlugDownForce;      // Концевик замкнуть (для случая, когда неисправен(потерян) концевик)
    bool    DepthSensorsExists; // Датчики глубины подключены
    uint    DepthSensorsMaskaOn;// Маска подключенных(логически) датчиков глубины (им посылать запрос, остальным нет)
    bool    DepthSensorsOn(uchar isen) {
        if (isen >= cMaxSensorsTv2) return false;
        return DepthSensorsMaskaOn & (1<<isen) ? true : false;
    }
    void    DepthSensorsOn_set(uchar isen, bool on)
    {
        if (isen < cMaxSensorsTv2) {
            if (on) {
                DepthSensorsMaskaOn |= (1<<isen);
            } else {
                DepthSensorsMaskaOn &= ~(1<<isen);
            }
        }
    }
    int     DepthSensorsTv2() { // Количество датчиков глубины (точный высев 2)
        return SensorCountTv2;  // по количеству датчиков зерна
    }

    bool    FixChangeStream;    // Фиксировать изменение потока

    int     SectionExists[cSectionsMaxCount][cSectionItemCount];      // конфигурация механических сеялок
    int     SectionBunker_exists(int section, int bunker) {
        if (section < SectionsCount && bunker < 2) {
            return SectionExists[section][0 + 2*bunker];
        } else {
            return 0;
        }
    }
    int     SectionValDos_exists(int section, int val) {
        if (section < SectionsCount && val < 2) {
            return SectionExists[section][1 + 2*val];
        } else {
            return 0;
        }
    }
    int     ValsCount_toSections()  // Количество валов в секциях
    {
        int count = 0;
        for (int i = 0; i < SectionsCount; i++) {
            for (int val = 0; val < 2; val++) {
                if (SectionValDos_exists(i, val)) {
                    count++;
                }
            }
        }
        return count;
    }

// Add
    int     SpeedMethod;                    // Метод определения скорости агрегата
    bool    SpeedMethodNone()       {   return SpeedMethod == SpeedNone     ? true : false;     }
    bool    SpeedMethodGPS()        {   return SpeedMethod == SpeedGPS      ? true : false;     }
    bool    SpeedMethodEncoder()    {   return SpeedMethod == SpeedEncoder  ? true : false;     }
    bool    SpeedMethodFixing7()    {   return SpeedMethod == SpeedFixing7  ? true : false;     }
    double  SpeedEncoderConstant;           // Константа Энкодера скорости мм/имульс
    double  SpeedEncoderImp2metr;           // импульсов/метр энкодера колеса скорости
    bool    EncoderCalibrateControl;        // Контроль калибровки энкодера включен (точный высев2)
    int     ProcentAlarmCalibrateEncoder;   // При отклонении скорости по Энкодеру от скорости GPS более, чем на %, показать СБОЙ КАЛИБРОВКИ ЭНКОДЕРА (точный высев2)
    bool    SpeedEncoderCalibrated()
    {
        return SpeedEncoderConstant > qreal(0.001) ? true : false;
    }

    bool    Fan_Gidro;
    bool    Fan_Dvigatel;

    bool    TimeSync;           // время синхронизировать по GPS
    int     TimeRegion;         // часовой пояс (добавка в часах к UTC)

    std::string PortContr;          // СОМ-порт соединения с контроллером
    std::string PortSpeed;          // скорость СОМ-порта соединения с контроллером
    std::string PaswCommon;         // Пароль

// Special
    int     SpeedSowMax;                    // максимальная скорость агрегата
    std::string PaswSpecial;
    int     SowDepthOn;                     // Контроль глубины высева ВКЛЮЧЕН
    int     SowDepthAgrIndex;               // Текущий индекс выбора имени навесного агрегата

    int     FanBaudMin[cFansMaxCount];      // Нижний предел оборотов вентилятора
    int     FanBaudMax[cFansMaxCount];      // Верхний предел оборотов вентилятора
    bool    UseSystemScale;                 // Использовать системные весы
    int     ActuatorsBack;                  // Маска инверсных актуаторов:  0х01 -0-ой актуатор, 0х02 -1-ой актуатор, 0х04 -2-ой актуатор
    bool    PressControl;                   // Контролировать давление в бункере
    int     PressureMin;                    // Давление в бункере минимально-допустимое

    int     FanMaxRpmTv2;           // Максимальные обороты вентилятора, об/мин (точный высев2)
    int     PressMustTv2;           // Требуемое разряжение воздуха, Па         (точный высев2)
    int     PressFromTv2;           // Допустимое разряжение воздуха, Па. От    (точный высев2)
    int     PressToTv2;             // Допустимое разряжение воздуха, Па. До    (точный высев2)
    int     HisteresisTv2;          // Гистерезис разряжение воздуха, Па.       (точный высев2)
    int     IntervalForDoubleTv2;   // Интервал для двойников, мс               (точный высев2)
    int     CoefIntervalForPassTv2; // Коэффициент интервала для 1-го пропуска   (точный высев2)

    int     SowNormaIndexCulture[cValsMaxCount];                    // номер культуры от 0 для каждого вала
    double  SowNormaBaseKg[     cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (кг) (1/10 нормы высева)  Калибровка культутры
    int     SowNormaBaseImp[    cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (импульсов)               Калибровка культутры
    double  SowNormaAutoKg[     cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (кг) (1/10 нормы высева)  Калибровка культутры
    int     SowNormaAutoImp[    cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (импульсов)               Калибровка культутры
    int     SowNormaIndexCultureDiff[cValsMaxCount];                // номер культуры от 0 для каждого вала
    double  SowNormaBaseKgDiff[ cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (кг) (1/10 нормы высева)  Калибровка культутры
    int     SowNormaBaseImpDiff[cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (импульсов)               Калибровка культутры
    double  SowNormaAutoKgDiff[ cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (кг) (1/10 нормы высева)  Калибровка культутры
    int     SowNormaAutoImpDiff[cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (импульсов)               Калибровка культутры
    int     SowNormaIndexCulture_get(uchar val);                    // номер культуры от 0 для каждого вала
    void    SowNormaIndexCulture_set(uchar val, int iCulture);      // номер культуры от 0 для каждого вала
    int     SowNormaImp_get(uchar val);
    int     SowNormaImp_get(uchar val, int iCulture);
    void    SowNormaImp_set(uchar val, int iCulture, int imp);
    double  SowNormaKg_get(uchar val);
    double  SowNormaKg_get(uchar val, int iCulture);
    void    SowNormaKg_set(uchar val, int iCulture, double kg);
    bool    SowNormaKg2imp(uchar val, int iCulture, double& kg2imp);

    double  SowNormaSet[    cValsMaxCount];                     // Норма высева, установленная пользователем кнопкой "изменение норма высева"
    int     SowNormaPotenc[ cValsMaxCount];                     // Значение потенциометра для установленной нормы высева
    std::string nameCulture[cValsMaxCount][cMaxCountCulture];   // наименование культуры
    bool    SowNormaDebug;                                      // Отладка нормы высева

    bool    SowNorma_exists(uchar val);
    bool    Culture_calibrated(uint val, int iCulture);
    bool    Culture_calibrated(uint val);
    const char* SowNormaBase_state(uint ival, double kg2ga);
    int     SowNormaTv_Duration;
    int     SowNormaTv_Interval;
    double  SowNormaTv_Count2m;         // установленная линейная норма высева (зерен на метр)
    int     SowNormaTv_ProcentPass;     // Если отклонение от нормы более установленного процента -Окрасить желтым и пикнуть три раза
    int     SowNormaTv_ProcentDouble;   // Если отклонение от нормы более установленного процента -Окрасить желтым и пикнуть три раза
    bool    SowNormaTv_ShowAuto;        // При выглублении открыть окно на 7сек
    int     SowNormaTv_Average;         // Усреднение гистограммы нормы высева 5сек, 10сек, 100сек, Загонка
    bool    SowNormaTv_ModeSowAlarm;    // Режим засорений (фиксировать только засорения, двойники и пропуски не фиксировать)
    int     SowNormaTv_ProcentAlarm;    // Если счетчики зерен за секунду ниже установленного процента - засорение
    std::string SowNormaTv_Culture;     // Наименование культуры

    bool    SowNormaTv_Exists() {       // Норма точного высева существует
        return SowNormaTv_Count2m > qreal(0.01) ? true : false;
    }
    static int SowNormaTv_Average_indexToSec(int count) {
        switch (count) {
        case 0:     return 1;       // 1sec
        case 1:     return 5;       // 5sec
        case 2:     return 10;      // 10sec
        case 3:     return 100;     // 100sec
        default:    return 0;       // Загонка
        }
    }

// SowNorma Options. Скорость по энкодеру.
    double  ScaleCoef0[cValsMaxCount];  // Коэффициент поправочный (весы 1)
    double  ScaleCoef1[cValsMaxCount];  // Коэффициент поправочный (весы 2)
    int     ScaleProcent;               // Максимальный гистерезис при взвешивании
    int     SowNormaMax;                // Максимальная норма высева кг/га
    bool    SowNormaMin15;              // Минимальная норма высева 15 кг/га
    ushort  Potencio[cValsMaxCount][cIntervals + 1];    // Значения потенциометра(вал штока).   Норма высева, если скорость по энкодеру
    double  Imp2metr[cValsMaxCount][cIntervals + 1];    // Импульсов/метр вала высевающего.     Норма высева, если скорость по энкодеру

// SowNorma Options. Дифференциальное внесение
    bool    DifEntry;                   // Дифференциальное внесение ВКЛЮЧИТЬ
    double  DifNormaBase[   cValsMaxCountCmd];  // Норма высева вне контура
    double  DifNormaGpsNot[ cValsMaxCountCmd];  // Норма высева, кода нет GPS-сигнала
// SowNorma Options. Актуаторы
    int     ActTransRatio[cValsMaxCountCmd];    // Передаточное число актуатора

// Counts
    bool    BunkerHide[cBunkersMaxCount];

// Security
    int     volumeProcent;      // процент громкости
    int     brightnessProcent;  // процент яркости
    bool    sowNormaAutoAverage;// усреднять норму высева (при отображении)
    int     sowNormaMedianaIdx; // индекс размера медианы для "усреднять норму высева (при отображении)"
    static  int     SowNormaMedianaIdx_toSize(int idx) {
        switch (idx) {
        case 1:     return 31;
        case 2:     return 61;
        case 0:
        default:    return 11;
        }
    }
    bool    sowNormaAverageJson;// усреднять норму высева (при передаче в Json-формате)
    int     sowNormaJsonSendSec;// норму высева в Json-формате передавать не чаще, сек
    int     SowNormaMedianaActuator;    // размер усреднения нормы высева для управления актуатором
    bool    SowNormaAverageMediana;     // тип усреднения нормы высева для управления актуатором - true -Медиана, иначе Арифметическое среднее
    bool    traficWrite;        // писать входной трафик
    bool    logErrWrite;        // писать лог ошибок, входной трафик 5сек до ошибки и 10 сек после
    bool    errorIgnore;        // игнорировать ошибки датчиков
    int     valImpCorr[cValsMaxCount];  // коррекция импульсов вала дозатора, %
    int     sowNormaMode;       // Режим контроля норма высева
    int     SowNormaProcent;    // Процент нормы высева
    bool    LargeButtons;       // Большие сенсорные кнопки (если физически кнопки неисправны, можно пользоваться сенсорными увеличенными кнопками)
    bool    SendJsonFormat;     // Автограф в формате JSON
    bool    StaticSensorsOn;    // В состоянии заглубления каждую секунду передавать в контроллер нулевые датчики засорений
    bool    ModeService;        // Режим сервиса (Отладка нормы высева, ...)
    int     BrightnessScreen;   // Яркость экрана в процентах
    int     BrightnessButtons;  // Яркость кнопок в процентах

    int     indexBrightnessScreen;   // Яркость экрана в процентах
    int     indexBrightnessButtons;  // Яркость кнопок в процентах
    int     procentScreen100;
    int     procentScreen80;
    int     procentScreen60;
    int     procentScreen40;
    int     procentScreen20;
    int     procentScreen10;
    int     procentButtons100;
    int     procentButtons80;
    int     procentButtons60;
    int     procentButtons40;
    int     procentButtons20;
    int     procentButtons10;
    int     ProcentScreenToValue(int procent)
    {
        switch (procent) {
        case 100:   return procentScreen100;
        case 80:    return procentScreen80;
        case 60:    return procentScreen60;
        case 40:    return procentScreen40;
        case 20:    return procentScreen20;
        case 10:    return procentScreen10;
        default:    return procentScreen100;
        }
    }
    int     ProcentButtonsToValue(int procent)
    {
        switch (procent) {
        case 100:   return procentButtons100;
        case 80:    return procentButtons80;
        case 60:    return procentButtons60;
        case 40:    return procentButtons40;
        case 20:    return procentButtons20;
        case 10:    return procentButtons10;
        default:    return procentButtons100;
        }
    }
    static  int     BrightnessIndex_toProcent(int index)
    {
        switch (index) {
        case 0:     return 100;
        case 1:     return 80;
        case 2:     return 60;
        case 3:     return 40;
        case 4:     return 20;
        case 5:     return 10;
        default:    return 100;
        }
    }
    int getBrightnessScreen()
    {
        int procent = BrightnessIndex_toProcent(indexBrightnessScreen);
        int value = ProcentScreenToValue(procent);
        //printf("Screen index=%d proc=%d value=%d\n", indexBrightnessScreen, procent, value);
        return value;
    }
    int getBrightnessButtons()
    {
        int procent = BrightnessIndex_toProcent(indexBrightnessButtons);
        int value = ProcentButtonsToValue(procent);
        //printf("Buttons index=%d proc=%d value=%d\n", indexBrightnessButtons, procent, value);
        return value;
    }
// Navigator
    bool    Navigator;          // Навигатор включен
    bool    TrackerSound;       // Пищать при отклонении от курса более TrackerAlarmMetr
    double  TrackerOversow;     // Допустимый пересев (метров)
    double  TrackerUndersow;    // Допустимый недосев (метров)
    bool    ShowAlarm;          // показывать аварии (переключаться из навигатора на главное окно при аварии)
    double  LengGPS;            // длина(м) от GPS до сцепки
    double  LengBunker;         // длина(м) бункера с дышлом
    double  LengCultivator;     // длина(м) культиватора с дышлом
    int     SchemeAgregate;     // схема агрегата см. Scheme_Agregate
    int     TypeWork;           // Вид обработки: сев, вспашка, боронование
    bool    PlugNavigManual;    // Ручное управление заглублением от опрыскивателя (режим НАВИГАТОР)
    std::string GpsPort;        // Порт GPS-приемника
    std::string GpsSpeed;       // Скорость GPS-приемника
    int     StoreRealTime;      // Маска сохранения протоколов в режиме реального времени
    bool    realTimeState()
    {
        if (realTimeOff()) {
            return true;
        }
        if (realTimeInternet()) {
            if (!SendJsonFormat) {
                return true;
            } else {
                return StoreRealTime & cMaskaRealTimeState ? true : false;
            }
        }
        return StoreRealTime & cMaskaRealTimeState ? true : false;
    }
    bool    realTimeValue()
    {
        if (realTimeOff()) {
            return true;
        }
        if (realTimeInternet()) {
            if (!SendJsonFormat) {
                return true;
            } else {
                return StoreRealTime & cMaskaRealTimeValue ? true : false;
            }
        }
        return StoreRealTime & cMaskaRealTimeValue ? true : false;
    }
    bool    realTimeDepth()
    {
        if (realTimeOff()) {
            return true;
        }
        if (realTimeInternet()) {
            if (!SendJsonFormat) {
                return true;
            } else {
                return StoreRealTime & cMaskaRealTimeDepth ? true : false;
            }
        }
        return StoreRealTime & cMaskaRealTimeDepth ? true : false;
    }
    bool    realTimeGisto()
    {
        if (realTimeOff()) {
            return true;
        }
        if (realTimeInternet()) {
            if (!SendJsonFormat) {
                return true;
            } else {
                return StoreRealTime & cMaskaRealTimeGisto ? true : false;
            }
        }
        return StoreRealTime & cMaskaRealTimeGisto ? true : false;
    }
    bool    realTimeOptions()
    {
        return StoreRealTime & cMaskaRealTimeOptions    ? true : false;
    }
    void    realTimeCoord_set(int value)
    {
        StoreRealTime &= ~(cMaskaRealTimeCoord1sec + cMaskaRealTimeCoord2sec + cMaskaRealTimeCoord5sec);
        switch(value) {
        //case 0:     // Нет
        case 1:     StoreRealTime |= cMaskaRealTimeCoord1sec;   break;
        case 2:     StoreRealTime |= cMaskaRealTimeCoord2sec;   break;
        case 3:     StoreRealTime |= cMaskaRealTimeCoord5sec;   break;
        }
    }
    void    realTimeState_set(bool on)  {   on ? StoreRealTime |= cMaskaRealTimeState   : StoreRealTime &= ~cMaskaRealTimeState;    }
    void    realTimeValue_set(bool on)  {   on ? StoreRealTime |= cMaskaRealTimeValue   : StoreRealTime &= ~cMaskaRealTimeValue;    }
    void    realTimeDepth_set(bool on)  {   on ? StoreRealTime |= cMaskaRealTimeDepth   : StoreRealTime &= ~cMaskaRealTimeDepth;    }
    void    realTimeGisto_set(bool on)  {   on ? StoreRealTime |= cMaskaRealTimeGisto   : StoreRealTime &= ~cMaskaRealTimeGisto;    }
    void    realTimeOptions_set(bool on){   on ? StoreRealTime |= cMaskaRealTimeOptions : StoreRealTime &= ~cMaskaRealTimeOptions;  }

    inline  bool    realTimeOff()       {   return  sendRealTime == SendRealTime_off        ?   true : false;   }
    inline  bool    realTimeInternet()  {   return  sendRealTime == SendRealTime_internet   ?   true : false;   }
    inline  bool    realTimeAutograph() {   return  sendRealTime == SendRealTime_autograph  ?   true : false;   }

// Internet
    int         sendRealTime;       // SendRealTime_off, SendRealTime_internet, SendRealTime_autograph
    std::string tcpIpServer;
    std::string owner;
    std::string password;
    std::string agrName;
    std::string agrNameTv;
    int         indexProviderSim0;
    int         indexProviderSim1;
    std::string PortModem;          // порт GSM-модема
    std::string PortModemSpeed;     // скорость порта GSM-модема
    int         GprsReduction;      // Уменьшение выходной мощности GSM-модуля
    int         PauseAfterTransmit; // Пауза после передачи, мин

// SemensOff
    bool SemensOff[cTowersMaxCount][cSemensMaxCount];   // Отключенные датчики зерна
    bool SemensCropsOff[ cMaxModules][cMaxSensors];     // Отключенные датчики зерна        (точный высев)
    bool SemensManureOff[cMaxModules][cMaxSensors];     // Отключенные датчики удобрений    (точный высев)

// Жидкие удобрения
    int     VolumeCount;                                // Количество емкостей с жидкими удобрениями
    bool    IsSensorPress[  cMaxCountVolumeLiquid];     // есть датчик давления
    bool    IsFlowmeter[    cMaxCountVolumeLiquid];     // есть расходомер
    double  SensorPressMax[ cMaxCountVolumeLiquid];     // максимальное давление датчика
    double  FlowmeterMl2imp[cMaxCountVolumeLiquid];     // цена импульса расходомера
    int     LiquidNameIndex[cMaxCountVolumeLiquid];     // индекс имени жидких удобрений
    bool    LiquidSound;                                // Звук при превышении давления
    bool    LiquidShowEvent;                            // Переключать с основного окна при событии
    std::string nameLiquid[ cMaxCountLiquid];           // наименование жидких удобрений

    //===============
    Options();
    void    setDiskData(const std::string &path);
    int     GetNumberCrops(int im, int is);         // Возвращает сквозной номер семяпровода от 1   (agroTv)
    int     GetNumberManure(int im, int is);        // Возвращает сквозной номер удобрениевода от 1 (agroTv)
    int     SowNormaTv_StepSowSm();                 // возвращает установленный шаг высева в см
    double  SowNormaTv_MsecNorma(double m2s);       // возвращает установленный(калиброванный) интервал между зернами в мсек для скорости m2s
    double  SowNormaTv_Count2s(double m2s);             // Норма высева штук/сек для данной скорости
    double  SowNormaTv_Count2s(double m2s, int msec);   // Норма высева штук/сек для данной скорости и длительности

    void    ActuatorsCalibrate_clear();             // Сброс калибровки актуаторов
    bool    ActuatorCalibrated(uchar val);          // Вернуть true, если Актуатор калиброван (скорость по Энкодеру)

    ushort  i2m_to_potenc(uchar val, double i2m);
    ushort  kg2ga_to_potenc(uchar val, double kg2ga);
    ushort  potenc_min_forSetNorma(uchar val);      // Возвращает минимальное значение потенциометра при установке нормы высева
    ushort  Potenc_for_kg2ga_forCulture(uchar val, double kg2ga, int iCulture);
    void    Potenc_and_Imp2metr_forSowNormaSet(uchar val, double km2h, ushort& potenc, double& i2m);    // Возвращает значение потенциометра и импульсов/метр для установленной нормы высева. Скорость по энкодеру
    ushort  SowNormaSet_toPotenc(int val);     // Переводит значение нормы (кг/га) в значение Потенциометра

    void    Read();
    bool    Write(bool storeIfCmp = false);
    void    CalibrateCultureDiff_clear();
    double  Sow_Width();
    double  SowWidth_subOver();
    double  LeftWidth_subOver();
    double  RightWidth_subOver();
    double  LengthTrainToCultivator();
    std::string pathData();
    static  std::string shortNameFileClientLog();
    static  std::string shortNameFileClientLogDebug();
    std::string nameFileAgroClientLog();
    std::string nameFileAgroClientLogDebug();
    std::string nameFileContrErrorsLog();
    static      std::string nameFileDayLog(const std::string& path, time_t date);
    static      std::string nameFileDayLog(const std::string& path);

    void    param_add_SemensOff(DXMLObject* root);
    void    param_add_SemensCropsOff(DXMLObject* root);
    void    param_add_SemensManureOff(DXMLObject* root);

    std::string pathOptionsForSend();   // папка файлов для xml-файла настроек после изменения
    std::string pathSend();             // файлы для передачи на сервер
    std::string pathSended();           // файлы, переданные на сервер
    std::string pathRecvTmp();          // папка для принимаемого файла
    std::string pathRecv();             // файлы, принятые с сервера
    std::string pathCoordinate();
    std::string pathTracks_Sow();
    std::string pathTracks_Plough();
    std::string pathTracks_Harrow();
    std::string pathTracks_SPH();       // треки текущего режима работы: сев, вспашка или боронование(подработка)
    std::string pathTracks_depth();     // треки текущего режима работы с значениями датчиков глубины
    std::string pathTracks();           // треки для сервера
    std::string pathSound();
    std::string pathBunker();
    std::string pathPressure();
    std::string pathArea();
    std::string pathGisto();
    std::string pathSowNorma();
    std::string pathSowNormaAlarm();
    std::string pathSpeedSowMax();
    std::string pathValRotate();
    std::string pathSemens();
    std::string pathFanBaudMax();
    std::string pathOptionsChange();
//    std::string pathStartup();
    std::string pathLoops();
    std::string pathRealTime();

    std::string pathDebug();
    std::string pathGps();
    std::string pathMap();
    std::string pathLogErrors();

    std::string pathTvArea();
    std::string pathTvGisto();
    std::string pathTvSemsGa();
    std::string pathTvStartup();
    std::string pathTvOptionsChange();
    std::string pathTvBunkerCrops();
    std::string pathTvBunkerManure();
    std::string pathTvSemens();
    std::string pathTvManure();
    std::string pathTvDoubles();
    std::string pathTvSpeedSowMax();
    std::string pathTvTracks_Sow();
    std::string pathTvLoops();
};

extern Options options;

#endif // OPTIONS_H
