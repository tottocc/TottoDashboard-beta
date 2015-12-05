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
	GUID ClassGuid[1];
	DWORD dwRequiredSize;
	BOOL bRet;
	HDEVINFO DeviceInfoSet = NULL;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD dwMemberIndex = 0;
	int i;

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // Initialize list
	 for (i = 0; i < comPortNum; i++) {
		free(comPortDescList[i]);
		comPortDescList[i] = NULL;
	}
	comPortNum = 0;

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
				comPortNumList[comPortNum] = atoi(&szPortName[3]);
				comPortDescList[comPortNum] = _strdup(szFriendlyName);
				comPortNum++;
			}
		}
	}

cleanup:
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	
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
	if (num >= comPortNum)
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

bool Serial::IsOpen()
{
	return serialObj != NULL;
}


void Serial::DisconnectCom()
{
	if (IsOpen()) {
		serial_delete(serialObj);
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

    HANDLE handle;
	DCB dcb;

	HANDLE thread_handle;
	DWORD thread_id;
	BOOL thread_active;
	CRITICAL_SECTION cs_send;
	CRITICAL_SECTION cs_recv;

	fifo_t *q_recv;
	fifo_t *q_send;

	char *pname;
	char *msg;
};

DWORD WINAPI serialThread(LPVOID param);

#define SERIAL_TMP_BUFSIZE	128

serial_t serial_create(char *pname, unsigned int baud)
{
	serial_t obj;
	COMMTIMEOUTS timeout;

	obj = (serial_t)malloc(sizeof(struct _TAG_SERIAL));
	if (obj == NULL) return NULL;
	ZeroMemory(obj, sizeof(struct _TAG_SERIAL));
	obj->pname = pname;

	obj->handle = CreateFile(pname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_OVERLAPPED*/, NULL);
	if (obj->handle == INVALID_HANDLE_VALUE) {
		free(obj);
		return NULL;
	}

	GetCommState(obj->handle, &obj->dcb);
	obj->dcb.BaudRate = baud;
	if (SetCommState(obj->handle, &obj->dcb) == FALSE) {
		free(obj);
		return NULL;
	}

	ZeroMemory(&timeout, sizeof(COMMTIMEOUTS));
	timeout.ReadIntervalTimeout = MAXDWORD;
	if (SetCommTimeouts(obj->handle, &timeout) == FALSE) {
		free(obj);
		return NULL;
	}

	obj->q_send = fifo_create();
	obj->q_recv = fifo_create();
	if (obj->q_send == NULL || obj->q_recv == NULL) {
		fifo_delete(obj->q_send);
		fifo_delete(obj->q_recv);
		free(obj);
		return NULL;
	}

	InitializeCriticalSection(&obj->cs_recv);
	InitializeCriticalSection(&obj->cs_send);
	obj->thread_active = TRUE;
	obj->thread_handle = CreateThread(NULL, 0, serialThread, (LPVOID *)obj, 0, &obj->thread_id);
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

    obj->thread_active = FALSE;
	do {
		Sleep(1);
		GetExitCodeThread(obj->thread_handle, &thread_state);
	} while (thread_state == STILL_ACTIVE);
	DeleteCriticalSection(&obj->cs_send);
	DeleteCriticalSection(&obj->cs_recv);

	CloseHandle(obj->handle);

	fifo_delete(obj->q_send);
	fifo_delete(obj->q_recv);
	free(obj);
}

DWORD WINAPI serialThread(LPVOID param)
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
