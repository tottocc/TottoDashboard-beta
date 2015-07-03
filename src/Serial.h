#ifndef _SERIAL_WIN_H_
#define _SERIAL_WIN_H_

typedef unsigned char BYTE;
typedef struct _TAG_SERIAL* serial_t;

class Serial
{
public:
	Serial();
	~Serial();

	bool Create(const char *comname, unsigned int baud);
	void Delete();
	unsigned int Send(const BYTE *buf, unsigned int size);
	unsigned int Send(const char *buf, unsigned int size);
	unsigned int Recv(BYTE *buf, unsigned int size);
	unsigned int RecvLength();

protected:

private:
	serial_t _serial_obj;
	char _pnamestr[15];
};

#endif /* _SERIAL_WIN_H_ */
