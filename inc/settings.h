#ifndef __SETTINGS_H__
#define __SETTINGS_H__
#include <stdint.h>
#include <stdbool.h>

// constants +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//const int   cDebug = 0;

#define cBunkersMaxCount 			3        // Максимальное количество бункеров, режим  "Посевной комплекс"
#define cTowersMaxCount				16       // Максимальное количество башень  (до 6.2.2016 было 12)
#define cSemensMaxCount				16       // Максимальное количество датчиков зерна

#define cSectionsMaxCount			6        // Максимальное количество секций механической сеялки
#define cSectionItemCount			4        // Количество элементов в секции
#define cValsSectionsMaxCount		(cSectionsMaxCount*2)  // Максимальное количество валов в секциях, режим "Механические сеялки"
#define cValsMaxCount				cValsSectionsMaxCount  // Максимальное количество валов дозатора
#define cSectionBunkersMaxCount 	(cSectionsMaxCount*2)  // Максимальное количество бункеров в секциях, режим "Механические сеялки"
#define cValsMaxCountCmd 			3        // Максимальное количество валов дозатора в команде управления
#define cFansMaxCount 				2        // Максимальное количество вентиляторов
#define cMaxCountCulture 			10       // Максимальное количество культур для автоматической нормы высева
#define cMaxSensorDepth				4        // Максимальное количество датчиков глубины

#define cMaxCountVolumeLiquid		3    // Максимальное количество емкостей с жидкими удобрениями
#define cMaxCountLiquid				10   // Максимальное количество наименований жидких удобрений

#define cSensorsCombineMaxCount 	10   // Максимальное количество датчиков урожайности (комбайна)

#define cMaxModules 				3        // Максимальное количество модулей  (зерна или удобрений, точный высев)
#define cMaxSensors					12       // Максимальное количество датчиков (зерна или удобрений, точный высев)
#define cMaxSensorsTv2				24       // Максимальное количество датчиков зерна (точный высев 2)

#define   cTimeOutMuteSec         5    // При частых тревогах голос не чаще, чем данный тайм-аут (одновременно несколько тревог одного вида)
#define   cTimeOutRepeatLostSec   30   // Если потеря устройства не уходит, надпоминать голосом через данный тайм-аут
#define   cTimeOutRepeatSec       180  // old:60. Если тревога не уходит, четыре надпоминания голосом через данный тайм-аут
#define   cTimeOutRepeatSec5      300  // Если тревога не уходит, пятое и далее надпоминание голосом через данный тайм-аут

#define   cTimeOutValStopSec      10   // Таймаут голоса "останов вала дозатора"

#define   cCountConfirmAlarm      2    // кол-во тревожных подряд идущих сообщений, чтобы объявить тревогу
#define   cCountConfirmAlarmSem   10   // кол-во подряд идущих засорений, чтобы засорение показать на экране и зафиксировать в протоколе
#define   cCountConfirmAlarmSemTv 3    // old:10 (2020.03) кол-во подряд идущих засорений, чтобы засорение показать на экране и зафиксировать в протоколе
#define   cCountConfirmAlarmVal   10   // кол-во подряд идущих остановов вала дозатора, чтобы останов показать на экране и зафиксировать в протоколе

#define   cMaskaRealTimeCoord1sec  0x01 // передавать координаты раз в сек
#define   cMaskaRealTimeCoord2sec  0x80 // передавать координаты раз в 2 сек
#define   cMaskaRealTimeCoord5sec  0x02 // передавать координаты раз в 5 сек
#define   cMaskaRealTimeState      0x04 // передавать состояние
#define   cMaskaRealTimeValue      0x08 // передавать значение
#define   cMaskaRealTimeDepth      0x10 // передавать датчики глубины
#define   cMaskaRealTimeGisto      0x20 // передавать гистограмму
#define   cMaskaRealTimeOptions    0x40 // передавать настройки

#define cIntervals 					10            // Количество интервалов, на которое делим значение энкодера от максимума до минимума
#define cSowNormaMax 				300.0f		// Максимальная норма высева в кг
// constants ---------------------------------------------------------------------------------------

// Enums +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
enum Agro_Mode { AgroModeSow, AgroModeTv, AgroModeNavig1, AgroModeNavig2, AgroModeTv2, AgroModeSprayer, AgroModeCombine, AgroModeCombine2, AgroModeMechSow };
// 0 -Посевной_комплекс, 1 -Точный_высев1, 2 -Точный_высев2, 3 -Навигатор1, 4-Навигатор2, 5 -Комбайн(датчики урожайности)
enum ModeOptEdit { ModeNone, ModeCommon, ModeSpecial, ModeCounts, ModeSowNorma, ModeSowNormaOptions, ModeMenu, ModeDepth,
                       ModeLiquidOptions, ModeLiquidName0changed, ModeLiquidName1changed, ModeLiquidName2changed };
