#ifndef _CHOPPERCMD_H_
#define _CHOPPERCMD_H_

#include "CoreCmd.h"

class ChopperCmd : public CoreCmd
{
public:
	ChopperCmd();
	~ChopperCmd();

	void CtrlPort(unsigned int ch, bool on);

protected:

private:
};

#endif /* _CHOPPERCMD__H_ */
