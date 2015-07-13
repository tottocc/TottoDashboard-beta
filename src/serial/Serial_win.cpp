#include "Serial.h"
#include "fifo.h"
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <stdio.h>
using namespace std;


// Required to get device informmation
#include <devguid.h>
#include <regstr.h>
#include <setupapi.h>
#pragma	comment(lib,"setupapi.lib")

// Based on the following URL:
// http://d.hatena.ne.jp/udp_ip/20110401/1301677438
// http://ttssh2.osdn.jp/manual/ja/reference/sourcecode.html

//
// Macro
//
#ifndef __GNUC__
#define sprintf sprintf_s
#endif

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
	_serial_obj = NULL;
}

Serial::~Serial()
{
	for (int i = 0; i < _comPortNum; i++) {
		free(_comPortDescList[i]);
		_comPortDescList[i] = NULL;
	}

	DisconnectCom();
}

unsigned int Serial::UpdateComPortList()
{
	GUID ClassGuid[1];
	DWORD dwRequiredSize;
	BOOL bRet;
	HDEVINFO DeviceInfoSet = NULL;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD dwMemberIndex = 0;
	int i;

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // Initialize list
	 for (i = 0; i < _comPortNum; i++) {
		free(_comPortDescList[i]);
		_comPortDescList[i] = NULL;
	}
	_comPortNum = 0;

	bRet =
		SetupDiClassGuidsFromName(_T("PORTS"), (LPGUID)& ClassGuid, 1,
		&dwRequiredSize);
	if (!bRet) {
		goto cleanup;
	}

	DeviceInfoSet =
		SetupDiGetClassDevs(&ClassGuid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);

	if (DeviceInfoSet) {
		dwMemberIndex = 0;
		while (SetupDiEnumDeviceInfo
			(DeviceInfoSet, dwMemberIndex++, &DeviceInfoData)) {
			TCHAR szFriendlyName[MAX_PATH];
			TCHAR szPortName[MAX_PATH];
			DWORD dwReqSize = 0;
			DWORD dwPropType;
			DWORD dwType = REG_SZ;
			HKEY hKey = NULL;

			bRet = SetupDiGetDeviceRegistryProperty(DeviceInfoSet,
				&DeviceInfoData,
				SPDRP_FRIENDLYNAME,
				&dwPropType,
				(LPBYTE)
				szFriendlyName,
				sizeof(szFriendlyName),
				&dwReqSize);

			hKey = SetupDiOpenDevRegKey(DeviceInfoSet,
				&DeviceInfoData,
				DICS_FLAG_GLOBAL,
				0, DIREG_DEV, KEY_READ);
			if (hKey) {
				long lRet;
				dwReqSize = sizeof(szPortName);
				lRet = RegQueryValueEx(hKey,
					_T("PortName"),
					0,
					&dwType,
					(LPBYTE)& szPortName,
					&dwReqSize);
				RegCloseKey(hKey);
			}

			if (_strnicmp(szPortName, "COM", 3) == 0) {  // Find COM port driver
				_comPortNumList[_comPortNum] = atoi(&szPortName[3]);
				_comPortDescList[_comPortNum] = _strdup(szFriendlyName);
				_comPortNum++;
			}
		}
	}

cleanup:
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	
	return _comPortNum;
}


unsigned int Serial::GetComPortNum(unsigned int num)
{
	if (num >= _comPortNum)
		return 0;
	return _comPortNumList[num];
}


char *Serial::GetComPortDesc(unsigned int num)
{
	if (num >= _comPortNum)
		return NULL;
	return _comPortDescList[num];
}


bool Serial::ConnectCom(unsigned int num, unsigned int baud)
{
	char comname[11];
	sprintf(comname, "\\\\.\\COM%d", GetComPortNum(num));
	_serial_obj = serial_create(comname, baud);
	return _serial_obj != NULL;
}

bool Serial::IsOpen()
{
	return _serial_obj != NULL;
}


void Serial::DisconnectCom()
{
	if (IsOpen()) {
		serial_delete(_serial_obj);
		_serial_obj = NULL;
	}
}

unsigned int Serial::Send(const BYTE *buf, unsigned int size)
{
	return serial_send(_serial_obj, buf, size);
}

unsigned int Serial::Recv(BYTE *buf, unsigned int size)
{
	return serial_recv(_serial_obj, buf, size);
}

unsigned int Serial::RecvLength()
{
	return serial_recv_length(_serial_obj);
}


///////////////////////////////////////////////////////////////////////////////
/// local function
///////////////////////////////////////////////////////////////////////////////

/* 1�̃V���A���ʐM�Ɋւ����f�[�^�\�� */
struct _TAG_SERIAL {
	// �ʐM�֌W
	HANDLE handle;
	DCB dcb;

	// �X���b�h�Ɋւ���
	HANDLE thread_handle;
	DWORD thread_id;
	BOOL thread_active;
	CRITICAL_SECTION cs_send;
	CRITICAL_SECTION cs_recv;

	// FIFO
	fifo_t *q_recv;
	fifo_t *q_send;

	// ���̑�
	char *pname;
	char *msg;
};

/* �v���g�^�C�v */
DWORD WINAPI serial_thread(LPVOID param);

/* 1���o�b�t�@ */
#define SERIAL_TMP_BUFSIZE	128

serial_t serial_create(char *pname, unsigned int baud)
{
	serial_t obj;
	COMMTIMEOUTS timeout;

	// �C���X�^���X�������m��
	obj = (serial_t)malloc(sizeof(struct _TAG_SERIAL));
	if (obj == NULL) return NULL;
	ZeroMemory(obj, sizeof(struct _TAG_SERIAL));
	obj->pname = pname;

	// COM�|�[�g�̃n���h�����擾
	obj->handle = CreateFile(pname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_OVERLAPPED*/, NULL);
	if (obj->handle == INVALID_HANDLE_VALUE) {
		free(obj);
		return NULL;
	}

	// COM�|�[�g�̒ʐM�ݒ�
	GetCommState(obj->handle, &obj->dcb);
	obj->dcb.BaudRate = baud;
	if (SetCommState(obj->handle, &obj->dcb) == FALSE) {
		free(obj);
		return NULL;
	}

	// COM�|�[�g�̃^�C���A�E�g�ݒ�
	ZeroMemory(&timeout, sizeof(COMMTIMEOUTS));
	timeout.ReadIntervalTimeout = MAXDWORD;
	if (SetCommTimeouts(obj->handle, &timeout) == FALSE) {
		free(obj);
		return NULL;
	}

	// FIFO�������m��
	obj->q_send = fifo_create();
	obj->q_recv = fifo_create();
	if (obj->q_send == NULL || obj->q_recv == NULL) {
		fifo_delete(obj->q_send);
		fifo_delete(obj->q_recv);
		free(obj);
		return NULL;
	}

	// �X���b�h�J�n
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

	// �X���b�h�����~
	obj->thread_active = FALSE;
	do {
		Sleep(1);
		GetExitCodeThread(obj->thread_handle, &thread_state);
	} while (thread_state == STILL_ACTIVE);
	DeleteCriticalSection(&obj->cs_send);
	DeleteCriticalSection(&obj->cs_recv);

	// �ʐM�|�[�g������
	CloseHandle(obj->handle);

	// �������[�̈��̉���
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
		// ���M
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

		// ���M
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