enum Speed_Method {  SpeedNone=0, SpeedGPS=1, SpeedEncoder=2, SpeedFixing7=3 };
enum Type_Work { TypeSow, TypePlough, TypeHarrow}; // сев, вспашка, боронование
enum Scheme_Agregate {  Scheme_AgrCultBunk, Scheme_AgrBunkCult, Scheme_Agr, Scheme_SpraySelf, Scheme_SprayTrailer };
enum SowNormaMode { SowNorma_Off, SowNorma_Base, SowNorma_Auto, SowNorma_Auto2 };
enum SendRealTime { SendRealTime_off, SendRealTime_internet, SendRealTime_autograph };
// Enums -------------------------------------------------------------------------------------------

struct AppSettings_s {
	uint8_t AgroMode;
	uint8_t AgroDemo;

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
    bool    Gisto_Show;			// Показывать ли окно гистограммы
    bool    GistoYellowExist;   // Желтая зона есть
    int     GistoYellowProcent; // Желтая зона гистограммы (%)
    bool    GistoRedExist;      // Красная зона есть
    int     GistoRedProcent;    // Красная зона гистограммы (%)
    bool    PlugDownInversion;  // Концевик заглубления инверсный
    bool    PlugDownForce;      // Концевик замкнуть (для случая, когда неисправен(потерян) концевик)
    bool    DepthSensorsExists; // Датчики глубины подключены
    uint32_t    DepthSensorsMaskaOn;// Маска подключенных(логически) датчиков глубины (им посылать запрос, остальным нет)
    bool    FixChangeStream;    // Фиксировать изменение потока
//    int     SpraySectionsCount; // Количество секций Опрыскивателя
//    double  SpraySectionsLength[cSpraySectionsMaxCount];   // длина секции Опрыскивателя в метрах
//    int     SprayInjectorsCount;// количество крайних отключаемых форсунок
//    int     SprayOverProcent;   // Перекрытие в процентах (порог отключения секции)
//    int     SprayDelaySec;      // Задержка в сек (на рисование трека после открытия Главного крана)
//    int     SprayImpLiter;      // Импульсов на литр
//    int     SprayMaxSpeed;      // Максимальная скорость км/час
//    int     SprayNorma;         // Норма внесения литр/га
//    bool    SprayOnOff;         // Концевой выключатель
//    bool    SprayAutoOn;        // Автоматическое включение обработки при скорости более ХХ км/час
//    int     SpraySpeedOn;       // Скорость км/час, при которой Виртуальное включение

    int     SensorsCombineCount;    // Количество датчиков урожайности комбайна
    int     ScrapersCount[   cSensorsCombineMaxCount];  // кол-во скребков за каждым датчиком урожайности
    int     SensorCombineAdr[cSensorsCombineMaxCount];  // адреса датчиком урожайности

    int     ScraperWidth;       // Ширина скребка
    int     ScraperDepth;       // Глубина скребка
    int     ScraperDist;        // Расстояние между скребками
    int     CombineSumVol;      // Суммировать объемы за ХХ сек

    int     SectionExists[cSectionsMaxCount][cSectionItemCount];      // конфигурация механических сеялок
 
// Add
    int     SpeedMethod;                    // Метод определения скорости агрегата
    double  SpeedEncoderConstant;           // Константа Энкодера скорости мм/имульс
    double  SpeedEncoderImp2metr;           // импульсов/метр энкодера колеса скорости
    bool    EncoderCalibrateControl;        // Контроль калибровки энкодера включен (точный высев2)
    int     ProcentAlarmCalibrateEncoder;   // При отклонении скорости по Энкодеру от скорости GPS более, чем на %, показать СБОЙ КАЛИБРОВКИ ЭНКОДЕРА (точный высев2)

    bool    Fan_Gidro;
    bool    Fan_Dvigatel;

    bool    TimeSync;           // время синхронизировать по GPS
    int     TimeRegion;         // часовой пояс (добавка в часах к UTC)

    char *PortContr;          // СОМ-порт соединения с контроллером
    int PortSpeed;          // скорость СОМ-порта соединения с контроллером
    struct {
		char *dev;
		int speed;
		unsigned pass;
	}PortControl;
    char PaswCommon[64];         // Пароль

// Special
    int     SpeedSowMax;                    // максимальная скорость агрегата
    char PaswSpecial[64];
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

