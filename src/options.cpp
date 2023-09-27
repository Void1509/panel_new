#include <stdio.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include "count_of.h"
#include "utils.h"
#ifdef  IMX6_SODIMM_
#include <QTextCodec>
#include "../agroNavig22/shape_list.h"
#endif
//#include "diconv.h"
#include "timeDecode.h"
#include "options.h"
//#include "../agroWin22/Store_2023_08_25/options.h"
#include "xml.h"
#include "cp1251_to_utf8.h"

int sound_off = 0;     // отключение звука (для отладки дома, чтобы не мешал)

//================
Options::Options()
{
    printf("create Options ");
//--- Common ---
#ifdef  IMX6_SODIMM_
    // установим кодировку строковых констант, так как конструктор может быть вызван до main()
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif
    AgroMode = AgroModeSow;

    BunkersCount = 2;           // кол-во бункеров
    SectionsCount = 6;          // кол-во сеялок (Механические сеялки)
    TowersCount = 6;            // кол-во башен
    for (int i = 0; i < cTowersMaxCount; i++) {
        SemensCount[i] = 10;    // кол-во семяпроводов башни
        CropsType[i] = 0;       // тип зерна (удобрения)
    }
    FansCount = 1;              // Кол-во вентиляторов
    for (int i = 0; i < cFansMaxCount; i++) {
        FanImpulsCount[i] = 1;  // кол-во импульсов на оборот вентилятора
    }
    ValsCount = 2;              // Кол-во валов (высевающих)

    for (int i = 0; i < cMaxModules; i++) {
        SensorCropsCount[i] = cMaxSensors;   // кол-во датчиков зерна в модуле
        SensorManureCount[i] = cMaxSensors;  // кол-во датчиков удобрения в модуле
    }
    ControlLevelCrops = true;   // Контролировать уровень зерна
    ControlLevelManure = true;  // Контролировать уровень удобрений

    SensorCountTv2  = 12;       // Кол-во датчиков зерна, максимум:24   (точный высев2)
    isEncoderTv2    = false;    // Есть Енкодер                 (точный высев2)
    isFan1Tv2       = false;    // Есть вентилятор 1            (точный высев2)
    isFan2Tv2       = false;    // Есть вентилятор 2            (точный высев2)
    imp2cycleFanTv2 = 10;       // Импульсов на оборот вентилятора 1    (точный высев2)
    imp2cycleFan2Tv2= 10;       // Импульсов на оборот вентилятора 2    (точный высев2)
    isPressTv2      = false;    // Есть датчик разряжения       (точный высев2)
    isPress2Tv2     = false;    // Есть 2-ой датчик разряжения  (точный высев2)
    isActuatorTv2   = false;    // Есть актуатор                (точный высев2)

    isSowWidth  = true;         // Есть ширина высева (метр)
    SowWidth    = qreal(10.0);  // Ширина высева (метр)
    LeftWidth   = qreal(5.0);   // Ширина левой стороны высева (метр)
    RightWidth  = qreal(5.0);   // Ширина правой стороны высева (метр)
    SowOver     = qreal(0.2);   // Ширина перекрытия (метр)
    Gisto_Show          = true;
    GistoYellowProcent  = 30;       // Желтая зона гистограммы (%)
    GistoRedProcent     = 90;       // Красная зона гистограммы (%)
    GistoSizeAverageIdx = 1;        // Индекс разсера усреднения гистограммы
    GistoYellowExist    = true;     // Желтая зона есть
    GistoRedExist       = true;     // Красная зона есть

    PlugDownInversion   = false;    // Концевик заглубления инверсный
    PlugDownForce       = false;    // Концевик замкнуть (для случая, когда неисправен(потерян) концевик)
    DepthSensorsExists  = false;    // Датчики глубины подключены
    DepthSensorsMaskaOn = 0;        // Маска подключенных(логически) датчиков глубины (им посылать запрос, остальным нет)
    FixChangeStream     = true;     // Фиксировать изменение потока

    for (int i = 0; i < cSectionsMaxCount; i++) {
        for (int j = 0; j < cSectionItemCount; j++) {
            SectionExists[i][j] = 1;      // конфигурация механических сеялок
        }
    }

//--- Add ---
    SpeedMethod = SpeedGPS;
    SpeedEncoderConstant = qreal(0);         // Константа Энкодера скорости мм/имульс
    SpeedEncoderImp2metr = qreal(100);       // импульсов/метр энкодера колеса скорости
    EncoderCalibrateControl = false;    // true -Контроль калибровки энкодера включен (точный высев2)
    ProcentAlarmCalibrateEncoder = 5;   // При отклонении скорости по Энкодеру от скорости GPS более, чем на %, показать СБОЙ КАЛИБРОВКИ ЭНКОДЕРА

    Fan_Gidro       = false;
    Fan_Dvigatel    = false;
    TimeSync        = true;             // время синхронизировать по GPS
    TimeRegion      = 0;                // часовой пояс (добавка в часах к UTC)
    PortContr       = "mxc2";
    PortSpeed       = "115200";
    PaswCommon      = "123";

//--- Special ---
    SpeedSowMax = 10;           // максимальная скорость агрегата
    for (int i = 0; i < cFansMaxCount; i++) {
        FanBaudMin[i] = 1000;   // Нижний предел оборотов вентилятора
        FanBaudMax[i] = 5000;   // Верхний предел оборотов вентилятора
    }
    UseSystemScale  = false;    // Использовать системные весы
    ActuatorsBack   = 0;        // Маска инверсных актуаторов:  0х01 -0-ой актуатор, 0х02 -1-ой актуатор, 0х04 -2-ой актуатор
    PressControl    = false;    // Контролировать давление в бункере
    PressureMin     = 3;        // Давление в бункере минимально-допустимое
    PaswSpecial     = "123";

    FanMaxRpmTv2    = 5000;     // Максимальные обороты вентилятора, об/мин (точный высев2)
    PressMustTv2    = 5000;     // Требуемое разряжение воздуха, Па         (точный высев2)
    PressFromTv2    = 2500;     // Допустимое разряжение воздуха, Па. От    (точный высев2)
    PressToTv2      = 6000;     // Допустимое разряжение воздуха, Па. До    (точный высев2)
    HisteresisTv2   = 20;       // Гистерезис разряжение воздуха, Па.       (точный высев2)
    IntervalForDoubleTv2 = 50;  // Интервал для двойников, мс
    CoefIntervalForPassTv2 = 7; // Коэффициент интервал для пропусков
    calcDoubleTv2   = true;     // Расчетные двойники           (точный высев2)
    StreemToMsTv2   = false;    // Управление потоком семян в мс(точный высев2)

    SowDepthOn          = 0;    // Контроль глубины высева ВКЛЮЧЕН
    SowDepthAgrIndex    = 0;    // Текущий индекс выбора имени навесного агрегата

    for(int val = 0; val < COUNT_OF(SowNormaIndexCulture); val++) {
        SowNormaIndexCulture[val]       = 0;    // индекс культуры для нормы высева (базовой или автоматической)
    }
    for(int val = 0; val < COUNT_OF(SowNormaSet); val++) {
        SowNormaSet   [val] = 0;    // Норма высева установленная пользователем кнопкой "изменение норма высева"
        SowNormaPotenc[val] = 0;    // Значение потенциометра для установленной нормы высева
    }

    for(int val = 0; val < COUNT_OF(nameCulture); val++) {
        for(int j = 0; j < cMaxCountCulture; j++) {
            nameCulture         [val][j]  = "";
            SowNormaBaseKg      [val][j]  = qreal(0);     // Норма высева расчетная (кг)
            SowNormaBaseImp     [val][j]  = 0;            // Норма высева расчетная (импульсов)
            SowNormaAutoKg      [val][j]  = qreal(0);     // Норма высева расчетная (кг)
            SowNormaAutoImp     [val][j]  = 0;            // Норма высева расчетная (импульсов)
        }
    }
    CalibrateCultureDiff_clear();

    SowNormaDebug = false;          // Отладка нормы высева

    SowNormaTv_Duration     = 1;    // Длительность (мсек) (точный высев)
    SowNormaTv_Interval     = 2;    // Интервал (мсек) (точный высев)
    SowNormaTv_Count2m      = 5;    // установленная линейная норма высева (зерен на метр)
    SowNormaTv_ProcentPass  = 25;   // Если отклонение от нормы более установленного процента -Окрасить желтым и пикнуть три раза
    SowNormaTv_ProcentDouble= 25;   // Если отклонение от нормы более установленного процента -Окрасить желтым и пикнуть три раза
    SowNormaTv_ShowAuto     = true; // При выглублении открыть окно на 7сек
    SowNormaTv_Average      = 2;    // Усреднение гистограммы нормы высева 1сек, 5сек, 10сек, 100сек, Загонка
    SowNormaTv_ModeSowAlarm = false;// Режим засорений (фиксировать только засорения, двойники и пропуси не фиксировать)
    SowNormaTv_ProcentAlarm = 25;   // Если счетчики зерен за секунду ниже установленного процента - засорение
    SowNormaTv_Culture      = "";   // Наименование культуры

    // SowNorma Options
    for (size_t i = 0; i < COUNT_OF(ScaleCoef0); i++) {
        ScaleCoef0[i] = qreal(1.0);    // Коэффициент поправочный (весы 1)
        ScaleCoef1[i] = qreal(1.0);    // Коэффициент поправочный (весы 2)
    }
    ScaleProcent    = 10;           // Максимальный гистерезис при взвешивании
    SowNormaMax     = 100;          // Максимальная норма высева
    SowNormaMin15   = true;         // Минимальная норма высева 15 кг/га

    for (uchar val = 0; val < COUNT_OF(Potencio); val++) {
        for (size_t i = 0; i < COUNT_OF(Potencio[0]); i++) {
            Potencio[val][i] = 0;
        }
    }
    for (uchar val = 0; val < COUNT_OF(Imp2metr); val++) {
        for (size_t i = 0; i < COUNT_OF(Imp2metr[0]); i++) {
            Imp2metr[val][i] = qreal(0);
        }
    }

    DifEntry = false;       // Дифференциальное внесение ВКЛЮЧИТЬ
    for (uchar val = 0; val < COUNT_OF(DifNormaBase); val++) {
        DifNormaBase[   val]    = qreal(0);
        DifNormaGpsNot[ val]    = qreal(0);
    }
    for (uchar val = 0; val < COUNT_OF(ActTransRatio); val++) {
        ActTransRatio[val] = 10;    // Передаточное число актуатора
    }

//--- Counts ---
    for (int i = 0; i < COUNT_OF(BunkerHide); i++) {
        BunkerHide[i] = false;
    }

//--- Navigator ---
    Navigator = false;
    TrackerSound = false;           // Пищать при отклонении от курса более TrackerAlarmMetr
    TrackerOversow = qreal(0.5);    // пересев
    TrackerUndersow = qreal(0.0);   // недосев
    ShowAlarm = true;
    LengGPS         = qreal(3.0);   // длина(м) от GPS до сцепки
    LengBunker      = qreal(6.0);   // длина(м) бункера с дышлом
    LengCultivator  = qreal(4.0);   // длина(м) культиватора с дышлом
    SchemeAgregate  = Options::Scheme_AgrCultBunk;
    TypeWork        = TypeSow;      // Вид обработки: сев, вспашка, боронование
    PlugNavigManual = false;        // управление заглублением от опрыскивателя (режим НАВИГАТОР)

    GpsPort = "mxc1";               // Порт GPS-приемника
    GpsSpeed = "115200";            // Скорость GPS-приемника
//--- Internet ---
    sendRealTime    = SendRealTime_off; // передаем автограф и раз в час протоколы
    SendJsonFormat  = false;        // Передавать в формате JSON (realTime или Автограф)
    StoreRealTime   = 0;            // Маска сохранения протоколов в режиме реального времени
    indexProviderSim0   = 0;        // mts
    indexProviderSim1   = 1;        // megafon
    tcpIpServer         = std::string("127.0.0.1");
    owner               = std::string("ИВАНОВ");
    password            = std::string("123");
    agrName             = std::string("К700");
    agrNameTv           = std::string("К700");
    PortModem           = "mxc4";   // порт GSM-модема
    PortModemSpeed      = "115200"; // скорость порта GSM-модема
    GprsReduction       = 0;        // Уменьшение выходной мощности GSM-модуля
    PauseAfterTransmit  = 0;        // Пауза после передачи, мин

//--- Service Окно из плуга ---
    volumeProcent       = 80;       // процент громкости
    brightnessProcent   = 100;      // процент яркости
    sowNormaAutoAverage = false;    // усреднять норму высева автоматическую (при отображении)
    sowNormaMedianaIdx  = 0;        // индекс размера медианы для "усреднять норму высева (при отображении)"
    sowNormaAverageJson = false;    // усреднять норму высева (при передаче в Json-формате)
    sowNormaJsonSendSec = 1;        // норму высева в Json-формате передавать не чаще, сек
    SowNormaMedianaActuator = 1;    // значение усреднения нормы высева для управления актуатором
    SowNormaAverageMediana = false; // тип усреднения нормы высева для управления актуатором - true -Медиана, иначе Арифметическое среднее
    traficWrite         = false;    // писать входной трафик
    logErrWrite         = false;    // писать лог ошибок, входной трафик 5сек до ошибки и 10 сек после
    errorIgnore         = true;     // игнорировать ошибки датчиков
    for (int i = 0; i < COUNT_OF(valImpCorr); i++) {
        valImpCorr[i] = 0;
    }
    sowNormaMode        = SowNorma_Off;    // Контроль нормы высева выключен
    SowNormaProcent     = 25;       // Процент нормы высева
    LargeButtons        = false;    // Большие сенсорные кнопки (если физически кнопки неисправны, можно пользоваться сенсорными увеличенными кнопками)
    StaticSensorsOn     = false;    // В состоянии заглубления каждую секунду передавать в контроллер нулевые датчики засорений
    ModeService         = false;    // Отладка нормы и площади высева
    BrightnessScreen    = 100;      // Яркость экрана в процентах
    BrightnessButtons   = 100;      // Яркость кнопок в процентах

    indexBrightnessScreen   = 0;    // Яркость экрана в процентах
    indexBrightnessButtons  = 0;    // Яркость кнопок в процентах
    procentScreen100    = 100;
    procentScreen80     = 80;
    procentScreen60     = 60;
    procentScreen40     = 40;
    procentScreen20     = 20;
    procentScreen10     = 10;
    procentButtons100   = 100;
    procentButtons80    = 80;
    procentButtons60    = 60;
    procentButtons40    = 40;
    procentButtons20    = 20;
    procentButtons10    = 10;

    // Жидкие удобрения
    VolumeCount = 1;                        // Количество емкостей с жидкими удобрениями
    for (int i = 0; i < COUNT_OF(IsSensorPress); i++) {
        IsSensorPress[i]    = true;         // есть датчик давления
        IsFlowmeter[i]      = true;         // есть расходомер
        SensorPressMax[i]   = qreal(3);     // максимальное давление датчика
        FlowmeterMl2imp[i]  = qreal(12.0);  // цена импульса расходомера
        LiquidNameIndex[i]  = -1;           // индекс имени жидких удобрений
    }
    LiquidSound     = true;                 // Звук при превышении давления
    LiquidShowEvent = true;                 // Переключать с основного окна при событии
    for (int i = 0; i < COUNT_OF(nameLiquid); i++) {
        nameLiquid[i]   = "";   // наименование жидких удобрений
    }
    printf(" OK\n");
}

