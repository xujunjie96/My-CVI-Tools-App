/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PNL                             1
#define  PNL_GETSTR                      2       /* callback function: getStrCB */
#define  PNL_DIFFEQN                     3       /* callback function: solveDiffEqnCB */
#define  PNL_INVMATRIX                   4       /* callback function: invertCB */
#define  PNL_FFT                         5       /* callback function: fftCB */
#define  PNL_SENDSTR                     6       /* callback function: sendStrCB */
#define  PNL_EXIT                        7       /* callback function: exitCB */
#define  PNL_LAUNCH                      8       /* callback function: launchCB */
#define  PNL_RUN                         9       /* callback function: runCB */
#define  PNL_QUIT                        10      /* callback function: quitCB */
#define  PNL_MAXMIN                      11      /* callback function: maxMinCB */
#define  PNL_CMD                         12      /* callback function: runCB */
#define  PNL_RUNM                        13      /* callback function: runMCB */
#define  PNL_TEXTMSG_2                   14
#define  PNL_TEXTMSG                     15
#define  PNL_TEXTMSG_3                   16
#define  PNL_TEXTMSG_4                   17
#define  PNL_TEXTMSG_5                   18
#define  PNL_TEXTMSG_6                   19
#define  PNL_TEXTMSG_7                   20
#define  PNL_TEXTMSG_8                   21


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK exitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK fftCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getStrCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK invertCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK launchCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK maxMinCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quitCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK runCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK runMCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sendStrCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK solveDiffEqnCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
