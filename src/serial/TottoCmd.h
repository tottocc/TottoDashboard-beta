#ifndef _TOTTOCMD_H_
#define _TOTTOCMD_H_

#include "CoreCmd.h"

class TottoCmd : public CoreCmd
{
public:
	TottoCmd();
	~TottoCmd();

	void CtrlPort(unsigned int ch, bool on);

protected:

private:
};

#endif /* _TOTTOCMD__H_ */
