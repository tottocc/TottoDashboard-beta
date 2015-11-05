
#ifndef __MEASUREDATA_H__
#define __MEASUREDATA_H__

//#define SAMPLING_NUM_MAX 8640000
#define SAMPLING_NUM_MAX 1000

#include <stdlib.h>

class MeasureData
{
public:
	MeasureData() {
		num = 0;
		xdelta = 0.01;   // 10 msec interval
//		xdelta = 1;   // 1 sec interval
		for (int x = 0; x < SAMPLING_NUM_MAX; x++) {
//			ydat[x] = (rand() % 11) * 0.1;
			ydat[x] =(x % 1000) / 100;
			num++;
		}
		xmin = 0;
		xmax = xdelta * (num-1);
	};
	~MeasureData() {
	};

	int num;
	float xdelta;
	float ydat[SAMPLING_NUM_MAX];

	float xmin;
	float xmax;

private:

};


#endif // __MEASUREDATADATA_H__