void    Options::CalibrateCultureDiff_clear()
{
    for(int val = 0; val < COUNT_OF(SowNormaIndexCultureDiff); val++) {
        SowNormaIndexCultureDiff[val]   = -1;    // номер культуры от 0 для каждого вала
    }
    for(int val = 0; val < COUNT_OF(SowNormaBaseKgDiff); val++) {
        for(int j = 0; j < COUNT_OF(SowNormaBaseKgDiff[0]); j++) {
            SowNormaBaseKgDiff  [val][j]  = qreal(0);     // Норма высева расчетная (кг)
            SowNormaBaseImpDiff [val][j]  = 0;            // Норма высева расчетная (импульсов)
            SowNormaAutoKgDiff  [val][j]  = qreal(0);     // Норма высева расчетная (кг)
            SowNormaAutoImpDiff [val][j]  = 0;            // Норма высева расчетная (импульсов)
        }
    }
}

double Options::Sow_Width()     // Ширина высева (метр)
{
    if (AgroNavig1()) {
        if (isSowWidth) return SowWidth;
        else            return LeftWidth + RightWidth;
    } else {
        return SowWidth;
    }
}
double Options::SowWidth_subOver()
{
    if (AgroTv() || AgroTv2()) {
        return SowWidth;
    }
    return Sow_Width() - SowOver;
}
double Options::LeftWidth_subOver()
{
    if (AgroTv() || AgroTv2()) {
        return SowWidth/qreal(2.0);
    }
    if (AgroNavig1()) {
        if (isSowWidth) {
            return (SowWidth - SowOver)/qreal(2.0);
        } else {
            return LeftWidth - SowOver/qreal(2.0);
        }
    } else {
        return (SowWidth - SowOver)/qreal(2.0);
    }
}
double Options::RightWidth_subOver()
{
    if (AgroTv() || AgroTv2()) {
        return SowWidth/qreal(2.0);
    }
    if (AgroNavig1()) {
        if (isSowWidth) {
            return (SowWidth - SowOver)/qreal(2.0);
        } else {
            return RightWidth - SowOver/qreal(2.0);
        }
    } else {
        return (SowWidth - SowOver)/qreal(2.0);
    }
}

// Возвращает сквозной номер семяпровода от 1 (agroTv)
int Options::GetNumberCrops(int im, int is)
{
    if (im >= ModCropsCount) {
        return 0;
    }
    if (is >= SensorCropsCount[im]) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < im; i++) {
        count += SensorCropsCount[i];
    }
    return count + is + 1;
}
// Возвращает сквозной номер удобрениевода от 1 (agroTv)
int Options::GetNumberManure(int im, int is)
{
    if (im >= ModManureCount) {
        return 0;
    }
    if (is >= SensorCropsCount[im]) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < im; i++) {
        count += SensorManureCount[i];
    }
    return count + is + 1;
}

// возвращает установленный шаг высева в см
int Options::SowNormaTv_StepSowSm() { return !SowNormaTv_Exists() ? 0 : agro_Round(qreal(100) / SowNormaTv_Count2m); }

// возвращает установленный(калиброванный) интервал между зернами в мсек для скорости m2s
double Options::SowNormaTv_MsecNorma(double m2s) { return qreal(1000) / (SowNormaTv_Count2m * m2s); }

// Норма высева штук/сек для данной скорости
double Options::SowNormaTv_Count2s(double m2s)              { return SowNormaTv_Count2m * m2s;  }

// Норма высева штук/сек для данной скорости и длительности
double Options::SowNormaTv_Count2s(double m2s, int msec)    { return SowNormaTv_Count2m * (m2s * (double)msec / qreal(1000)); }

// Вернуть true, если Актуатор калиброван (скорость по Энкодеру)
bool    Options::ActuatorCalibrated(uchar val)
{
    if (val >= COUNT_OF(Imp2metr)) {
        return false;
    }
    return Imp2metr[val][cIntervals] > qreal(0.001) ? true : false;
}
// Сброс калибровки актуаторов
void    Options::ActuatorsCalibrate_clear()
{
    for (size_t val = 0; val < COUNT_OF(Potencio); val++) {
        for (size_t i = 0; i < cIntervals + 1; i++) {
            Potencio[val][i] = 0;
            Imp2metr[val][i] = qreal(0);
        }
    }
}

ushort Options::i2m_to_potenc(uchar val, double i2m)
{
    ushort potenc = 0;
    if (ActuatorCalibrated(val)) {      // Актуатор калиброван
        if (i2m <= Imp2metr[val][0]) {
            potenc = Potencio[val][0];
        } else if (i2m >= Imp2metr[val][cIntervals]) {
            potenc = Potencio[val][cIntervals];
        } else {
            for (int i = 0; i < cIntervals; i++) {
                if (i2m >= Imp2metr[val][i] && i2m < Imp2metr[val][i+1]) {
                    double dd;
                    if (Imp2metr[val][i+1] - Imp2metr[val][i] < qreal(0.001)) {
                        dd = qreal(0);
                    } else {
                        double kk = i2m - Imp2metr[val][i];
                        dd = (double)(Potencio[val][i+1] - Potencio[val][i])/(Imp2metr[val][i+1] - Imp2metr[val][i])*kk;
                    }
                    potenc = Potencio[val][i] + (ushort)dd;
                    //printf(" k=%d. pot=%d  kg2ga=%.1f\n", k, potenc, kg2ga);
                    break;
                }
            }
        }
    }
    return potenc;
}

ushort Options::kg2ga_to_potenc(uchar val, double kg2ga)
{
    ushort potenc = 0;
    if (ActuatorCalibrated(val)) {      // Актуатор калиброван
        double norma[cIntervals + 1];
        for (int i = 0; i < cIntervals + 1; i++) {
            norma[i] = Imp2metr[val][i]/Imp2metr[val][cIntervals]*SowNormaMax;
        }

        if (kg2ga <= norma[0]) {
            potenc = norma[0];
        } else if (kg2ga >= norma[cIntervals]) {
            potenc = Potencio[val][cIntervals];
        } else {
            for (int i = 0; i < cIntervals; i++) {
                if (kg2ga >= norma[i] && kg2ga < norma[i+1]) {
                    double dd;
                    if (norma[i+1] - norma[i] < qreal(0.001)) {
                        dd = qreal(0);
                    } else {
                        double kk = kg2ga - norma[i];
                        dd = (double)(Potencio[val][i+1] - Potencio[val][i])/(norma[i+1] - norma[i])*kk;
                    }
                    potenc = Potencio[val][i] + (ushort)dd;
                    //printf(" k=%d. pot=%d  kg2ga=%.1f\n", k, potenc, kg2ga);
                    break;
                }
            }
        }
    }
    return potenc;
}
// При "Ручной установке нормы высева" значение потенциометра должно быть не меньше
// первого значения потенциометра с имп/м > 0.
// Возвращает минимальное значение потенциометра при установке нормы высева
ushort Options::potenc_min_forSetNorma(uchar val)
{
    ushort potenc = 0;
    if (ActuatorCalibrated(val)) {      // Актуатор калиброван
        for (int i = 0; i < cIntervals; i++) {
            if (Imp2metr[val][i] > 0) {
                potenc = Potencio[val][i];
                break;
            }
        }
    }
    return potenc;
}

ushort Options::Potenc_for_kg2ga_forCulture(uchar val, double kg2ga, int iCulture)
{
    static  char    func[] = "Potenc_for_kg2ga_forCulture";
    ushort potenc = Potencio[val][0];
    double kg2imp;
    bool ok = SowNormaKg2imp(val, iCulture, kg2imp);
    //if (val==0) {
        printf("val=%d. %s. kg2ga=%.1f  iCult=%d   kg2imp=%f\n", val, func, kg2ga, iCulture, kg2imp);
    //}
    if (!ok) {     // нет калибровки культуры
        printf("val=%d Culture not calibrated  potenc=%d\n", val, potenc);
        return potenc;
    }
    double kg2metrP = kg2ga/qreal(10000)*options.SowWidth;     // масса на погонный метр
    double imp2metrP = kg2metrP/kg2imp;
    potenc = i2m_to_potenc(val, imp2metrP);
    //if (val==0) {
        printf("val=%d. %s. potenc=%d\n", val, func, potenc);
    //}
    return potenc;
}

// Возвращает значение потенциометра и импульсов/метр для установленной нормы высева. Управление актуатором по потенциометру
void Options::Potenc_and_Imp2metr_forSowNormaSet(uchar val, double km2h, ushort& potenc, double& i2m)
{
    potenc  = 0;
    i2m     = qreal(0);
    if (val >= options.ValsCount) {
        return;
    }
    if (!ActuatorCalibrated(val)) { // Актуатор не калиброван
        printf("val=%d Actuator not calibrated\n", val);
        return;
    }
    int iCulture = SowNormaIndexCulture_get(val);
    if (iCulture < 0) {
        return;
    }
    double kg2imp;
    bool ok = SowNormaKg2imp(val, iCulture, kg2imp);
    if (!ok) {        // нет калибровки культуры
        return;
    }
    double m2sec = km2h_To_m2s(km2h);           // пройдено м/сек на скорости km2h
    double ga2sec = m2sec * Sow_Width() / qreal(10000); // площадь (га): m2s * width_m / 10000
    double kg2sec = SowNormaSet[val]*ga2sec;    // норма в кг за секунду на данной скорости
    double imp2sec = kg2sec/kg2imp;             // импульсов за секунду
    i2m = imp2sec/m2sec;                        // импульсов на метр для установленной нормы на данной скорости
    potenc = i2m_to_potenc(val, i2m);
}

