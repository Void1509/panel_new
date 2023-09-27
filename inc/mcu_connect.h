#ifndef __MCU_CONNECT_H__
#define __MCU_CONNECT_H__

#define MCUCmdMark		0xAA
#define MCUAckMark		0x55

typedef enum {
	MCU_Ok = 0,
	ErrorOpenPort = 1,
	ErrorBaud = 2,
	ErrorNoPort = 3,
}MCUError;

typedef struct __attribute__((packed)){
	uint8_t mark;
	uint8_t cmd;
	uint16_t len;
	uint8_t data[];
}MCUCmdPack_t;

typedef struct __attribute__((packed)){
	uint8_t mark;
	uint8_t cmd;
	uint16_t len;
	uint8_t data[];
}MCUAckPack_t;

struct MCUProtocolSend_s {

};

struct MCUConnect_s {
	char *dev;
	uint32_t baud;
	int fd, pIn, pOut;
	MCUError status;
};

gboolean mcu_serial_init(struct MCUConnect_s *cn);
gpointer mcu_serial_thread(gpointer data);
void mcu_send_commands();
gboolean mcu_read_pipe(GIOChannel *ch, GIOCondition con, gpointer p);

#endif
