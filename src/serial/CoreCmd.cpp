#include "CoreCmd.h"
#include "fifo.h"
#include <stdlib.h>
#include <string>
using namespace std;

//
// Macro
//
#ifndef __GNUC__
#define sprintf sprintf_s
#endif

CoreCmd::CoreCmd()
{
}

CoreCmd::~CoreCmd()
{
}

bool CoreCmd::SendCmd(char *cmd, unsigned int t)
{
	char buf[FIFO_BUFSIZE];

	Send((BYTE *)cmd, strlen(cmd));
	Send((BYTE *)"\r", 1);
	msleep(t);       // Wait for response
	return !strcmp(RecvResponse(buf), cmd);
}

char *CoreCmd::RecvResponse(char *buf)
{
	unsigned int len = RecvLength();
	if (len<2) {
		*buf = NULL;
	}
	else {
		Recv((BYTE *)buf, len);
		buf[len-2] = NULL;     // Remove "\r\n"
	}
	return buf;
}