    int     SowNormaIndexCulture[cValsMaxCount];                // номер от 0 культуры
    int     SowNormaBaseImp[    cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (импульсов)               Калибровка культутры
    double  SowNormaBaseKg[     cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (кг) (1/10 нормы высева)  Калибровка культутры
    int     SowNormaAutoImp[    cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (импульсов)               Калибровка культутры
    double  SowNormaAutoKg[     cValsMaxCount][cMaxCountCulture];   // Норма высева расчетная (кг) (1/10 нормы высева)  Калибровка культутры
    double  SowNormaSet[    cValsMaxCount];                     // Норма высева, установленная пользователем кнопкой "изменение норма высева"
    int     SowNormaPotenc[ cValsMaxCount];                     // Значение потенциометра для установленной нормы высева
    char  *nameCulture[    cValsMaxCount][cMaxCountCulture];   // наименование культуры
    bool    SowNormaDebug;                                      // Отладка нормы высева

    int     SowNormaTv_Duration;
    int     SowNormaTv_Interval;
    double  SowNormaTv_Count2m;         // установленная линейная норма высева (зерен на метр)
    int     SowNormaTv_ProcentPass;     // Если отклонение от нормы более установленного процента -Окрасить желтым и пикнуть три раза
    int     SowNormaTv_ProcentDouble;   // Если отклонение от нормы более установленного процента -Окрасить желтым и пикнуть три раза
    bool    SowNormaTv_ShowAuto;        // При выглублении открыть окно на 7сек
    int     SowNormaTv_Average;         // Усреднение гистограммы нормы высева 5сек, 10сек, 100сек, Загонка
    bool    SowNormaTv_ModeSowAlarm;    // Режим засорений (фиксировать только засорения, двойники и пропуски не фиксировать)
    int     SowNormaTv_ProcentAlarm;    // Если счетчики зерен за секунду ниже установленного процента - засорение
    char *SowNormaTv_Culture;         // Наименование культуры

// SowNorma Options. Скорость по энкодеру.
    double  ScaleCoef0[cValsMaxCount];  // Коэффициент поправочный (весы 1)
    double  ScaleCoef1[cValsMaxCount];  // Коэффициент поправочный (весы 2)
    int     ScaleProcent;               // Максимальный гистерезис при взвешивании
    int     SowNormaMax;                // Максимальная норма высева кг/га
    bool    SowNormaMin15;              // Минимальная норма высева 15 кг/га
    uint16_t  Potencio[cValsMaxCount][cIntervals + 1];    // Значения потенциометра(вал штока).   Норма высева, если скорость по энкодеру
    double  Imp2metr[cValsMaxCount][cIntervals + 1];    // Импульсов/метр вала высевающего.     Норма высева, если скорость по энкодеру

// SowNorma Options. Дифференциальное внесение
    bool    DifEntry;       // Дифференциальное внесение ВКЛЮЧИТЬ
    char* DifEntryName[cValsMaxCountCmd];

// Counts
    bool    BunkerHide[cBunkersMaxCount];

// Security
    int     volumeProcent;      // процент громкости
    int     brightnessProcent;  // процент яркости
    bool    sowNormaAutoAverage;// усреднять норму высева (при отображении)
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
//    double  SprayGpsToRod;      // Расстояние от GPS-приемника до штанги
//    double  SprayGpsToPoint;    // Расстояние от GPS-приемника до сцепки трактора
//    double  SprayPointToRod;    // Расстояние от сцепки до штанги
    char *GpsPort;            // Порт GPS-приемника
    int GpsSpeed;           // Скорость GPS-приемника
    int     StoreRealTime;      // Маска сохранения протоколов в режиме реального времени
// Internet
    int     sendRealTime;       // SendRealTime_off, SendRealTime_internet, SendRealTime_autograph
    char *tcpIpServer;
    char *owner;
    char *password;
    char *agrName;
    char *agrNameTv;
    int     indexProviderSim0;
    int     indexProviderSim1;
    char *PortModem;          // порт GSM-модема
    int PortModemSpeed;     // скорость порта GSM-модема

// SemensOff
    bool SemensOff[cTowersMaxCount][cSemensMaxCount];   // Отключенные датчики зерна

    bool SemensCropsOff[cMaxModules][cMaxSensors];      // Отключенные датчики зерна        (точный высев)
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
    char *nameLiquid[     cMaxCountLiquid];           // наименование жидких удобрений
};
#ifdef __SETTINGS_C__
struct AppSettings_s appSettings;
#else
extern struct AppSettings_s appSettings;
#endif
const char* speed_Method_ToString(int speedMethod);
double settings_get_speed();
void read_options(const char *file);
#endif