// Переводит значение нормы (кг/га) в значение Потенциометра
ushort Options::SowNormaSet_toPotenc(int val)
{
    ushort potenc = 0;
    if (val >= options.ValsCount) {
        return potenc;
    }
    if (val >= COUNT_OF(SowNormaSet)) {
        return potenc;
    }
    double norma = SowNormaSet[val];
    if (options.SowNormaMin15 && (norma < qreal(15))) {
        norma = qreal(15);
    }
    if (options.SpeedMethodEncoder() || options.SpeedMethodGPS()) {
        int iCulture = options.SowNormaIndexCulture_get(val);
        if (iCulture >= 0) {
            potenc = Potenc_for_kg2ga_forCulture(val, norma, iCulture); // Значение потенциометра для указанной нормы высева
            ushort potencMin = potenc_min_forSetNorma((uchar)val);      // Возвращает минимальное значение потенциометра при установке нормы высева
            if (potenc < potencMin) {
                potenc = potencMin;
                //if (val== 0) {
                    printf("val=%d. potencMin=%d\n", val, potencMin);
                //}
            }
        }
    }
    printf("val=%d. norma=%.1f -> potenc=%d\n", val, norma, potenc);
    return potenc;
}

int     Options::SowNormaIndexCulture_get(uchar val) // номер культуры от 0 для каждого вала
{
    if (DifEntry) {
        if (val < COUNT_OF(SowNormaIndexCultureDiff)) {
            return SowNormaIndexCultureDiff[val];
        } else {
            return -1;
        }
    } else {
        if (val < COUNT_OF(SowNormaIndexCulture)) {
            return SowNormaIndexCulture[val];
        } else {
            return -1;
        }
    }
}
void    Options::SowNormaIndexCulture_set(uchar val, int iCulture)    // номер культуры от 0 для каждого вала
{
    if (DifEntry) {
        if ((val < COUNT_OF(SowNormaIndexCultureDiff)) && (iCulture < cMaxCountCulture)) {
            SowNormaIndexCultureDiff[val] = iCulture;
        }
    } else {
        if ((val < COUNT_OF(SowNormaIndexCulture)) && (iCulture < cMaxCountCulture)) {
            SowNormaIndexCulture[val] = iCulture;
        }
    }
}

int     Options::SowNormaImp_get(uchar val, int iCulture)
{
    int imp = 0;
    if (val >= options.ValsCount) {
        return imp;
    }
    if (iCulture < 0) { // культура не назначена или не калибрована
        return imp;
    }
    if (SowNormaBase()) {
        if (options.DifEntry) {
            if (val < COUNT_OF(SowNormaBaseImpDiff)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaBaseImpDiff[0]))) {
                    imp = SowNormaBaseImpDiff[val][iCulture];
                }
            }
        } else {
            if ((val < COUNT_OF(SowNormaBaseImp)) && (iCulture < COUNT_OF(SowNormaBaseImp[0]))) {
                imp = SowNormaBaseImp[val][iCulture];
            }
        }
    } else if (SowNormaAuto()) {
        if (options.DifEntry) {
            if ((val < COUNT_OF(SowNormaAutoImpDiff)) && (iCulture < COUNT_OF(SowNormaAutoImpDiff[0]))) {
                imp = SowNormaAutoImpDiff[val][iCulture];
            }
        } else {
            if ((val < COUNT_OF(SowNormaAutoImp)) && (iCulture < COUNT_OF(SowNormaAutoImp[0]))) {
                imp = SowNormaAutoImp[val][iCulture];
            }
        }
    }
    return imp;
}
int     Options::SowNormaImp_get(uchar val)
{
    int imp = 0;
    if (val < options.ValsCount) {
        int iCulture = SowNormaIndexCulture_get(val);
        if (iCulture >= 0) {
            imp = SowNormaImp_get(val, iCulture);
        }
    }
    return imp;
}

void    Options::SowNormaImp_set(uchar val, int iCulture, int imp)
{
    if (val >= options.ValsCount) {
        return;
    }
    if (iCulture < 0) { // культура не назначена или не калибрована
        return;
    }
    if (SowNormaBase()) {
        if (options.DifEntry) {
            if ((val < COUNT_OF(SowNormaBaseImpDiff)) && (iCulture < COUNT_OF(SowNormaBaseImpDiff[0]))) {
                SowNormaBaseImpDiff[val][iCulture] = imp;
            }
        } else {
            if ((val < COUNT_OF(SowNormaBaseImp)) && (iCulture < COUNT_OF(SowNormaBaseImp[0]))) {
                SowNormaBaseImp[val][iCulture] = imp;
            }
        }
    } else if (SowNormaAuto()) {
        if (options.DifEntry) {
            if ((val < COUNT_OF(SowNormaAutoImpDiff)) && (iCulture < COUNT_OF(SowNormaAutoImpDiff[0]))) {
                SowNormaAutoImpDiff[val][iCulture] = imp;
            }
        } else {
            if ((val < COUNT_OF(SowNormaAutoImp)) && (iCulture < COUNT_OF(SowNormaAutoImp[0]))) {
                SowNormaAutoImp[val][iCulture] = imp;
            }
        }
    }
}
double  Options::SowNormaKg_get(uchar val, int iCulture)
{
    double kg = qreal(0);
    if (val >= options.ValsCount) {
        return kg;
    }
    if (SowNormaBase()) {
        if (options.DifEntry) {
            if (val < COUNT_OF(SowNormaBaseKgDiff)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaBaseKgDiff[0]))) {
                    kg = SowNormaBaseKgDiff[val][iCulture];
                }
            }
        } else {
            if (val < COUNT_OF(SowNormaBaseKg)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaBaseKg[0]))) {
                    kg = SowNormaBaseKg[val][iCulture];
                }
            }
        }
    } else if (SowNormaAuto()) {
        if (options.DifEntry) {
            if (val < COUNT_OF(SowNormaAutoKgDiff)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaAutoKgDiff[0]))) {
                    kg = SowNormaAutoKgDiff[val][iCulture];
                }
            }
        } else {
            if (val < COUNT_OF(SowNormaAutoKg)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaAutoKg[0]))) {
                    kg = SowNormaAutoKg[val][iCulture];
                }
            }
        }
    }
    return kg;
}
double  Options::SowNormaKg_get(uchar val)
{
    double kg = qreal(0);
    if (val < options.ValsCount) {
        int iCulture = SowNormaIndexCulture_get(val);
        if (iCulture >= 0) {
            kg = SowNormaKg_get(val, iCulture);
        }
    }
    return kg;
}

void    Options::SowNormaKg_set(uchar val, int iCulture, double kg)
{
    if (val >= options.ValsCount) {
        return;
    }
    if (SowNormaBase()) {
        if (options.DifEntry) {
            if (val < COUNT_OF(SowNormaBaseKgDiff)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaBaseKgDiff[0]))) {
                    SowNormaBaseKgDiff[val][iCulture] = kg;
                }
            }
        } else {
            if (val < COUNT_OF(SowNormaBaseKg)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaBaseKg[0]))) {
                    SowNormaBaseKg[val][iCulture] = kg;
                }
            }
        }
    } else if (SowNormaAuto()) {
        if (options.DifEntry) {
            if (val < COUNT_OF(SowNormaAutoKgDiff)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaAutoKgDiff[0]))) {
                    SowNormaAutoKgDiff[val][iCulture] = kg;
                }
            }
        } else {
            if (val < COUNT_OF(SowNormaAutoKg)) {
                if ((iCulture >= 0) && (iCulture < COUNT_OF(SowNormaAutoKg[0]))) {
                    SowNormaAutoKg[val][iCulture] = kg;
                }
            }
        }
    }
}
bool  Options::SowNormaKg2imp(uchar val, int iCulture, double& kg2imp)
{
    int imp = SowNormaImp_get(val, iCulture);
    if (!imp) {     // культура не калибрована
        kg2imp = qreal(0);
        return false;
    }
    double kg = SowNormaKg_get(val, iCulture);
    kg2imp = kg / (double)imp;
    return true;
}

bool    Options::SowNorma_exists(uchar val)
{
    if (val >= COUNT_OF(SowNormaSet)) {
        return false;
    }
    return SowNormaSet[val] > qreal(0.001) ? true : false;
}

bool    Options::Culture_calibrated(uint val, int iCulture)
{
    if (iCulture >= 0) {
        if (SowNormaBase()) {
            if (options.DifEntry) {
                if ((val < COUNT_OF(SowNormaBaseKgDiff)) && (iCulture < COUNT_OF(SowNormaBaseKgDiff[0]))) {
                    return (SowNormaBaseKgDiff[val][iCulture] > qreal(0.01)) && (SowNormaBaseImpDiff[val][iCulture] > 0);
                }
            } else {
                if ((val < COUNT_OF(SowNormaBaseKg)) && (iCulture < COUNT_OF(SowNormaBaseKg[0]))) {
                    return (SowNormaBaseKg[val][iCulture] > qreal(0.01)) && (SowNormaBaseImp[val][iCulture] > 0);
                }
            }
        } else if (SowNormaAuto()) {
            if (options.DifEntry) {
                if ((val < COUNT_OF(SowNormaAutoKgDiff)) && (iCulture < COUNT_OF(SowNormaAutoKgDiff[0]))) {
                    return (SowNormaAutoKgDiff[val][iCulture] > qreal(0.01)) && (SowNormaAutoImpDiff[val][iCulture] > 0);
                }
            } else {
                if ((val < COUNT_OF(SowNormaAutoKg)) && (iCulture < COUNT_OF(SowNormaAutoKg[0]))) {
                    return (SowNormaAutoKg[val][iCulture] > qreal(0.01)) && (SowNormaAutoImp[val][iCulture] > 0);
                }
            }
        }
    }
    return false;
}

bool    Options::Culture_calibrated(uint val)
{
    bool calibrated = false;
    int iCulture = options.SowNormaIndexCulture_get(val);   // номер от 0 культуры
    if (iCulture >= 0) {
        calibrated = options.Culture_calibrated(val,iCulture);
    }
    return calibrated;
}

const char* Options::SowNormaBase_state(uint val, double kg2ga)
{
    if (val >= COUNT_OF(SowNormaSet)) {
        return "norm";
    }
    double normaSet = SowNormaSet[val];
    double delta = normaSet / qreal(100.0) * (double)SowNormaProcent;
    if (kg2ga > normaSet + delta) {     // Больше (norma + 25%)
        return "up";
    }
    if (kg2ga < normaSet - delta) {     // Меньше (norma - 25%)
        return "dn";
    }
    return "norm";
}

// длина агрегата от GPS-приемника до культиватора
double Options::LengthTrainToCultivator()
{
    switch(SchemeAgregate) {
    case Options::Scheme_AgrCultBunk:   return LengGPS + LengCultivator;
    case Options::Scheme_AgrBunkCult:   return LengGPS + LengBunker + LengCultivator;
    case Options::Scheme_Agr:
    default:                            return LengGPS + LengCultivator;
    }
}

void    Options::setDiskData(const std::string& path)  {   diskData = path; }
std::string Options::pathData()                 {   return diskData + "/DATA";               }
std::string Options::shortNameFileClientLog()   {   return "client22.log";     }
static  char    nameClientLogDebug[] = "client22_0.log";
static  int     nameClientLogDebugIndex = 0;
std::string Options::shortNameFileClientLogDebug()
{
    nameClientLogDebug[9] = '0' + nameClientLogDebugIndex;
    if (++nameClientLogDebugIndex >= 10) {
        nameClientLogDebugIndex = 0;
    }
    return nameClientLogDebug;
}
std::string Options::nameFileAgroClientLog()        {   return pathData() + "/" + shortNameFileClientLog();       }
std::string Options::nameFileAgroClientLogDebug()   {   return pathData() + "/" + shortNameFileClientLogDebug();  }
std::string Options::nameFileContrErrorsLog()       {   return pathData() + "/contr_errors.log";             }

std::string Options::nameFileDayLog(const std::string& path, time_t date)
{
    TimeDecode td(date);
    char ss[64];
    snprintf(ss, sizeof(ss), "/%04d_%02d_%02d.txt", td.Year(), td.Mon(), td.Day());
    return path + std::string(ss);
}
std::string Options::nameFileDayLog(const std::string& path)    { return Options::nameFileDayLog(path, time(0)); }

