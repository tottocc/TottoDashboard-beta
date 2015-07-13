#include "ChopperCmd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

//
// Macro
//
#ifndef __GNUC__
#define sprintf sprintf_s
#endif


ChopperCmd::ChopperCmd()
{
}

ChopperCmd::~ChopperCmd()
{
}

//
// Control USB Chopper Port
//
void ChopperCmd::CtrlPort(unsigned int ch, bool on)
{
	char buf[12];

	if (on == true) {
		sprintf(buf, "usb en %d", ch);
	}
	else {
		sprintf(buf, "usb dis %d", ch);
	}
	if (!SendCmd(buf, 50)) {  // 50 msec wait
		fprintf(stderr, "Fail to send command\r\n");
	}

}
