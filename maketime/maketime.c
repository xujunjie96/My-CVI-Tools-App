#include <cvirte.h>		
#include <userint.h>
#include "maketime.h"
#include <stdio.h>
#include <time.h>

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "maketime.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panel (int panel, int event, void *callbackData,
					   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_KEYPRESS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK OkCallback (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int year,month,day,hour,min,sec;
	struct tm tm1;
	time_t testtime; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL_YEAR, &year);
			GetCtrlVal (panel, PANEL_Month, &month);
			GetCtrlVal (panel, PANEL_DAY, &day);
			GetCtrlVal (panel, PANEL_HOUR, &hour);
			GetCtrlVal (panel, PANEL_MIN, &min);
			GetCtrlVal (panel, PANEL_SEC, &sec);		//得到时间值
			
			  tm1.tm_year=year-1900;
			  tm1.tm_mon=month-1;
			  tm1.tm_mday=day;
			  tm1.tm_hour=hour+1;
			  tm1.tm_min=min;
			  tm1.tm_sec=sec;
			testtime=  mktime(&tm1);		  //得到测试时间 
			
			printf("Cur time: tm1=0x%08x\n", testtime);  
     		printf("Local: %s\n", asctime(localtime(&testtime))); 
			break;
	}
	return 0;
}
