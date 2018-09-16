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

#define  PANEL                            1       /* callback function: panel */
#define  PANEL_SEC                        2       /* control type: numeric, callback function: (none) */
#define  PANEL_MIN                        3       /* control type: numeric, callback function: (none) */
#define  PANEL_HOUR                       4       /* control type: numeric, callback function: (none) */
#define  PANEL_DAY                        5       /* control type: numeric, callback function: (none) */
#define  PANEL_Month                      6       /* control type: numeric, callback function: (none) */
#define  PANEL_YEAR                       7       /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON              8       /* control type: command, callback function: OkCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OkCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panel(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
