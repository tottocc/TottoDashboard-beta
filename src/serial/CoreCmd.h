#ifndef _CORECMD_H_
#define _CORECMD_H_

#include "Serial.h"

class CoreCmd : public Serial
{
public:
	CoreCmd();
	~CoreCmd();

protected:
	bool SendCmd(char *cmd, unsigned int t);
	char *RecvResponse(char *buf);

private:

};

#endif /* _CORECMD__H_ */
