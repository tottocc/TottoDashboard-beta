#ifndef _SERIAL_H_
#define _SERIAL_H_

#define COM_PORT_MAX 100  // List up max 100 com ports

typedef unsigned char BYTE;
typedef struct _TAG_SERIAL* serial_t;

//
// Macro
//
#ifdef _WIN32
#include <windows.h>
#define msleep(X) Sleep(X)
#else
#define msleep(X) usleep((X) * 1000)
#endif

class Serial
{
public:
	Serial();
	~Serial();

	unsigned int UpdateComPortList();
	unsigned int GetComPortNum(unsigned int);
	char *GetComPortDesc(unsigned int num);
	unsigned int FindComPortListIndexWithName(char *name);
	bool ConnectCom(unsigned int num, unsigned int baud);
	bool IsOpen();
	void DisconnectCom();

protected:
	unsigned int Send(const BYTE *buf, unsigned int size);
	unsigned int Send(const char *buf, unsigned int size);
	unsigned int Recv(BYTE *buf, unsigned int size);
	unsigned int RecvLength();

private:
	serial_t serialObj;
	unsigned int comPortNum;
	unsigned int comPortNumList[COM_PORT_MAX];
	char *comPortDescList[COM_PORT_MAX];
};

#endif /* _SERIAL_H_ */