std::string Options::pathOptionsForSend()   { return pathData() + "/OptionsForSend"; }   // папка файлов для xml-файла настроек после изменения
std::string Options::pathSend()             { return pathData() + "/Send";           }   // папка файлов для передачи на сервер
std::string Options::pathSended()           { return pathData() + "/Sended";         }   // папка переданных файлов на сервер
std::string Options::pathRecvTmp()          { return pathData() + "/RecvTmp";        }   // папка принимаемого файла
std::string Options::pathRecv()             { return pathData() + "/Recv";           }   // папка принятых файлов с сервер
std::string Options::pathCoordinate()       { return pathData() + "/Coordinate";     }
std::string Options::pathTracks()           { return pathData() + "/Tracks";         }   // треки для сервера
std::string Options::pathTracks_Sow()       { return pathData() + "/Tracks_Sow";     }
std::string Options::pathTracks_Plough()    { return pathData() + "/Tracks_Plough";  }
std::string Options::pathTracks_Harrow()    { return pathData() + "/Tracks_Harrow";  }
std::string Options::pathTracks_SPH()       // треки текущего режима работы: сев, вспашка или обработка
{
    if (AgroTv() || AgroTv2())  return pathTvTracks_Sow();
    if (TypeWork == TypeSow)    return pathTracks_Sow();
    if (TypeWork == TypePlough) return pathTracks_Plough();
    return pathTracks_Harrow();
}
std::string Options::pathTracks_depth()     // треки текущего режима работы с значениями датчиков глубины
{
    char    ss[32];
    snprintf(ss, sizeof(ss), "/agr%d", options.SowDepthAgrIndex);
    return pathTracks_SPH() + std::string(ss);
}

const char* Options::ModeSowNorma_toString(int mode) {
    switch (mode) {
    case SowNorma_Base:     return "Базовая";
    case SowNorma_Auto:     return "Автоматическая";
    case SowNorma_Off:
    default:                return "Нет";
    }
}
std::string Options::SowNormaKg_toString(double kg)
{
    char ss[16];
    snprintf(ss, sizeof(ss), "%.2f", kg);
    return ss;
}

std::string Options::NameCulture(int val, int index)   // Возвращает имя посевной культуры (для автоматической нормы высева)
{
    std::string name = "";
    if (DifEntry) {
#ifdef  IMX6_SODIMM_
        if ((index >= 0) && (index < (int)listShapes.listProductNames.size())) {
            name = listShapes.listProductNames.at(index);
        }
#endif
    } else {
        if ((val >= 0) && (val < COUNT_OF(nameCulture))) {
            if ((index >= 0) && (index < cMaxCountCulture)) {
                name = nameCulture[val][index];
            }
        }
    }
    return name;
}

std::string Options::getNameCulture(int val)   // Возвращает имя посевной культуры
{
    std::string name = "";
    if (DifEntry) {
#ifdef  IMX6_SODIMM_
        if (val >= 0 && val < COUNT_OF(SowNormaIndexCultureDiff)) {
            int index = SowNormaIndexCultureDiff[val];
            if (index >= 0 && index < (int)listShapes.listProductNames.size()) {
                name = listShapes.listProductNames.at(index);
            }
        }
#endif
    } else {
        if (val >= 0 && val < COUNT_OF(SowNormaIndexCulture)) {
            int index = SowNormaIndexCulture[val];
            if (index >= 0 && index < cMaxCountCulture) {
                name = nameCulture[val][index];
            }
        }
    }
    return name;
}

void Options::setNameCulture(int val, int index, const std::string &name)  // Установить имя посевной культуры
{
    if (DifEntry) {     // В режиме ДИФФЕРЕНЦИАЛЬНОЕ ВНЕСЕНИЕ имя культуры не меняем
        return;
    }
    if (val < 0 || val >= COUNT_OF(nameCulture)) {
        return;
    }
    if (index < 0 || index >= cMaxCountCulture) {
        return;
    }
    nameCulture[val][index] = name;
}

std::string Options::NameLiquid(int index)   // Возвращает имя жидких удобрений
{
    if (index < 0 || index >= COUNT_OF(nameLiquid)) {
        return "";
    }
    return nameLiquid[index];
}
void Options::setNameLiquid(int index, const std::string &name)  // Установить имя жидких удобрений
{
    if (index < 0 || index >= COUNT_OF(nameLiquid)) {
        return;
    }
    nameLiquid[index] = name;
}

//---------Options------------
std::string Options::pathSound()            { return "/home/user/Sound/";   }

std::string Options::pathArea()             { return pathData() + "/Area";           }
std::string Options::pathGisto()            { return pathData() + "/Gistogramma";    }
std::string Options::pathSowNorma()         { return pathData() + "/SowNorma";       }
std::string Options::pathSowNormaAlarm()    { return pathData() + "/SowNormaAlarm";  }
std::string Options::pathOptionsChange()    { return pathData() + "/OptionsChange";  }
std::string Options::pathBunker()           { return pathData() + "/Bunker";         }
std::string Options::pathSemens()           { return pathData() + "/Semens";         }
std::string Options::pathSpeedSowMax()      { return pathData() + "/SpeedSowMax";    }
std::string Options::pathFanBaudMax()       { return pathData() + "/FanBaudMax";     }
std::string Options::pathPressure()         { return pathData() + "/Pressure";       }
std::string Options::pathValRotate()        { return pathData() + "/ValRotate";      }
std::string Options::pathLoops()            { return pathData() + "/Loops";          }
std::string Options::pathRealTime()         { return pathData() + "/RealTime";       }

std::string Options::pathDebug()            { return pathData() + "/Debug";          }
std::string Options::pathGps()              { return pathData() + "/Gps";            }
std::string Options::pathMap()              { return pathData() + "/Map";            }
std::string Options::pathLogErrors()        { return pathData() + "/LogErrors";      }

std::string Options::pathTvArea()           { return pathData() + "/tvArea";         }
std::string Options::pathTvGisto()          { return pathData() + "/tvGistogramma";  }
std::string Options::pathTvSemsGa()         { return pathData() + "/tvSemsGa";       }
std::string Options::pathTvStartup()        { return pathData() + "/tvStartup";      }
std::string Options::pathTvOptionsChange()  { return pathData() + "/tvOptionsChange";}
std::string Options::pathTvBunkerCrops()    { return pathData() + "/tvBunkerCrops";  }
std::string Options::pathTvBunkerManure()   { return pathData() + "/tvBunkerManure"; }
std::string Options::pathTvSemens()         { return pathData() + "/tvSemens";       }
std::string Options::pathTvManure()         { return pathData() + "/tvManure";       }
std::string Options::pathTvDoubles()        { return pathData() + "/tvDoubles";      }
std::string Options::pathTvSpeedSowMax()    { return pathData() + "/tvSpeedSowMax";  }
std::string Options::pathTvTracks_Sow()     { return pathData() + "/tvTracks_Sow";   }
std::string Options::pathTvLoops()          { return pathData() + "/tvLoops";        }

static  std::string xml_parse_str(const char* ps, const char* def)  {   return ps ? ps : def;               }
static  int         xml_parse_int(const char* ps, int def)          {   return ps ? (int)strtoul(ps,0,0) : def;  }
static  bool        xml_parse_bool(const char* ps, bool def)        {   return ps ? (atoi(ps) ? true : false) : def;  }
static  double      xml_parse_dbl(const char* ps, double def)    {   return ps ? atof(ps) : def;         }
static  void        xml_parse_array_int(const char* ps, int* ar, int size, int def)
{
    if (ps) {
        char *aa[32];
        char* pd = strdup(ps);
        int cnt = util_split(pd, ' ', aa, COUNT_OF(aa), true);
        for (int i = 0; i < size; i++) {
            ar[i] = (i < cnt) ? atoi(aa[i]) : def;
        }
        free(pd);
    } else {
        for (int i = 0; i < size; i++) {
            ar[i] = def;
        }
    }
}
static  void        xml_parse_array_bool(const char* ps, bool* ar, int size, bool def)
{
    if (ps) {
        char *aa[32];
        char* pd = strdup(ps);
        int cnt = util_split(pd, ' ', aa, COUNT_OF(aa));
        for (int i = 0; i < size; i++) {
            ar[i] = (i < cnt) ? atoi(aa[i]) ? true : false : def;
        }
        free(pd);
    } else {
        for (int i = 0; i < size; i++) {
            ar[i] = def;
        }
    }
}
static  void        xml_parse_array_ushort(const char* ps, ushort* ar, int size, ushort def)
{
    if (ps) {
        char *aa[32];
        char* pd = strdup(ps);
        int cnt = util_split(pd, ' ', aa, COUNT_OF(aa));
        for (int i = 0; i < size; i++) {
            ar[i] = (i < cnt) ? atoi(aa[i]) : def;
        }
        free(pd);
    } else {
        for (int i = 0; i < size; i++) {
            ar[i] = def;
        }
    }
}
static  void        xml_parse_array_dbl(const char* ps, double* ar, int size, double def)
{
    if (ps) {
        char *aa[32];
        char* pd = strdup(ps);
        int cnt = util_split(pd, ' ', aa, COUNT_OF(aa));
        for (int i = 0; i < size; i++) {
            ar[i] = (i < cnt) ? atof(aa[i]) : def;
        }
        free(pd);
    } else {
        for (int i = 0; i < size; i++) {
            ar[i] = def;
        }
    }
}
static  void        xml_parse_array_str1251(const char* ps, std::string* ar, int size, const char* def)
{
    if (ps) {
        char *aa[cValsMaxCount*cMaxCountCulture];
        char* pd = strdup(ps);
        int cnt = util_split(pd, '_', aa, COUNT_OF(aa));
        for (int i = 0; i < size; i++) {
            ar[i] = (i < cnt) ? std_cp1251_to_utf8(aa[i]) : def;
        }
        free(pd);
    } else {
        for (int i = 0; i < size; i++) {
            ar[i] = def;
        }
    }
}

