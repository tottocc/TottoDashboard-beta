#ifndef _SERIAL_H_
#define _SERIAL_H_

#define COM_PORT_MAX 100  // List up max 100 com ports

typedef unsigned char BYTE;
typedef struct _TAG_SERIAL* serial_t;

class Serial
{
public:
	Serial();
	~Serial();

//	int FindPortNum(char *name);
	//	unsigned int ListupSerialPort(unsigned short *ComPortTable, char **ComPortDesc, int ComPortMax);
	unsigned int UpdateComPortList();
	unsigned int GetComPortNum(unsigned int);
	char *GetComPortDesc(unsigned int num);
	bool Create(unsigned int num, unsigned int baud);
	bool IsOpen();
	void Delete();
	unsigned int Send(const BYTE *buf, unsigned int size);
	unsigned int Send(const char *buf, unsigned int size);
	unsigned int Recv(BYTE *buf, unsigned int size);
	unsigned int RecvLength();

protected:

private:
	serial_t _serial_obj;
	unsigned int _comPortNum;
	unsigned int _comPortNumList[COM_PORT_MAX];
	char *_comPortDescList[COM_PORT_MAX];
};

#endif /* _SERIAL_H_ */
