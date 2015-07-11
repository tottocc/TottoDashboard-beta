#ifndef _FIFO_H_
#define _FIFO_H_

#define FIFO_BUFSIZE	1024

typedef unsigned char BYTE;

/* FIFO data structure */
typedef struct _TAG_FIFO {
	unsigned char buf[FIFO_BUFSIZE];
	unsigned int size;
	unsigned int read;
	unsigned int write;
}fifo_t;

/* Prototype */
fifo_t *fifo_create(void);
void fifo_delete(fifo_t *obj);
unsigned int fifo_write(fifo_t *obj, const BYTE *buf, unsigned int size);
unsigned int fifo_read(fifo_t *obj, BYTE *buf, unsigned int size);
unsigned int fifo_length(fifo_t *obj);

#endif /* _FIFO_H_ */