void    Options::xmlObject_parse(const char* name, const char* value)
{
         if (!strcmp(name,"AgroMode"))              AgroMode        = xml_parse_int(value, AgroModeSow);
    else if (!strcmp(name,"AgroDemo"))              AgroDemo        = xml_parse_int(value, 0);
    else if (!strcmp(name,"sound_off"))             sound_off       = xml_parse_int(value, 0);
// Common
    else if (!strcmp(name,"BunkersCount"))          BunkersCount    = xml_parse_int(value, 2);
    else if (!strcmp(name,"SectionsCount"))         SectionsCount   = xml_parse_int(value, 6);
    else if (!strcmp(name,"TowersCount"))           TowersCount     = xml_parse_int(value, 6);

    else if (!strcmp(name,"SemensCount"))           xml_parse_array_int(value, SemensCount, COUNT_OF(SemensCount),  0);
    else if (!strcmp(name,"CropsType"))             xml_parse_array_int(value, CropsType,   COUNT_OF(CropsType),    0);

    else if (!strcmp(name,"FansCount"))             FansCount           = xml_parse_int(value, 1);
    else if (!strcmp(name,"FanImpulsCount"))        xml_parse_array_int(value, FanImpulsCount,  COUNT_OF(FanImpulsCount),   1);
    else if (!strcmp(name,"ValsCount"))             ValsCount           = xml_parse_int(value, 2);
    else if (!strcmp(name,"ModCropsCount"))         ModCropsCount       = xml_parse_int(value, 2);       // кол-во модулей зерна
    else if (!strcmp(name,"ModManureCount"))        ModManureCount      = xml_parse_int(value, 2);      // кол-во модулей удобрений
    else if (!strcmp(name,"SensorCropsCount"))      xml_parse_array_int(value,  SensorCropsCount,   COUNT_OF(SensorCropsCount),     1);
    else if (!strcmp(name,"SensorManureCount"))     xml_parse_array_int(value,  SensorManureCount,  COUNT_OF(SensorManureCount),    1);
    else if (!strcmp(name,"ControlLevelCrops"))     ControlLevelCrops   = xml_parse_bool(value, false);
    else if (!strcmp(name,"ControlLevelManure"))    ControlLevelManure  = xml_parse_bool(value, false);

    else if (!strcmp(name,"SensorCountTv2"))        SensorCountTv2      = xml_parse_int(value, 12);
    else if (!strcmp(name,"isEncoderTv2"))          isEncoderTv2        = xml_parse_bool(value, false);
    else if (!strcmp(name,"isFanTv2"))              isFan1Tv2           = xml_parse_bool(value, false);
    else if (!strcmp(name,"isFan2Tv2"))             isFan2Tv2           = xml_parse_bool(value, false);
    else if (!strcmp(name,"imp2cycleFanTv2"))       imp2cycleFanTv2     = xml_parse_int(value, 10);
    else if (!strcmp(name,"imp2cycleFan2Tv2"))      imp2cycleFan2Tv2    = xml_parse_int(value, 10);
    else if (!strcmp(name,"isPressTv2"))            isPressTv2          = xml_parse_bool(value, false);
    else if (!strcmp(name,"isPress2Tv2"))           isPress2Tv2         = xml_parse_bool(value, false);
    else if (!strcmp(name,"isActuatorTv2"))         isActuatorTv2       = xml_parse_bool(value, false);

    else if (!strcmp(name,"isSowWidth"))            isSowWidth          = xml_parse_bool(value, false);
    else if (!strcmp(name,"SowWidth"))              SowWidth            = xml_parse_dbl(value, qreal(10));
    else if (!strcmp(name,"LeftWidth"))             LeftWidth           = xml_parse_dbl(value, qreal(5));
    else if (!strcmp(name,"RightWidth"))            RightWidth          = xml_parse_dbl(value, qreal(5));
    else if (!strcmp(name,"SowOver"))               SowOver             = xml_parse_dbl(value, qreal(0.2));
    else if (!strcmp(name,"Gisto_Show"))            Gisto_Show          = xml_parse_bool(value, true);
    else if (!strcmp(name,"GistoYellowExist"))      GistoYellowExist    = xml_parse_bool(value, false);
    else if (!strcmp(name,"GistoYellowProcent"))    GistoYellowProcent  = xml_parse_int(value, 30);
    else if (!strcmp(name,"GistoRedExist"))         GistoRedExist       = xml_parse_bool(value, false);
    else if (!strcmp(name,"GistoRedProcent"))       GistoRedProcent     = xml_parse_int(value, 90);
    else if (!strcmp(name,"GistoSizeAverageIdx"))   GistoSizeAverageIdx = xml_parse_int(value, 1);
    else if (!strcmp(name,"PlugDownInversion"))     PlugDownInversion   = xml_parse_bool(value, false);
    else if (!strcmp(name,"PlugDownForce"))         PlugDownForce       = xml_parse_bool(value, false);
    else if (!strcmp(name,"DepthSensorsExists"))    DepthSensorsExists  = xml_parse_bool(value, false);
    else if (!strcmp(name,"DepthSensorsMaskaOn"))   DepthSensorsMaskaOn = xml_parse_int(value, 0);
    else if (!strcmp(name,"FixChangeStream"))       FixChangeStream     = xml_parse_bool(value, false);

    else if (!strcmp(name,"SectionExists"))         xml_parse_array_int(value,  (int*)SectionExists,    cSectionsMaxCount*cSectionItemCount,    0);

// Add
    else if (!strcmp(name,"SpeedMethod"))         SpeedMethod             = xml_parse_int(value, SpeedNone);
    else if (!strcmp(name,"EncoderConstant"))     SpeedEncoderConstant    = xml_parse_dbl(value, qreal(0));
    else if (!strcmp(name,"EncoderImp2metr"))     SpeedEncoderImp2metr    = xml_parse_dbl(value, qreal(100));
    else if (!strcmp(name,"EncoderCalibrateControl"))     EncoderCalibrateControl       = xml_parse_bool(value, false);
    else if (!strcmp(name,"ProcentAlarmCalibrateEncoder")) ProcentAlarmCalibrateEncoder = xml_parse_int(value, 5);
    else if (!strcmp(name,"Fan_Gidro"))           Fan_Gidro       = xml_parse_bool(value, false);
    else if (!strcmp(name,"Fan_Dvigatel"))        Fan_Dvigatel    = xml_parse_bool(value, false);
    else if (!strcmp(name,"TimeSync"))            TimeSync        = xml_parse_bool(value, false);
    else if (!strcmp(name,"TimeRegion"))          TimeRegion      = xml_parse_int(value, 0);
    else if (!strcmp(name,"PortContr"))           PortContr       = xml_parse_str(value, "mxc2");
    else if (!strcmp(name,"PortSpeed"))           PortSpeed       = xml_parse_str(value, "115200");
    else if (!strcmp(name,"PaswCommon"))          PaswCommon      = xml_parse_str(value, "123");

// Special
    else if (!strcmp(name,"SpeedSowMax"))               SpeedSowMax = xml_parse_int(value, 10);
    else if (!strcmp(name,"FanBaudMin"))                xml_parse_array_int(value,  FanBaudMin, COUNT_OF(FanBaudMin),   1000);
    else if (!strcmp(name,"FanBaudMax"))                xml_parse_array_int(value,  FanBaudMax, COUNT_OF(FanBaudMax),   5000);
    else if (!strcmp(name,"UseSystemWeight"))           UseSystemScale          = xml_parse_bool(value, false);
    else if (!strcmp(name,"ActuatorsBack"))             ActuatorsBack           = xml_parse_int(value, 0);
    else if (!strcmp(name,"PressControl"))              PressControl            = xml_parse_bool(value, false);
    else if (!strcmp(name,"PressureMin"))               PressureMin             = xml_parse_int(value, 3);
    else if (!strcmp(name, "PaswSpecial"))              PaswSpecial             = xml_parse_str(value, "123");
    else if (!strcmp(name,"SowDepthOn"))                SowDepthOn              = xml_parse_int(value, 0);
    else if (!strcmp(name,"SowDepthAgrIndex"))          SowDepthAgrIndex        = xml_parse_int(value, 0);
    else if (!strcmp(name,"FanMaxRpmTv2"))              FanMaxRpmTv2            = xml_parse_int(value, 5000);
    else if (!strcmp(name,"PressMustTv2"))              PressMustTv2            = xml_parse_int(value, 5000);
    else if (!strcmp(name,"PressFromTv2"))              PressFromTv2            = xml_parse_int(value, 2500);
    else if (!strcmp(name,"PressToTv2"))                PressToTv2              = xml_parse_int(value, 6000);
    else if (!strcmp(name,"HisteresisTv2"))             HisteresisTv2           = xml_parse_int(value, 20);
    else if (!strcmp(name,"IntervalForDoubleTv2"))      IntervalForDoubleTv2    = xml_parse_int(value, 50);
    else if (!strcmp(name,"CoefIntervalForPassTv2"))    CoefIntervalForPassTv2  = xml_parse_int(value, 7);
    else if (!strcmp(name,"calcDoubleTv2"))             calcDoubleTv2           = xml_parse_bool(value, false);
    else if (!strcmp(name,"StreemToMsTv2"))             StreemToMsTv2           = xml_parse_bool(value, false);

    else if (!strcmp(name,"SowNormaIndexCultureDiff"))  xml_parse_array_int(value, SowNormaIndexCultureDiff,    COUNT_OF(SowNormaIndexCultureDiff), -1);  // должно быть перед "SowNormaIndexCulture"
    else if (!strcmp(name,"SowNormaBaseKgDiff"))        xml_parse_array_dbl(value, (double*)SowNormaBaseKgDiff, COUNT_OF(SowNormaBaseKgDiff)*cMaxCountCulture, qreal(0));
    else if (!strcmp(name,"SowNormaBaseImpDiff"))       xml_parse_array_int(value, (int*)SowNormaBaseImpDiff,   COUNT_OF(SowNormaBaseImpDiff)*cMaxCountCulture, 0);  // должно быть перед "SowNormaIndexCulture"
    else if (!strcmp(name,"SowNormaAutoKgDiff"))        xml_parse_array_dbl(value, (double*)SowNormaAutoKgDiff, COUNT_OF(SowNormaAutoKgDiff)*cMaxCountCulture, qreal(0));
    else if (!strcmp(name,"SowNormaAutoImpDiff"))       xml_parse_array_int(value, (int*)SowNormaAutoImpDiff,   COUNT_OF(SowNormaAutoImpDiff)*cMaxCountCulture, 0);  // должно быть перед "SowNormaIndexCulture"
    else if (!strcmp(name,"SowNormaIndexCulture"))      xml_parse_array_int(value, SowNormaIndexCulture,        COUNT_OF(SowNormaIndexCulture), 0);
    else if (!strcmp(name,"NameCulture"))               xml_parse_array_str1251(value, (std::string *)nameCulture,  COUNT_OF(nameCulture)*COUNT_OF(nameCulture[0]), "");
    else if (!strcmp(name,"SowNormaBaseKg"))            xml_parse_array_dbl(value, (double*)SowNormaBaseKg,     COUNT_OF(SowNormaBaseKg)*cMaxCountCulture, qreal(0));
    else if (!strcmp(name,"SowNormaBaseImp"))           xml_parse_array_int(value, (int*)SowNormaBaseImp,       COUNT_OF(SowNormaBaseImp)*cMaxCountCulture, 0);  // должно быть перед "SowNormaIndexCulture"
    else if (!strcmp(name,"SowNormaAutoKg"))            xml_parse_array_dbl(value, (double*)SowNormaAutoKg,     COUNT_OF(SowNormaAutoKg)*cMaxCountCulture, qreal(0));
    else if (!strcmp(name,"SowNormaAutoImp"))           xml_parse_array_int(value, (int*)SowNormaAutoImp,       COUNT_OF(SowNormaAutoImp)*cMaxCountCulture, 0);
    else if (!strcmp(name,"SowNormaSet"))               xml_parse_array_dbl(value, SowNormaSet,                 COUNT_OF(SowNormaSet), qreal(0));
    else if (!strcmp(name,"SowNormaPotenc"))            xml_parse_array_int(value, SowNormaPotenc,              COUNT_OF(SowNormaPotenc), 0);
    else if (!strcmp(name,"SowNormaDebug"))             SowNormaDebug   = xml_parse_bool(value, false);

    else if (!strcmp(name,"DifEntry"))                  DifEntry                = xml_parse_bool(value, false);
    else if (!strcmp(name,"DifNormaBase"))              xml_parse_array_dbl(value, DifNormaBase,    COUNT_OF(DifNormaBase), qreal(0));
    else if (!strcmp(name,"DifNormaGpsNot"))            xml_parse_array_dbl(value, DifNormaGpsNot,  COUNT_OF(DifNormaGpsNot), qreal(0));
    else if (!strcmp(name,"ActTransRatio"))             xml_parse_array_int(value, ActTransRatio,   COUNT_OF(ActTransRatio), 10);
    else if (!strcmp(name,"SowNormaTv_Duration"))       SowNormaTv_Duration     = xml_parse_int(value, 1);
    else if (!strcmp(name,"SowNormaTv_Interval"))       SowNormaTv_Interval     = xml_parse_int(value, 2);
    else if (!strcmp(name,"SowNormaTv_ProcentPass"))    SowNormaTv_ProcentPass  = xml_parse_int(value, 25);
    else if (!strcmp(name,"SowNormaTv_ProcentDouble"))  SowNormaTv_ProcentDouble= xml_parse_int(value, 25);
    else if (!strcmp(name,"SowNormaTv_ShowAuto"))       SowNormaTv_ShowAuto     = xml_parse_bool(value, false);
    else if (!strcmp(name,"SowNormaTv_Count2m"))        SowNormaTv_Count2m      = xml_parse_dbl(value, qreal(5));
    else if (!strcmp(name,"SowNormaTv_Average"))        SowNormaTv_Average      = xml_parse_int(value, 1);
    else if (!strcmp(name,"SowNormaTv_ModeSowAlarm"))   SowNormaTv_ModeSowAlarm = xml_parse_bool(value, false);
    else if (!strcmp(name,"SowNormaTv_ProcentAlarm"))   SowNormaTv_ProcentAlarm = xml_parse_int(value, 25);
    else if (!strcmp(name,"SowNormaTv_Culture"))        SowNormaTv_Culture      = xml_parse_str(value, "");

    else if (!strcmp(name,"ScaleCoef0"))                xml_parse_array_dbl(value, ScaleCoef0, COUNT_OF(ScaleCoef0), qreal(1));
    else if (!strcmp(name,"ScaleCoef1"))                xml_parse_array_dbl(value, ScaleCoef1, COUNT_OF(ScaleCoef1), qreal(1));
    else if (!strcmp(name,"ScaleProcent"))              ScaleProcent    = xml_parse_int(value, 10);
    else if (!strcmp(name,"SowNormaMax"))               SowNormaMax     = xml_parse_int(value, 200);
    else if (!strcmp(name,"SowNormaMin15"))             SowNormaMin15   = xml_parse_bool(value, true);

    else if (!strcmp(name,"Potencio0"))     xml_parse_array_ushort(value, Potencio[0], COUNT_OF(Potencio[0]), 0);
    else if (!strcmp(name,"Potencio1"))     xml_parse_array_ushort(value, Potencio[1], COUNT_OF(Potencio[0]), 0);
    else if (!strcmp(name,"Potencio2"))     xml_parse_array_ushort(value, Potencio[2], COUNT_OF(Potencio[0]), 0);
    else if (!strcmp(name,"Imp2metr0"))     xml_parse_array_dbl(value, Imp2metr[0], COUNT_OF(Imp2metr[0]), 0);
    else if (!strcmp(name,"Imp2metr1"))     xml_parse_array_dbl(value, Imp2metr[1], COUNT_OF(Imp2metr[0]), 0);
    else if (!strcmp(name,"Imp2metr2"))     xml_parse_array_dbl(value, Imp2metr[2], COUNT_OF(Imp2metr[0]), 0);

// Counts
    else if (!strcmp(name,"BunkerHide"))    xml_parse_array_bool(value, BunkerHide, COUNT_OF(BunkerHide), false);

// Navigation
    else if (!strcmp(name,"Navigator"))               Navigator       = xml_parse_bool(value, false);
    else if (!strcmp(name,"TrackerSound"))            TrackerSound    = xml_parse_bool(value, false);
    else if (!strcmp(name,"TrackerOversow"))          TrackerOversow  = xml_parse_dbl(value, qreal(0.4));
    else if (!strcmp(name,"TrackerUndersow"))         TrackerUndersow = xml_parse_dbl(value, qreal(0.0));
    else if (!strcmp(name,"ShowAlarm"))               ShowAlarm       = xml_parse_bool(value, true);
    else if (!strcmp(name,"LengGPS"))                 LengGPS         = xml_parse_dbl(value, qreal(3.0));
    else if (!strcmp(name,"LengBunker"))              LengBunker      = xml_parse_dbl(value, qreal(6.0));
    else if (!strcmp(name,"LengCultivator"))          LengCultivator  = xml_parse_dbl(value, qreal(4.0));
    else if (!strcmp(name,"SchemeAgregate"))          SchemeAgregate  = xml_parse_int(value, Options::Scheme_AgrCultBunk);
    else if (!strcmp(name,"TypeWork"))                TypeWork        = xml_parse_int(value, TypeSow);
    else if (!strcmp(name,"PlugNavigManual"))         PlugNavigManual = xml_parse_bool(value, false);
    else if (!strcmp(name,"GpsPort"))                 GpsPort         = xml_parse_str(value, "mxc1");
    else if (!strcmp(name,"GpsSpeed"))                GpsSpeed        = xml_parse_str(value, "115200");

// Internet
    else if (!strcmp(name,"sendRealTime"))          sendRealTime        = xml_parse_int(value, 0);
    else if (!strcmp(name,"SendJsonFormat"))        SendJsonFormat      = xml_parse_bool(value, false);
    else if (!strcmp(name,"StoreRealTime"))         StoreRealTime       = xml_parse_int(value, 0);
    else if (!strcmp(name,"indexProviderSim0"))     indexProviderSim0   = xml_parse_int(value, 0);
    else if (!strcmp(name,"indexProviderSim1"))     indexProviderSim1   = xml_parse_int(value, 0);
    else if (!strcmp(name,"tcpIpServer"))           tcpIpServer         = xml_parse_str(value, "");
    else if (!strcmp(name,"owner")) {                owner               = std_cp1251_to_utf8(value ? value : "");
//             printf("owner.cp1251=%s   owner.utf8=%s\n", value, owner.c_str());
//             FILE* fo = fopen("/home/user/owner.txt", "wt");
//             if (fo) {
//                 fprintf(fo, "owner.cp1251=%s   owner.utf8=%s\n", value, owner.c_str());
//                 fclose(fo);
//             } else {
//                 printf("Error write file owner.txt\n");
//             }
    }
    else if (!strcmp(name,"password"))              password            = xml_parse_str(value, "");
    else if (!strcmp(name,"agrName"))               agrName             = std_cp1251_to_utf8(value ? value : "");
    else if (!strcmp(name,"agrNameTv"))             agrNameTv           = std_cp1251_to_utf8(value ? value : "");
    else if (!strcmp(name,"PortModem"))             PortModem           = xml_parse_str(value, "mxc4");
    else if (!strcmp(name,"PortModemSpeed"))        PortModemSpeed      = xml_parse_str(value, "115200");
    else if (!strcmp(name,"GprsReduction"))         GprsReduction       = xml_parse_int(value, 0);
    else if (!strcmp(name,"PauseAfterTransmit"))    PauseAfterTransmit  = xml_parse_int(value, 0);

// Security
    else if (!strcmp(name,"volumeProcent"))           volumeProcent       = xml_parse_int(value, 100);       // процент громкости
    else if (!strcmp(name,"brightnessProcent"))       brightnessProcent   = xml_parse_int(value, 100);       // процент яркости
    else if (!strcmp(name,"sowNormaAutoAverage"))     sowNormaAutoAverage = xml_parse_bool(value, false);    // усреднять норму высева автоматическую (при отображении)
    else if (!strcmp(name,"sowNormaMedianaIdx"))      sowNormaMedianaIdx  = xml_parse_int(value, 0);
    else if (!strcmp(name,"sowNormaAverageJson"))     sowNormaAverageJson = xml_parse_bool(value, false);    // усреднять норму высева автоматическую (при передаче в Json-формате)
    else if (!strcmp(name,"sowNormaJsonSendSec"))     sowNormaJsonSendSec     = xml_parse_int(value, 1);
    else if (!strcmp(name,"SowNormaMedianaActuator")) SowNormaMedianaActuator = xml_parse_int(value, 1);
    else if (!strcmp(name,"SowNormaAverageMediana"))  SowNormaAverageMediana  = xml_parse_bool(value, false);
    else if (!strcmp(name,"traficWrite"))             traficWrite         = xml_parse_bool(value, false);   // писать входной трафик
    else if (!strcmp(name,"logErrWrite"))             logErrWrite         = xml_parse_bool(value, false);   // писать лог ошибок
    else if (!strcmp(name,"errorIgnore"))             errorIgnore         = xml_parse_bool(value, false);    // игнорировать ошибки датчиков
    else if (!strcmp(name,"valImpCorr0"))             valImpCorr[0]       = xml_parse_int(value, 0);              // коррекция нормы высева +5%,-5%,0%
    else if (!strcmp(name,"valImpCorr1"))             valImpCorr[1]       = xml_parse_int(value, 0);              // коррекция нормы высева +5%,-5%,0%
    else if (!strcmp(name,"valImpCorr2"))             valImpCorr[2]       = xml_parse_int(value, 0);              // коррекция нормы высева +5%,-5%,0%
    else if (!strcmp(name,"sowNormaMode"))            sowNormaMode        = xml_parse_int(value, 0);
    else if (!strcmp(name,"SowNormaProcent"))         SowNormaProcent     = xml_parse_int(value, 25);
    else if (!strcmp(name,"LargeButtons"))            LargeButtons        = xml_parse_bool(value, true);    // кнопки сенсорные
    else if (!strcmp(name,"StaticSensorsOn"))         StaticSensorsOn     = xml_parse_bool(value, false);
    else if (!strcmp(name,"ModeService"))             ModeService         = xml_parse_bool(value, false);
    else if (!strcmp(name,"BrightnessScreen"))        BrightnessScreen    = xml_parse_int(value, 100);
    else if (!strcmp(name,"BrightnessButtons"))       BrightnessButtons   = xml_parse_int(value, 100);

    else if (!strcmp(name,"indexBrightnessScreen"))   indexBrightnessScreen    = xml_parse_int(value, 0);
    else if (!strcmp(name,"indexBrightnessButtons"))  indexBrightnessButtons   = xml_parse_int(value, 0);
    else if (!strcmp(name,"procentScreen100"))        procentScreen100    = xml_parse_int(value, 100);
    else if (!strcmp(name,"procentScreen80"))         procentScreen80     = xml_parse_int(value, 80);
    else if (!strcmp(name,"procentScreen60"))         procentScreen60     = xml_parse_int(value, 60);
    else if (!strcmp(name,"procentScreen40"))         procentScreen40     = xml_parse_int(value, 40);
    else if (!strcmp(name,"procentScreen20"))         procentScreen20     = xml_parse_int(value, 20);
    else if (!strcmp(name,"procentScreen10"))         procentScreen10     = xml_parse_int(value, 10);
    else if (!strcmp(name,"procentButtons100"))       procentButtons100   = xml_parse_int(value, 100);
    else if (!strcmp(name,"procentButtons80"))        procentButtons80    = xml_parse_int(value, 80);
    else if (!strcmp(name,"procentButtons60"))        procentButtons60    = xml_parse_int(value, 60);
    else if (!strcmp(name,"procentButtons40"))        procentButtons40    = xml_parse_int(value, 40);
    else if (!strcmp(name,"procentButtons20"))        procentButtons20    = xml_parse_int(value, 20);
    else if (!strcmp(name,"procentButtons10"))        procentButtons10    = xml_parse_int(value, 10);

// Жидкие удобрения
    else if (!strcmp(name,"VolumeCount"))       VolumeCount    = xml_parse_int(value, 1);
    else if (!strcmp(name,"IsSensorPress"))     xml_parse_array_bool(   value, IsSensorPress,   COUNT_OF(IsSensorPress), false);
    else if (!strcmp(name,"IsFlowmeter"))       xml_parse_array_bool(   value, IsFlowmeter,     COUNT_OF(IsFlowmeter), false);
    else if (!strcmp(name,"SensorPressMax"))    xml_parse_array_dbl(    value, SensorPressMax,  COUNT_OF(SensorPressMax), qreal(0));
    else if (!strcmp(name,"FlowmeterMl2imp"))   xml_parse_array_dbl(    value, FlowmeterMl2imp, COUNT_OF(FlowmeterMl2imp), qreal(0));
    else if (!strcmp(name,"LiquidSound"))       LiquidSound     = xml_parse_bool(value, true);
    else if (!strcmp(name,"LiquidShowEvent"))   LiquidShowEvent = xml_parse_bool(value, true);
    else if (!strcmp(name,"nameLiquid"))        xml_parse_array_str1251(value, nameLiquid,      COUNT_OF(nameLiquid), "");
    else if (!strcmp(name,"LiquidNameIndex"))   xml_parse_array_int(    value, LiquidNameIndex, COUNT_OF(LiquidNameIndex), -1);
}

