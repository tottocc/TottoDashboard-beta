#include <windows.h>
#include <stdlib.h>
#include "fifo.h"
#include "Serial.h"
#include <string>
using namespace std;

// Based on the following URL:
// http://d.hatena.ne.jp/udp_ip/20110401/1301677438

///////////////////////////////////////////////////////////////////////////////
/// local variable/function prototype
///////////////////////////////////////////////////////////////////////////////

/* 関数プロトタイプ */
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

bool Serial::Create(const char *comname, unsigned int baud)
{
	// Set port name
	char *p = _pnamestr;
	*p++ = '\\';
	*p++ = '\\';
	*p++ = '.';
	*p++ = '\\';
	while (*p++ = *comname++) {};

	_serial_obj = serial_create(_pnamestr, baud);
	return _serial_obj != NULL;
}

void Serial::Delete()
{
	serial_delete(_serial_obj);
}

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

/* 1つのシリアル通信に関するデータ構造 */
struct _TAG_SERIAL {
	// 通信関係
	HANDLE handle;
	DCB dcb;

	// スレッドに関して
	HANDLE thread_handle;
	DWORD thread_id;
	BOOL thread_active;
	CRITICAL_SECTION cs_send;
	CRITICAL_SECTION cs_recv;

	// FIFO
	fifo_t *q_recv;
	fifo_t *q_send;

	// その他
	char *pname;
	char *msg;
};

/* プロトタイプ */
DWORD WINAPI serial_thread(LPVOID param);

/* 1次バッファ */
#define SERIAL_TMP_BUFSIZE	128

serial_t serial_create(char *pname, unsigned int baud)
{
	serial_t obj;
	COMMTIMEOUTS timeout;

	// インスタンスメモリ確保
	obj = (serial_t)malloc(sizeof(struct _TAG_SERIAL));
	if (obj == NULL) return NULL;
	ZeroMemory(obj, sizeof(struct _TAG_SERIAL));
	obj->pname = pname;

	// COMポートのハンドルを取得
	obj->handle = CreateFile(pname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_OVERLAPPED*/, NULL);
	if (obj->handle == INVALID_HANDLE_VALUE) {
		free(obj);
		return NULL;
	}

	// COMポートの通信設定
	GetCommState(obj->handle, &obj->dcb);
	obj->dcb.BaudRate = baud;
	if (SetCommState(obj->handle, &obj->dcb) == FALSE) {
		free(obj);
		return NULL;
	}

	// COMポートのタイムアウト設定
	ZeroMemory(&timeout, sizeof(COMMTIMEOUTS));
	timeout.ReadIntervalTimeout = MAXDWORD;
	if (SetCommTimeouts(obj->handle, &timeout) == FALSE) {
		free(obj);
		return NULL;
	}

	// FIFOメモリ確保
	obj->q_send = fifo_create();
	obj->q_recv = fifo_create();
	if (obj->q_send == NULL || obj->q_recv == NULL) {
		fifo_delete(obj->q_send);
		fifo_delete(obj->q_recv);
		free(obj);
		return NULL;
	}

	// スレッド開始
	InitializeCriticalSection(&obj->cs_recv);
	InitializeCriticalSection(&obj->cs_send);
	obj->thread_active = TRUE;
	obj->thread_handle = CreateThread(NULL, 0, serial_thread, (LPVOID *)obj, 0, &obj->thread_id);
	if (obj->thread_handle == NULL) {
		DeleteCriticalSection(&obj->cs_recv);
		DeleteCriticalSection(&obj->cs_send);
		fifo_delete(obj->q_send);
		fifo_delete(obj->q_recv);
		free(obj);
		return NULL;
	}

	return obj;
}

void serial_delete(serial_t obj)
{
	DWORD thread_state;

	// スレッドを停止
	obj->thread_active = FALSE;
	do {
		Sleep(1);
		GetExitCodeThread(obj->thread_handle, &thread_state);
	} while (thread_state == STILL_ACTIVE);
	DeleteCriticalSection(&obj->cs_send);
	DeleteCriticalSection(&obj->cs_recv);

	// 通信ポートを閉じる
	CloseHandle(obj->handle);

	// メモリー領域の解放
	fifo_delete(obj->q_send);
	fifo_delete(obj->q_recv);
	free(obj);
}

DWORD WINAPI serial_thread(LPVOID param)
{
	serial_t obj = (serial_t)param;
	BYTE recv_buf[SERIAL_TMP_BUFSIZE];
	BYTE send_buf[SERIAL_TMP_BUFSIZE];
	DWORD recv_len;
	DWORD send_len, send_size;
	BOOL ret;
	BOOL recv_hold = FALSE;
	BOOL send_hold = FALSE;

	while (obj->thread_active) {
		// 受信
		if (recv_hold == FALSE) {
			ret = ReadFile(obj->handle, recv_buf, sizeof(recv_buf), &recv_len, NULL);
			if (ret == FALSE) {
				obj->msg = "ReadFile failed.";
				break;
			}
			if (recv_len)	recv_hold = TRUE;
		}
		else if (TryEnterCriticalSection(&obj->cs_recv)) {
			recv_len -= fifo_write(obj->q_recv, recv_buf, recv_len);
			LeaveCriticalSection(&obj->cs_recv);
			if (recv_len != 0)	obj->msg = "q_recv is fully filled. (>_<)";
			recv_hold = FALSE;
		}

		// 送信
		if (send_hold) {
			ret = WriteFile(obj->handle, send_buf, send_size, &send_len, NULL);
			if (ret == FALSE) {
				obj->msg = "WriteFile failed.";
				break;
			}
			if (send_size != send_len)	obj->msg = "WriteFile spilled some of q_send.";
			send_hold = FALSE;
		}
		else if (TryEnterCriticalSection(&obj->cs_send)) {
			send_size = fifo_read(obj->q_send, send_buf, sizeof(send_buf));
			LeaveCriticalSection(&obj->cs_send);
			send_hold = TRUE;
		}

		Sleep(1);
	}

	obj->thread_active = FALSE;
	ExitThread(TRUE);
	return 0;
}

unsigned int serial_send(serial_t obj, const BYTE *buf, unsigned int size)
{
	unsigned int ret;
	EnterCriticalSection(&obj->cs_send);
	ret = fifo_write(obj->q_send, buf, size);
	LeaveCriticalSection(&obj->cs_send);
	return ret;
}

unsigned int serial_send(serial_t obj, const char *buf, unsigned int size)
{
	return serial_send(obj, (BYTE *)buf, size);
}

unsigned int serial_recv(serial_t obj, BYTE *buf, unsigned int size)
{
	unsigned int ret;
	EnterCriticalSection(&obj->cs_recv);
	ret = fifo_read(obj->q_recv, buf, size);
	LeaveCriticalSection(&obj->cs_recv);
	return ret;
}

unsigned int serial_recv_length(serial_t obj)
{
	return fifo_length(obj->q_recv);
}