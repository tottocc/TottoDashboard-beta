
#ifndef __MEASUREDATA_H__
#define __MEASUREDATA_H__

#include <stdlib.h>
#include <wx/string.h> // instead of <string>
using namespace std;

#include <wx/time.h>



//#define SAMPLING_NUM_MAX 8640000
#define SAMPLING_NUM_MAX 100000
//#define SAMPLING_NUM_MAX 1000


enum unit_t {sec, mA};

class MeasureData
{
public:
	MeasureData() {
		num = 0;
		xdelta = 0.01;   // 10 msec interval
//		xdelta = 1;   // 1 sec interval
		for (int x = 0; x < SAMPLING_NUM_MAX; x++) {
//			ydat[x] = (rand() % 11) * 0.1;
			ydat[x] =(x % 1000) / 100 * 100;
			num++;
		}
		xmin = 0;
		xmax = xdelta * (num-1);

		xname = "Time";
		yname = "Current";
		xunit = sec;
		yunit = mA;

		utime = wxGetUTCTimeUSec();
	};
	~MeasureData() {
	};

	int num;
	float xdelta;
	float ydat[SAMPLING_NUM_MAX];

	float xmin;
	float xmax;

	string xname;
	string yname;
	unit_t xunit;
	unit_t yunit;

	wxLongLong utime;  // number of microseconds since GMT 00:00:00 Jan 1st 1970. 

private:

};


#endif // __MEASUREDATADATA_H__