void Options::xmlObject_parse_SemensOff(DXMLObject *ob)
{
    static  char tower[] = "Tower0";
    for (int iTower = 0; iTower < COUNT_OF(SemensOff); iTower++) {
        tower[5] = '0' + iTower;
        DXMLParam* value = ob->Param(tower);
        if (value) {
            char ss[COUNT_OF(SemensOff[0])*2 + 2];
            snprintf(ss, sizeof(ss), "%s", value->Value());
            char *aa[COUNT_OF(SemensOff[0])];
            int cnt = util_split(ss, ' ', aa, COUNT_OF(aa));
            for (int iSem = 0; iSem < COUNT_OF(SemensOff[0]); iSem++) {
                SemensOff[iTower][iSem] = iSem < cnt ? (aa[iSem][0]=='1' ? true : false) : false;
            }
        } else {
            for (int iSem = 0; iSem < COUNT_OF(SemensOff[0]); iSem++) {
                SemensOff[iTower][iSem] = false;
            }
        }
    }
}
void Options::xmlObject_parse_SemensCropsOff(DXMLObject *ob)
{
    static  char mod[] = "Mod0";
    for (int iMod = 0; iMod < COUNT_OF(SemensCropsOff); iMod++) {
        mod[3] = '0' + iMod;
        DXMLParam* value = ob->Param(mod);
        if (value) {
            char ss[COUNT_OF(SemensCropsOff[0])*2 + 2];
            snprintf(ss, sizeof(ss), "%s", value->Value());
            char *aa[COUNT_OF(SemensCropsOff[0])];
            int cnt = util_split(ss, ' ', aa, COUNT_OF(aa));
            for (int iSem = 0; iSem < COUNT_OF(SemensCropsOff[0]); iSem++) {
                SemensCropsOff[iMod][iSem] = iSem < cnt ? (aa[iSem][0]=='1' ? true : false) : false;
            }
        } else {
            for (int iSem = 0; iSem < COUNT_OF(SemensCropsOff[0]); iSem++) {
                SemensCropsOff[iMod][iSem] = false;
            }
        }
    }
}
void Options::xmlObject_parse_SemensManureOff(DXMLObject *ob)
{
    static  char mod[] = "Mod0";
    for (int iMod = 0; iMod < COUNT_OF(SemensManureOff); iMod++) {
        mod[3] = '0' + iMod;
        DXMLParam* value = ob->Param(mod);
        if (value) {
            char ss[COUNT_OF(SemensManureOff[0])*2 + 2];
            snprintf(ss, sizeof(ss), "%s", value->Value());
            char *aa[COUNT_OF(SemensManureOff[0])];
            int cnt = util_split(ss, ' ', aa, COUNT_OF(aa));
            for (int iSem = 0; iSem < COUNT_OF(SemensManureOff[0]); iSem++) {
                SemensManureOff[iMod][iSem] = iSem < cnt ? (aa[iSem][0]=='1' ? true : false) : false;
            }
        } else {
            for (int iSem = 0; iSem < COUNT_OF(SemensManureOff[0]); iSem++) {
                SemensManureOff[iMod][iSem] = false;
            }
        }
    }
}

