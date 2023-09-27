#ifndef __MCU_DATA_H__
#define __MCU_DATA_H__
#include <stdint.h>

#define RBUF_MAX 		4096

struct ReadBuf_s {
	uint16_t inx;
	uint8_t buf[RBUF_MAX];
};

struct Scales_s {		// 0x60
	uint8_t Num;		// Номер весов
	uint16_t weight;		// вес
};

struct BunkerFillingLevel_s {	// 0x61
	uint8_t bNum;		// Номер бункера
	uint8_t sens;		// Sensors <hight 4 bit - hight sensor> <low 4 bit low sensor>
};

struct PressureSensor_s {	// 0x62
	uint8_t Num;		// Номер датчика
	uint8_t kPa;		// Давление в килопаскалях
};

struct DeepSensor_s {	// 0x63
	uint8_t Num;		// Номер датчика
	uint8_t sens;		// 0 - датчик замкнут; 1 - датчик разомкнут
};

struct SeedingShaftSensor_s {	// 0x64
	uint8_t Num;		// Номер вала
	uint16_t count;		// счетчик вала
};

struct CoolerSensor_s {	// 0x65
	uint8_t Num;		// Номер вентилятора
	uint16_t count;		// счетчик вентилятора
};

struct EncoderSensor_s {	// 0x66
//	uint8_t Num;		// 0 - передача счетчика энкодера 1 - передача интервала в мс
	uint16_t count;		// счетчик
	uint16_t ms;		// интервал в мс
};

struct SeminalSensor_s {	// 0x67
	uint8_t Num;		// <7..4> - номер башни <3..0> - номер семяпровода
	uint16_t count;		// 
};

struct OverVoltage_s {	// 0x68
	uint8_t Num;		// всегда 0
	uint16_t volt;		// напряжение
};

struct FuelCount_s {	// 0x69
	uint8_t Num;		// всегда 0
	uint16_t fuel;		// напряжение
};

struct ServiceInfo_s {	// 0x6a
	uint16_t volt;		// напряжение питания
	uint16_t volt10;	// напряжение стабилизатора 10В
	uint16_t volt19;	// напряжение стабилизатора 19В
	uint16_t voltRes;	// напряжение резерва
	uint16_t flags;		// флаги
};

struct PulsePUA_s {		// 0x6b
	uint8_t Num;
	uint16_t pulse;
};

struct PulsePUAms_s {	// 0x6c
	uint8_t Num;
	uint16_t ms;
};

struct DataPUA_s {
	unsigned errPower:1;
	unsigned errAct:1;
	unsigned actDown:1;
	unsigned actUp:1;
	unsigned reserv:1;
	unsigned ADC:11;
};

struct ProxiSensor_s {
	uint16_t count[3];
};

struct MCUProgVers_s {
	uint8_t maj, min;
};

struct MCUData_s {
	uint16_t scale[3];				// 0x60 Весы (вес) 
	uint8_t bunkers[3];				// 0x61 датчик бункера <hight 4 bit - hight sensor> <low 4 bit low sensor> 
	uint8_t psens[3];				// 0x62 Датчик давления(Люк)
	uint8_t dsens;					// 0x63 Датчик заглубления
	uint16_t shaft[3];				// 0x64 Датчик вала высевающего
	uint16_t fans[3];				// 0x65 Датчик вентилятора
	struct EncoderSensor_s enSpeed;	// 0x66 Датчик скорости по Энкодеру
	uint16_t seminal[16][16];		// 0x67 Датчик семяпровода
	uint16_t volt;					// 0x68 Превышение напряжения в бортовой сети
	uint16_t fuel;					// 0x69 Счётчик топлива
	uint16_t service[5];			// 0x6a Сервисная информация
};

struct MCUProtocolAck_s {
	uint8_t type, len;			// тип блока (команда), длина пакета (параметров)
	uint8_t hi, lo;				// пакет (параметры)
}__attribute__((packed));

struct MCUProtocolCmd_s {
	uint8_t mark;				// маркер
	uint8_t type, len;			// тип блока (команда), длина пакета (параметров)
	uint8_t data[];				// пакет (параметры)
}__attribute__((packed));

#ifdef __MCU_SERIAL_C__
struct ReadBuf_s readBuf = {.inx = 0};
struct MCUData_s mcuData;
#else
extern struct ReadBuf_s readBuf;
extern struct MCUData_s mcuData;
#endif

#endif
