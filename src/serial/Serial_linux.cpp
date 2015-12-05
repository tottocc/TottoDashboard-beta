#include <stdlib.h>
#include "fifo.h"
#include "Serial.h"
#include <string>
using namespace std;

// Based on the following URL:
//

#define msleep(X) usleep((X) * 1000)


///////////////////////////////////////////////////////////////////////////////
/// local variable/function prototype
///////////////////////////////////////////////////////////////////////////////

serial_t serial_create(char *pname, unsigned int baud);
void serial_delete(serial_t obj);
unsigned int serial_send(serial_t obj, const BYTE *buf, unsigned int size);
unsigned int serial_recv(serial_t obj, BYTE *buf, unsigned int size);
unsigned int serial_recv_length(serial_t obj);


///////////////////////////////////////////////////////////////////////////////
/// Class Serial
///////////////////////////////////////////////////////////////////////////////

Serial::Serial()
{
    serialObj = NULL;
}

Serial::~Serial()
{
    for (int i = 0; i < comPortNum; i++) {
        free(comPortDescList[i]);
        comPortDescList[i] = NULL;
    }

    DisconnectCom();
}

unsigned int Serial::UpdateComPortList()
{
    // Describe OS specific method to find com port
    
    return comPortNum;
}

unsigned int Serial::GetComPortNum(unsigned int num)
{
  if (num >= comPortNum)
      return 0;
  return comPortNumList[num];
}

char *Serial::GetComPortDesc(unsigned int num)
{
    if(num >= comPortNum)
        return NULL;
  return comPortDescList[num];
}

unsigned int Serial::FindComPortListIndexWithName(char *name)
{
	int i;
	for (i = 0; i < comPortNum; i++) {
		if (strstr(comPortDescList[i], name)) {
			break;
		}
	}
	if (i == comPortNum)	
		return 0;
	return i;
}

bool Serial::ConnectCom(unsigned int num, unsigned int baud)
{
	char comname[11];
	sprintf(comname, "\\\\.\\COM%d", GetComPortNum(num));
	serialObj = serial_create(comname, baud);
	return serialObj != NULL;
}

bool IsOpen()
{
	return serialObj != NULL;
}

void Serial::DisconnectCom()
{
	if (IsOpen()) {
		serialDelete(serialObj);
		serialObj = NULL;
	}
}

unsigned int Serial::Send(const BYTE *buf, unsigned int size)
{
	return serial_send(serialObj, buf, size);
}

unsigned int Serial::Recv(BYTE *buf, unsigned int size)
{
	return serial_recv(serialObj, buf, size);
}

unsigned int Serial::RecvLength()
{
	return serial_recv_length(serialObj);
}


///////////////////////////////////////////////////////////////////////////////
/// local function
///////////////////////////////////////////////////////////////////////////////

struct _TAG_SERIAL {

    fifo_t *q_recv;
	fifo_t *q_send;

	char *pname;
	char *msg;
};

//DWORD WINAPI serial_thread(LPVOID param);

#define SERIAL_TMP_BUFSIZE	128

serial_t serial_create(char *pname, unsigned int baud)
{
	serial_t obj;
	return obj;
}

void serial_delete(serial_t obj)
{
}


unsigned int serial_send(serial_t obj, const BYTE *buf, unsigned int size)
{
	unsigned int ret;
	return ret;
}

unsigned int serial_send(serial_t obj, const char *buf, unsigned int size)
{
	return serial_send(obj, (BYTE *)buf, size);
}

unsigned int serial_recv(serial_t obj, BYTE *buf, unsigned int size)
{
	unsigned int ret;
	return ret;
}

unsigned int serial_recv_length(serial_t obj)
{
	return fifo_length(obj->q_recv);
}