int Options::xml_Parse(char* ps, int size)
{
    int ok = 0;
    DXMLParser xml;
    xml.Parse(ps, size);
    DXMLObject* ObjXml = xml.Object("Options");
    if (ObjXml){
        for (DXMLElement* o = ObjXml->First(); o != 0; o = o->Next()) {
            if (o->type() != DXML_Type_Object)
                continue;
            DXMLObject *ob = (DXMLObject*)o;
            if (ob->Name()) {
                DXMLParam* value = ob->Param("Value");
                if (value) {
                    xmlObject_parse(ob->Name(), value->Value());
                } else if (!strcmp(ob->Name(),"SemensOff")) {
                    xmlObject_parse_SemensOff(ob);
                } else if (!strcmp(ob->Name(),"SemensCropsOff")) {
                    xmlObject_parse_SemensCropsOff(ob);
                } else if (!strcmp(ob->Name(),"SemensManureOff")) {
                    xmlObject_parse_SemensManureOff(ob);
                }
            }
        }
        ok = 1;
    } else {
       printf("Can't find object Options\n");
    }
    return ok;
}

int Options::xml_read()
{
    int ok = 0;
    FILE* fp = fopen(nameFile.c_str(), "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        printf("sizeFile=%d\n", size);
        fseek(fp, 0, SEEK_SET);
        char* ps = new char[size+1];
        int sz = fread(ps, size, 1, fp);
        if (sz == 1) {
            ok = xml_Parse(ps, size);
        } else {
            printf("Error read file %s\n", nameFile.c_str());
        }
        delete[] ps;
        fclose(fp);
    } else {
        printf("Error open file %s\n", nameFile.c_str());
    }
    return ok;
}

void Options::Read()
{
    int ok = xml_read();
    if (!ok) {
        Write();
    }
}

//======================================================================
void    param_add(DXMLObject* root, const char* name, const char* value)
{
    DXMLObject* obj = new DXMLObject(name);
    obj->add(new DXMLParam("Value", value));
    root->add(obj);
}
void    param_add(DXMLObject* root, const char* name, int value)
{
    std::string sval = int_toString(value, 10);
    param_add(root, name, sval.c_str());
}
static  void    param_add(DXMLObject* root, const char* name, uint value)
{
    std::string sval = int_toString(value, 10);
    param_add(root, name, sval.c_str());
}
static  void    param_add(DXMLObject* root, const char* name, bool value)
{
    param_add(root, name, value ? "1" : "0");
}
static  void    param_add(DXMLObject* root, const char* name, double value, int precision)
{
    char ss[16];
    snprintf(ss, sizeof(ss), "%.*f", precision, value);
    param_add(root, name, ss);
}
static  void    param_add_array_int(DXMLObject* root, const char* name, int* aa, int count)
{
    std::string ss = "";
    for (int i = 0; i < count; i++) {
        if (!ss.empty()) ss += " ";
        ss += int_toString(aa[i], 10);
    }
    return param_add(root, name, ss.c_str());
}
static  void    param_add_array_bool(DXMLObject* root, const char* name, bool* aa, int count)
{
    std::string ss = "";
    for (int i = 0; i < count; i++) {
        if (!ss.empty()) ss += " ";
        ss += aa[i] ? "1" : "0";
    }
    return param_add(root, name, ss.c_str());
}
static  void    param_add_array_ushort(DXMLObject* root, const char* name, ushort* aa, int count)
{
    std::string ss = "";
    for (int i = 0; i < count; i++) {
        if (!ss.empty()) ss += " ";
        ss += int_toString(aa[i], 10);
    }
    return param_add(root, name, ss.c_str());
}
static  void    param_add_array_str1251(DXMLObject* root, const char* name, const std::string* aa, int count)
{
    std::string ss = "";
    for (int i = 0; i < count; i++) {
        ss += std_utf8_to_cp1251(aa[i].c_str());
        ss += "_";
    }
    return param_add(root, name, ss.c_str());
}
static  void    param_add_array_dbl(DXMLObject* root, const char* name, double* aa, int count, int precision)
{
    std::string str = "";
    for (int i = 0; i < count; i++) {
        if (!str.empty()) str += " ";
        char    ss[16];
        snprintf(ss, sizeof(ss), "%.*f", precision, aa[i]);
        str += ss;
    }
    return param_add(root, name, str.c_str());
}
void    Options::param_add_SemensOff(DXMLObject* root)
{
    static  char    tower[] = "Tower0";
    DXMLObject* obj = new DXMLObject("SemensOff");
    for (int iTower = 0; iTower < COUNT_OF(SemensOff); iTower++) {
        char ar[2*COUNT_OF(SemensOff[0]) + 2];
        memset(ar, 0, sizeof(ar));
        for (int iSem = 0; iSem < COUNT_OF(SemensOff[0]); iSem++) {
            ar[2*iSem] = SemensOff[iTower][iSem] ? '1' : '0';
            if (iSem+1 < COUNT_OF(SemensOff[0])) {
                ar[2*iSem + 1] = ' ';
            }
        }
        ar[2*COUNT_OF(SemensOff[0])] = 0;
        tower[5] = '0' + iTower;
        DXMLParam* param = new DXMLParam(tower, ar);    // Tower0="0 0 0 ..."
        obj->add(param);    // <SemensOff Tower0="0 0 0 ..." Tower1="0 0 0 ..." .../>
    }
    root->add(obj);
}
void    Options::param_add_SemensCropsOff(DXMLObject* root)
{
    static  char    mod[] = "Mod0";
    DXMLObject* obj = new DXMLObject("SemensCropsOff");
    for (int iMod = 0; iMod < COUNT_OF(SemensCropsOff); iMod++) {
        char ar[2*COUNT_OF(SemensCropsOff[0]) + 2];
        memset(ar, 0, sizeof(ar));
        for (int iSem = 0; iSem < COUNT_OF(SemensCropsOff[0]); iSem++) {
            ar[2*iSem] = SemensCropsOff[iMod][iSem] ? '1' : '0';
            if (iSem+1 < COUNT_OF(SemensCropsOff[0])) {
                ar[2*iSem + 1] = ' ';
            }
        }
        ar[2*COUNT_OF(SemensCropsOff[0])] = 0;
        mod[3] = '0' + iMod;
        DXMLParam* param = new DXMLParam(mod, ar);    // Mod0="0 0 0 ..."
        obj->add(param);    // <SemensCropsOff Mod0="0 0 0 ..." Mod1="0 0 0 ..." .../>
    }
    root->add(obj);
}
void    Options::param_add_SemensManureOff(DXMLObject* root)
{
    static  char    mod[] = "Mod0";
    DXMLObject* obj = new DXMLObject("SemensManureOff");
    for (int iMod = 0; iMod < COUNT_OF(SemensManureOff); iMod++) {
        char ar[2*COUNT_OF(SemensManureOff[0]) + 2];
        memset(ar, 0, sizeof(ar));
        for (int iSem = 0; iSem < COUNT_OF(SemensManureOff[0]); iSem++) {
            ar[2*iSem] = SemensManureOff[iMod][iSem] ? '1' : '0';
            if (iSem+1 < COUNT_OF(SemensCropsOff[0])) {
                ar[2*iSem + 1] = ' ';
            }
        }
        ar[2*COUNT_OF(SemensManureOff[0])] = 0;
        mod[3] = '0' + iMod;
        DXMLParam* param = new DXMLParam(mod, ar);    // Mod0="0 0 0 ..."
        obj->add(param);    // <SemensManureOff Mod0="0 0 0 ..." Mod1="0 0 0 ..." .../>
    }
    root->add(obj);
}

