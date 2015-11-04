#include <stdlib.h>
#include "fifo.h"
#include "Serial.h"
#include <string>
using namespace std;

// Based on the following URL:
// http://d.hatena.ne.jp/udp_ip/20110401/1301677438

#define msleep(X) usleep((X) * 1000)


///////////////////////////////////////////////////////////////////////////////
/// local variable/function prototype
///////////////////////////////////////////////////////////////////////////////

/* �֐��v���g�^�C�v */
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
}

Serial::~Serial()
{
}

unsigned int Serial::UpdateComPortList()
{
  return 0;
}

unsigned int Serial::GetComPortNum(unsigned int)
{
  return 0;
}

char *Serial::GetComPortDesc(unsigned int num)
{
  return NULL;
}

bool IsOpen()
{
//	return _serial_obj != NULL;
}

//bool Serial::Create(unsigned int num, unsigned int baud)
//{
	// Set port name
/*
	char *p = _pnamestr;
	*p++ = '\\';
	*p++ = '\\';
	*p++ = '.';
	*p++ = '\\';
	while (*p++ = *comname++) {};
*/

//	_serial_obj = serial_create(_pnamestr, baud);
//	return _serial_obj != NULL;
//  return 0;
//}

/*
void Serial::Delete()
{
	serial_delete(_serial_obj);
}
*/

unsigned int Serial::Send(const BYTE *buf, unsigned int size)
{

	serial_send(_serial_obj, buf, size);
	return 0;
}

unsigned int Serial::Recv(BYTE *buf, unsigned int size)
{
	serial_recv(_serial_obj, buf, size);
	return 0;
}

unsigned int Serial::RecvLength()
{
	serial_recv_length(_serial_obj);
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// local function
///////////////////////////////////////////////////////////////////////////////

/* 1�̃V���A���ʐM�Ɋւ����f�[�^�\�� */
struct _TAG_SERIAL {
	// FIFO
	fifo_t *q_recv;
	fifo_t *q_send;

	// ���̑�
	char *pname;
	char *msg;
};

/* �v���g�^�C�v */
//DWORD WINAPI serial_thread(LPVOID param);

/* 1���o�b�t�@ */
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