DXMLObject* Options::createXmlRoot()
{
    printf("createXmlRoot\n");
    DXMLObject* root = new DXMLObject("Options");
    param_add(root, "AgroMode", AgroMode);
    param_add(root, "AgroDemo", AgroDemo);
    // Common
    param_add(root, "BunkersCount",     BunkersCount);
    param_add(root, "SectionsCount",    SectionsCount);
    param_add(root, "TowersCount",      TowersCount);
    param_add_array_int(root, "SemensCount",      SemensCount,    cTowersMaxCount);
    param_add_array_int(root, "CropsType",        CropsType,      cTowersMaxCount);
    param_add(root, "FansCount",        FansCount);
    param_add_array_int(root, "FanImpulsCount",   FanImpulsCount, cFansMaxCount);
    param_add(root, "ValsCount",        ValsCount);

    param_add(root, "ModCropsCount",       ModCropsCount);
    param_add(root, "ModManureCount",      ModManureCount);

    param_add_array_int(root, "SensorCropsCount",    SensorCropsCount,   COUNT_OF(SensorCropsCount));
    param_add_array_int(root, "SensorManureCount",   SensorManureCount,  COUNT_OF(SensorManureCount));
    param_add(root, "ControlLevelCrops",   ControlLevelCrops);
    param_add(root, "ControlLevelManure",  ControlLevelManure);

    param_add(root, "SensorCountTv2",      SensorCountTv2);
    param_add(root, "isEncoderTv2",        isEncoderTv2);
    param_add(root, "isFanTv2",            isFan1Tv2);
    param_add(root, "isFan2Tv2",           isFan2Tv2);
    param_add(root, "imp2cycleFanTv2",     imp2cycleFanTv2);
    param_add(root, "imp2cycleFan2Tv2",    imp2cycleFan2Tv2);
    param_add(root, "isPressTv2",          isPressTv2);
    param_add(root, "isPress2Tv2",         isPress2Tv2);
    param_add(root, "isActuatorTv2",       isActuatorTv2);

    param_add(root, "isSowWidth",          isSowWidth);
    param_add(root, "SowWidth",            SowWidth,    1);
    param_add(root, "LeftWidth",           LeftWidth,   1);
    param_add(root, "RightWidth",          RightWidth,  1);
    param_add(root, "SowOver",             SowOver,     1);

    param_add(root, "Gisto_Show",          Gisto_Show);
    param_add(root, "GistoYellowExist",    GistoYellowExist);
    param_add(root, "GistoYellowProcent",  GistoYellowProcent);
    param_add(root, "GistoRedExist",       GistoRedExist);
    param_add(root, "GistoRedProcent",     GistoRedProcent);
    param_add(root, "GistoSizeAverageIdx", GistoSizeAverageIdx);
    param_add(root, "PlugDownInversion",   PlugDownInversion);
    param_add(root, "PlugDownForce",       PlugDownForce);
    param_add(root, "DepthSensorsExists",  DepthSensorsExists);
    param_add(root, "DepthSensorsMaskaOn", DepthSensorsMaskaOn);
    param_add(root, "FixChangeStream",     FixChangeStream);
    param_add_array_int(root, "SectionExists", (int*)SectionExists, cSectionsMaxCount*cSectionItemCount);

    // Add
    param_add(root, "SpeedMethod",                 SpeedMethod);
    param_add(root, "EncoderConstant",             SpeedEncoderConstant,    1);
    param_add(root, "EncoderImp2metr",             SpeedEncoderImp2metr,    1);
    param_add(root, "EncoderCalibrateControl",     EncoderCalibrateControl);
    param_add(root, "ProcentAlarmCalibrateEncoder",ProcentAlarmCalibrateEncoder);

    param_add(root, "Fan_Gidro",    Fan_Gidro);
    param_add(root, "Fan_Dvigatel", Fan_Dvigatel);
    param_add(root, "TimeSync",     TimeSync);
    param_add(root, "TimeRegion",   TimeRegion);
    param_add(root, "PortContr",    PortContr.c_str());
    param_add(root, "PortSpeed",    PortSpeed.c_str());
    param_add(root, "PaswCommon",   PaswCommon.c_str());

    // Special
    param_add(root, "SpeedSowMax", SpeedSowMax);
    param_add_array_int(root, "FanBaudMin", FanBaudMin, cFansMaxCount);
    param_add_array_int(root, "FanBaudMax", FanBaudMax, cFansMaxCount);
    param_add(root, "UseSystemWeight", UseSystemScale);
    param_add(root, "ActuatorsBack",   ActuatorsBack);
    param_add(root, "PressControl",    PressControl);
    param_add(root, "PressureMin",     PressureMin);
    param_add(root, "PaswSpecial",     PaswSpecial.c_str());

    param_add(root, "FanMaxRpmTv2",    FanMaxRpmTv2);
    param_add(root, "PressMustTv2",    PressMustTv2);
    param_add(root, "PressFromTv2",    PressFromTv2);
    param_add(root, "PressToTv2",      PressToTv2);
    param_add(root, "HisteresisTv2",   HisteresisTv2);
    param_add(root, "IntervalForDoubleTv2",    IntervalForDoubleTv2);
    param_add(root, "CoefIntervalForPassTv2",  CoefIntervalForPassTv2);
    param_add(root, "calcDoubleTv2",           calcDoubleTv2);
    param_add(root, "StreemToMsTv2",           StreemToMsTv2);

    param_add(root, "SowDepthOn",              SowDepthOn);
    param_add(root, "SowDepthAgrIndex",        SowDepthAgrIndex);

    param_add_array_int(root, "SowNormaIndexCulture", SowNormaIndexCulture, COUNT_OF(SowNormaIndexCulture));
    param_add_array_dbl(root, "SowNormaBaseKg", (double*)SowNormaBaseKg,    COUNT_OF(SowNormaBaseKg)*COUNT_OF(SowNormaBaseKg[0]), 2);
    param_add_array_int(root, "SowNormaBaseImp", (int*)SowNormaBaseImp,     COUNT_OF(SowNormaBaseImp)*COUNT_OF(SowNormaBaseImp[0]));
    param_add_array_dbl(root, "SowNormaAutoKg", (double*)SowNormaAutoKg,    COUNT_OF(SowNormaAutoKg)*COUNT_OF(SowNormaAutoKg[0]), 2);
    param_add_array_int(root, "SowNormaAutoImp", (int*)SowNormaAutoImp,     COUNT_OF(SowNormaAutoImp)*COUNT_OF(SowNormaAutoImp[0]));

    param_add_array_int(root, "SowNormaIndexCultureDiff", SowNormaIndexCultureDiff, COUNT_OF(SowNormaIndexCultureDiff));
    param_add_array_dbl(root, "SowNormaBaseKgDiff", (double*)SowNormaBaseKgDiff,    COUNT_OF(SowNormaBaseKgDiff)*cMaxCountCulture, 2);
    param_add_array_int(root, "SowNormaBaseImpDiff", (int*)SowNormaBaseImpDiff,     COUNT_OF(SowNormaBaseImpDiff)*cMaxCountCulture);
    param_add_array_dbl(root, "SowNormaAutoKgDiff", (double*)SowNormaAutoKgDiff,    COUNT_OF(SowNormaAutoKgDiff)*cMaxCountCulture, 2);
    param_add_array_int(root, "SowNormaAutoImpDiff", (int*)SowNormaAutoImpDiff,     COUNT_OF(SowNormaAutoImpDiff)*cMaxCountCulture);

    param_add_array_dbl(root, "SowNormaSet",    SowNormaSet,     COUNT_OF(SowNormaSet),     1);
    param_add_array_int(root, "SowNormaPotenc", SowNormaPotenc,  COUNT_OF(SowNormaPotenc));

    param_add_array_str1251(root, "NameCulture", (const std::string*)nameCulture,  COUNT_OF(nameCulture)*COUNT_OF(nameCulture[0]));
    param_add(root, "SowNormaDebug",            SowNormaDebug);

    param_add(root, "DifEntry",                 DifEntry);
    param_add_array_dbl(root, "DifNormaBase",    DifNormaBase,   COUNT_OF(DifNormaBase),    1);
    param_add_array_dbl(root, "DifNormaGpsNot",  DifNormaGpsNot, COUNT_OF(DifNormaGpsNot),  1);
    param_add_array_int(root, "ActTransRatio",   ActTransRatio,  COUNT_OF(ActTransRatio));

    param_add(root, "SowNormaTv_Duration",      SowNormaTv_Duration);
    param_add(root, "SowNormaTv_Interval",      SowNormaTv_Interval);
    param_add(root, "SowNormaTv_ShowAuto",      SowNormaTv_ShowAuto);
    param_add(root, "SowNormaTv_ProcentPass",   SowNormaTv_ProcentPass);
    param_add(root, "SowNormaTv_ProcentDouble", SowNormaTv_ProcentDouble);
    param_add(root, "SowNormaTv_Count2m",       SowNormaTv_Count2m, 2);
    param_add(root, "SowNormaTv_Average",       SowNormaTv_Average);
    param_add(root, "SowNormaTv_ModeSowAlarm",  SowNormaTv_ModeSowAlarm);
    param_add(root, "SowNormaTv_ProcentAlarm",  SowNormaTv_ProcentAlarm);
    param_add(root, "SowNormaTv_Culture",       SowNormaTv_Culture.c_str());

    param_add_array_dbl(root, "ScaleCoef0",  ScaleCoef0, COUNT_OF(ScaleCoef0), 2);
    param_add_array_dbl(root, "ScaleCoef1",  ScaleCoef1, COUNT_OF(ScaleCoef1), 2);
    param_add(root, "ScaleProcent",             ScaleProcent);
    param_add(root, "SowNormaMax",              SowNormaMax);
    param_add(root, "SowNormaMin15",            SowNormaMin15);

    param_add_array_ushort(root, "Potencio0", Potencio[0],   COUNT_OF(Potencio[0]));
    param_add_array_ushort(root, "Potencio1", Potencio[1],   COUNT_OF(Potencio[1]));
    param_add_array_ushort(root, "Potencio2", Potencio[2],   COUNT_OF(Potencio[2]));

    param_add_array_dbl(root, "Imp2metr0", Imp2metr[0],COUNT_OF(Imp2metr[0]), 1);
    param_add_array_dbl(root, "Imp2metr1", Imp2metr[1],COUNT_OF(Imp2metr[1]), 1);
    param_add_array_dbl(root, "Imp2metr2", Imp2metr[2],COUNT_OF(Imp2metr[2]), 1);

    // Counts
    param_add_array_bool(root, "BunkerHide", BunkerHide, COUNT_OF(BunkerHide));

    // Navigation
    param_add(root, "Navigator",       Navigator);
    param_add(root, "TrackerSound",    TrackerSound);
    param_add(root, "TrackerOversow",  TrackerOversow,  1);
    param_add(root, "TrackerUndersow", TrackerUndersow, 1);
    param_add(root, "ShowAlarm",       ShowAlarm);
    param_add(root, "LengGPS",         LengGPS,         1);
    param_add(root, "LengBunker",      LengBunker,      1);
    param_add(root, "LengCultivator",  LengCultivator,  1);
    param_add(root, "SchemeAgregate",  SchemeAgregate);
    param_add(root, "TypeWork",        TypeWork);
    param_add(root, "PlugNavigManual", PlugNavigManual);

    param_add(root, "GpsPort",         GpsPort.c_str());
    param_add(root, "GpsSpeed",        GpsSpeed.c_str());

    // Internet
    param_add(root, "sendRealTime",        sendRealTime);
    param_add(root, "SendJsonFormat",      SendJsonFormat);
    param_add(root, "StoreRealTime",       StoreRealTime);
    param_add(root, "indexProviderSim0",   indexProviderSim0);
    param_add(root, "indexProviderSim1",   indexProviderSim1);
    param_add(root, "tcpIpServer",         tcpIpServer.c_str());
//    std::string owner1251 = std_utf8_to_cp1251(owner.c_str());
//    FILE* fo = fopen("/home/user/owner1251.txt", "wt");
//    if (fo) {
//        fprintf(fo, "owner.utf8=%s  1251=%s\n", owner.c_str(), owner1251.c_str());
//        fclose(fo);
//    }
    param_add(root, "owner",               std_utf8_to_cp1251(owner.c_str()).c_str());
    param_add(root, "password",            password.c_str());
    param_add(root, "agrName",             std_utf8_to_cp1251(agrName.c_str()).c_str());
    param_add(root, "agrNameTv",           std_utf8_to_cp1251(agrNameTv.c_str()).c_str());
    param_add(root, "PortModem",           PortModem.c_str());
    param_add(root, "PortModemSpeed",      PortModemSpeed.c_str());
    param_add(root, "GprsReduction",       GprsReduction);
    param_add(root, "PauseAfterTransmit",  PauseAfterTransmit); ;

    // Security
    param_add(root, "volumeProcent",   volumeProcent);
    param_add(root, "brightnessProcent",   brightnessProcent);
    param_add(root, "sowNormaAutoAverage", sowNormaAutoAverage);
    param_add(root, "sowNormaMedianaIdx",  sowNormaMedianaIdx);
    param_add(root, "sowNormaAverageJson", sowNormaAverageJson);
    param_add(root, "sowNormaJsonSendSec", sowNormaJsonSendSec);
    param_add(root, "SowNormaMedianaActuator", SowNormaMedianaActuator);
    param_add(root, "SowNormaAverageMediana",  SowNormaAverageMediana);
    param_add(root, "traficWrite",     traficWrite);
    param_add(root, "logErrWrite",     logErrWrite);
    param_add(root, "errorIgnore",     errorIgnore);
    param_add(root, "valImpCorr0",     valImpCorr[0]);
    param_add(root, "valImpCorr1",     valImpCorr[1]);
    param_add(root, "valImpCorr2",     valImpCorr[2]);
    param_add(root, "sowNormaMode",    sowNormaMode);
    param_add(root, "SowNormaProcent", SowNormaProcent);
    param_add(root, "LargeButtons",    LargeButtons);
    param_add(root, "StaticSensorsOn", StaticSensorsOn);
    param_add(root, "ModeService",     ModeService);
    param_add(root, "BrightnessScreen",    BrightnessScreen);
    param_add(root, "BrightnessButtons",   BrightnessButtons);

    param_add(root, "indexBrightnessScreen",   indexBrightnessScreen);
    param_add(root, "indexBrightnessButtons",  indexBrightnessButtons);
    param_add(root, "procentScreen100",    procentScreen100);
    param_add(root, "procentScreen80",     procentScreen80);
    param_add(root, "procentScreen60",     procentScreen60);
    param_add(root, "procentScreen40",     procentScreen40);
    param_add(root, "procentScreen20",     procentScreen20);
    param_add(root, "procentScreen10",     procentScreen10);
    param_add(root, "procentButtons100",   procentButtons100);
    param_add(root, "procentButtons80",    procentButtons80);
    param_add(root, "procentButtons60",    procentButtons60);
    param_add(root, "procentButtons40",    procentButtons40);
    param_add(root, "procentButtons20",    procentButtons20);
    param_add(root, "procentButtons10",    procentButtons10);

    // SemensOff
    param_add_SemensOff(root);
    param_add_SemensCropsOff(root);
    param_add_SemensManureOff(root);

// Жидкие удобрения
    param_add(root, "VolumeCount",     VolumeCount);
    param_add_array_bool(root,   "IsSensorPress",    IsSensorPress,      COUNT_OF(IsSensorPress));
    param_add_array_bool(root,   "IsFlowmeter",      IsFlowmeter,        COUNT_OF(IsFlowmeter));
    param_add_array_dbl( root,   "SensorPressMax",   SensorPressMax,     COUNT_OF(SensorPressMax),   1);
    param_add_array_dbl( root,   "FlowmeterMl2imp",  FlowmeterMl2imp,    COUNT_OF(FlowmeterMl2imp),  1);
    param_add(root, "LiquidSound",     LiquidSound);
    param_add(root, "LiquidShowEvent", LiquidShowEvent);
    param_add_array_str1251(root, "nameLiquid",      nameLiquid,         COUNT_OF(nameLiquid));
    param_add_array_int(root, "LiquidNameIndex", LiquidNameIndex,    COUNT_OF(LiquidNameIndex));

    return root;
}

// storeIfCmp -настройки сохранить в файле, если изменились
bool Options::Write(bool storeIfCmp)
{
    FILE* fp;
    bool writed = false;
    DXMLObject* root = createXmlRoot();
    DXMLParser xml;
    xml.add(root);
    char* ps = xml.Gen();
//    char ch = ps[256];
//    ps[256] = 0;
//    printf("xml:\n%s\n", ps);
//    ps[256] = ch;
    int sizeGen = strlen(ps);

    if (storeIfCmp) {               // настройки сохранить в файле, если изменились
        printf("writeIfCmp\n");
        bool equal = false;

        fp = fopen(nameFile.c_str(), "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            char* pd = new char[size+1];    pd[size] = 0;
            fread(pd, size, 1, fp);
            fclose(fp);
            if (size == sizeGen) {
                equal = !strncmp(pd, ps, size);
                //equal = memcmp(pd, ps, size);
            }
            delete[] pd;
        }

        printf("options %s equal\n", equal ? "" : "no");
        writed = !equal ? true : false;
    } else {
        writed = true;
    }
    if (writed) {
        fp = fopen(nameFile.c_str(), "wb");
        if (fp) {
            fwrite(ps, strlen(ps), 1, fp);
            fclose(fp);
        }
        std::string nameFileForSend = options.NameFileForSend();
        fp = fopen(nameFileForSend.c_str(), "wb");
        if (fp) {
            fwrite(ps, strlen(ps), 1, fp);
            fclose(fp);
        }
        sync();
    }
    return writed;
}

Options     options;
