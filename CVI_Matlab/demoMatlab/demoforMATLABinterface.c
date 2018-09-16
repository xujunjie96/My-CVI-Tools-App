/*---------------------------------------------------------------------------*/
/*                                                                           */
/* FILE:    demoforMATLABinterface.c                                         */
/*                                                                           */
/* PURPOSE: This sample illustrates how to communicate with MATLAB Automation*/
/*          Server from CVI and execute MATLAB commands, M scripts, and      */
/*          transfer data to and from MATLAB and CVI.                        */
/*                                                                           */
/* NOTES:   This sample program requires the MATLAB Automation Server Type   */
/*          Library, and the Wizard generated Automation Instrument Driver   */
/*          Matlab.fp, which is included in the project. You should have an  */
/*          Automation enabled version of MATLAB (5.0 or greater) properly   */
/*          installed and the MATLAB type library properly registered in     */
/*          your computer. The matlabUtil.c file should be in the project.   */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Include files                                                             */
/*---------------------------------------------------------------------------*/
#include "MLApp.h"
#include <cviauto.h>
#include "matlabUtil.h"
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include <cvirte.h>     /* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>
#include "demoforMATLABinterface.h"

/*---------------------------------------------------------------------------*/
/* Module-globals                                                            */
/*---------------------------------------------------------------------------*/
static int          pnl;
static CAObjHandle  hMatlab = 0;

/*---------------------------------------------------------------------------*/
/* This function reports the Automation error associated with the code.      */
/* errCode is the Automation error code.                                     */
/* errMesg is the error message that MATLAB returns in some functions. You   */
/* can pass NULL for this.                                                   */
/*---------------------------------------------------------------------------*/
void ErrFunc(HRESULT errCode, char *errMesg)
{
    char        errStr[200];
    
    CA_GetAutomationErrorString (errCode, errStr, 200);
    MessagePopup ("ERROR MESSAGE", errStr);
    if (errMesg)
    {
        MessagePopup ("MATLAB ERROR MESSAGE", errMesg);
        CA_FreeMemory (errMesg);
    }
}

/*---------------------------------------------------------------------------*/
/* This function closes MATLAB if it is running.                             */
/* hMatlabPtr is a pointer to the handle of the MATLAB Application Object.   */
/*---------------------------------------------------------------------------*/
int CloseMatlab(CAObjHandle *hMatlabPtr)
{
    HRESULT     stat    =   0;
    
    /* Exit MATLAB if it is running */
    if (*hMatlabPtr && hMatlabPtr)
    {
        stat = MLApp_DIMLAppQuit (*hMatlabPtr, NULL);
        if (stat < 0)
        {
            ErrFunc(stat, NULL);
            return (int)stat;
        }
        else
        {
            stat = CA_DiscardObjHandle (*hMatlabPtr);
            if (stat < 0)
            {
                ErrFunc(stat, NULL);
                return (int)stat;
            }
            *hMatlabPtr=0;
            return SUCCESS;
        }
    }
    else
        return ERROR_WRONG_HANDLE;
}

/*---------------------------------------------------------------------------*/
/* This function is used to Dim the appropriate controls.                    */
/* state=0 enables the controls, state=1 dims (disables) them.               */
/*---------------------------------------------------------------------------*/
void UIRDimming(int state)
{
    SetCtrlAttribute (pnl, PNL_GETSTR, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_DIFFEQN, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_INVMATRIX, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_FFT, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_SENDSTR, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_RUN, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_EXIT, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_MAXMIN, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_CMD, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_RUNM, ATTR_DIMMED, state);
    SetCtrlAttribute (pnl, PNL_LAUNCH, ATTR_DIMMED, !state);
}

/*---------------------------------------------------------------------------*/
/* This is the application's entry-point.                                    */
/*---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)   /* Needed if linking in external compiler; harmless otherwise */
        return -1;  /* out of memory */
    CA_InitActiveXThreadStyleForCurrentThread (0, COINIT_APARTMENTTHREADED);
    if ((pnl = LoadPanel (0, "demoforMATLABinterface.uir", PNL)) < 0)
        return -1;
    UIRDimming(1);
    DisplayPanel (pnl);
    RunUserInterface ();
    DiscardPanel (pnl);
    CloseCVIRTE ();
    
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the LAUNCH MATLAB button.               */
/*---------------------------------------------------------------------------*/
int CVICALLBACK launchCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    HRESULT stat;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Instantiate a MATLAB application object & maximize the window */
            stat = MLApp_NewDIMLApp (NULL, 1, LOCALE_NEUTRAL, 0, &hMatlab);
            if (stat < 0)
			{
				ErrFunc(stat, NULL);
				goto Error;
			}
            UIRDimming(0);
            MinMaxMatlab(hMatlab, 1);
            break;
        }

Error:
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the EXIT MATLAB button.                 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK exitCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int     result  =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Exit MATLAB if it is running */
            result = CloseMatlab(&hMatlab);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in closing MATLAB");
                return 0;
            }
            UIRDimming(1);
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the QUIT button.                        */
/*---------------------------------------------------------------------------*/
int CVICALLBACK quitCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int     result  =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Quit this application */
            if (hMatlab)
            {
                result = CloseMatlab(&hMatlab);   
                if (result != SUCCESS)
                {
                    MessagePopup ("ERROR", "Error in closing MATLAB");
                }
            }
            QuitUserInterface (0);
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the RUN button.                         */
/*---------------------------------------------------------------------------*/
int CVICALLBACK runCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char        command[300];
    int         result          =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            GetCtrlVal (pnl, PNL_CMD, command);
/*---------------------------------------------------------------------------*/     
/*  Execute the MATLAB command the User typed in the string box.             */
/*  Note that the MATLAB command will not be echoed on the MATLAB window.    */
/*  Commands that result in responses on the MATLAB window when executed     */
/*  from within MATLAB will not do the same when executed via Automation.    */
/*---------------------------------------------------------------------------*/             
            result = RunMatlabCommand(hMatlab,command);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the vertical switch.                    */
/*---------------------------------------------------------------------------*/
int CVICALLBACK maxMinCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int         val;
    HRESULT     stat;
    int         result  =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            GetCtrlVal (pnl, PNL_MAXMIN, &val);
            stat = MinMaxMatlab( hMatlab, val);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in maximizing and minimizing MATLAB WIndow");
                return 0;
            }
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the RUN M SCRIPT button.                */
/*---------------------------------------------------------------------------*/
int CVICALLBACK runMCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int         status;
    char        path[300];
    int         result      =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Display the FileSelectPopupEx to get a .m file */
            status = FileSelectPopupEx ("", "*.m", "*.m", "Select .m File to Run",
										VAL_SELECT_BUTTON, 0, 1, path);
            if (!status)
            {
                MessagePopup ("ERROR MESSAGE", "No File was Selected");
                return 0;
            }
            /* Run the M-script */
            result = RunMatlabScript(hMatlab, path);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in running MATLAB Script");
                return 0;
            }
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the FFT button.                         */
/*---------------------------------------------------------------------------*/
int CVICALLBACK fftCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    double      time[256] = { 0 };
    double      freq[128];
    double      vect_r[256],vect_i[256];
    double      *vecReal    =   NULL;
    double      *vecImag    =   NULL;
    int         i           =   0;
    size_t	    dim1;
    size_t  	dim2;
    int         result      =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Create the signal, send it to MATLAB and plot it in CVI */
            for(i=0;i<256;++i)
            {
                time[i]=0.001*i;
                vect_r[i]=(0.2*rand()/RAND_MAX)+sin(2*3.1415*50.0*time[i])
                                    +sin(2*3.1415*120.0*time[i]);
                vect_i[i]=0.0;
            }
            result = SendMatrix(hMatlab,"cVector",vect_r,vect_i,256,1);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending matrix to MATLAB");
                return 0;
            }
            YGraphPopup ("Signal", vect_r, 256, VAL_DOUBLE);
            
            /* Take the FFT of the signal */
            result = RunMatlabCommand(hMatlab, "mVector=fft(cVector,256);");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            /* Find the Power Spectral Density of the signal */
            result = RunMatlabCommand(hMatlab, "psdVector=mVector.*conj(mVector)/256;");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            result = GetMatrix(hMatlab, "psdVector", &vecReal, &vecImag, &dim1, &dim2);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in getting matrix from MATLAB");
                return 0;
            }
            
            /* Create a frequency axis for plotting the  Power Spectral Density */
            if (vecReal == NULL)
            {
                MessagePopup ("ERROR", "NULL pointer returned from MATLAB");
                return 0;
            }
            for(i=0;i<128;++i)
                freq[i]=i*1000.0/256;
            /* Plot the Power Spectral Density */
            XYGraphPopup ("Power Spectral Density", freq, vecReal, 128,
                          VAL_DOUBLE, VAL_DOUBLE);
            if (vecReal != NULL)    CA_FreeMemory(vecReal);
            if (vecImag != NULL)    CA_FreeMemory(vecImag);
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the INVERT button.                      */
/*---------------------------------------------------------------------------*/
int CVICALLBACK invertCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    /* The matrix to invert */
    double      matrix_r[2][2]    =   {{1,2},{3,4}};
    double      matrix_i[2][2]    =   {{0,0},{0,0}};
    double      *matrixReal     =   NULL;
    double      *matrixImag     =   NULL;
    size_t      dim1            =   2;
    size_t      dim2            =   2;
    size_t      i               =   0;
    size_t      j               =   0;
    size_t      k               =   0;
    int         result          =   0;
    char        message[300];
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Send the matrix to MATLAB */
            result = SendMatrix(hMatlab,"cMatrix",(double*)matrix_r,(double*)matrix_i,2,2);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending Matrix to MATLAB");
                return 0;
            }
            /* Invert the matrix in MATLAB */
            result = RunMatlabCommand(hMatlab,"mMatrix=inv(cMatrix);");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            /* Get the inverted matrix */
            result = GetMatrix(hMatlab,"mMatrix",&matrixReal,&matrixImag,&dim1,&dim2);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in Getting Matrix from MATLAB");
                return 0;
            }           
            /* Display the matrices */
            Fmt (message, "%s<matrix:\n");
            for(i=0; i<dim1; ++i)
            {   
                for(j=0;j<dim2;++j)
                    Fmt (message, "%s[a]<%f   ", matrix_r[i][j]);
                Fmt (message, "%s[a]<\n");
            }
            MessagePopup ("MATRIX", message);
            if(matrixReal == NULL)
            {
                MessagePopup ("ERROR", "NULL pointer returned from MATLAB");
                return 0;
            }
            Fmt (message, "%s<Inverse(matrix):\n");         
            for(i=0; i<dim1; ++i)
            {   
                for(j=0;j<dim2;++j)
                {
                    Fmt(message,"%s[a]<%f   ",*(matrixReal+k));
                    k++;
                }
                Fmt(message,"%s[a]<\n");
            }
            MessagePopup ("INVERTED MATRIX", message);          
            if (matrixReal != NULL) CA_FreeMemory(matrixReal);
            if (matrixImag != NULL) CA_FreeMemory(matrixImag);
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the SOLVE DIFF EQUATION button.         */
/*---------------------------------------------------------------------------*/
int CVICALLBACK solveDiffEqnCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    double          *matrixReal                 =   NULL;
    double          *matrixImag                 =   NULL;
    size_t          dim1                        =   0;
    size_t          dim2                        =   0;
    char            dirName[MAX_PATHNAME_LEN];
    int             result                      =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Get the directory of this project */
            GetProjectDir (dirName);
            result = SendString(hMatlab,"examplePath",dirName);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending Path to MATLAB");
                return 0;
            }
            /* Add the current project's path to MATLAB's search path */
            result = RunMatlabCommand(hMatlab,"path(path,examplePath)");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in Appending Path in MATLAB");
                return 0;
            }
            /* Create the neccessary variables in MATLAB */
            result = RunMatlabCommand(hMatlab,"y=[1,1,1];");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            result = RunMatlabCommand(hMatlab,"n=30/0.05;");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            /* Create the time array T */
            result = RunMatlabCommand(hMatlab,"T=linspace(0,30,n);");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            
/* [T,Y] = ODE23('F',TSPAN,Y0) with TSPAN = [T0 TFINAL] integrates the       */
/* system of differential equations y' = F(t,y) from time T0 to TFINAL with  */
/* initial conditions Y0.  'F' is a string containing the name of an ODE     */
/* file.  Function F(T,Y) must return a column vector.  Each row in          */
/* solution array Y corresponds to a time returned in column vector T.  To   */
/* obtain solutions at specific times T0, T1, ..., TFINAL (all increasing    */
/* or all decreasing), use TSPAN = [T0 T1 ... TFINAL].                       */

            /* Solve the Lorenz Attractor Equations */
            result = RunMatlabCommand(hMatlab,"[t,Y]=ode23('LorenzEqs',T,y);");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            /* Get the solution from MATLAB */
            result = GetMatrix(hMatlab, "Y", &matrixReal, &matrixImag, &dim1, &dim2);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in Getting Matrix from MATLAB");
                return 0;
            }
            if (matrixReal == NULL)
            {
                MessagePopup ("ERROR", "NULL pointer returned from MATLAB");
                return 0;
            }
            YGraphPopup ("Lorenz Equations", matrixReal, dim1, VAL_DOUBLE);
            if (matrixReal != NULL) CA_FreeMemory(matrixReal);
            if (matrixImag != NULL) CA_FreeMemory(matrixImag);
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the SEND STRING button.                 */
/*---------------------------------------------------------------------------*/
int CVICALLBACK sendStrCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char                    CVIString[300];
    int                     result          =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            Fmt(CVIString,"%s<Hello MATLAB");
            /* Send the "Hello MATLAB" string to MATLAB and store it as the matStr variable */
            result = SendString(hMatlab, "matStr", CVIString);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending String to MATLAB");
            }
            break;
        }
    return 0;
}

/*---------------------------------------------------------------------------*/
/* This is the callback function for the GET STRING button.                  */
/*---------------------------------------------------------------------------*/
int CVICALLBACK getStrCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char        *cStr       =   NULL;
    int         result      =   0;
    
    switch (event)
        {
        case EVENT_COMMIT:
            /* Get the matStr variable from MATLAB and store it as cStr in CVI */
            result = GetString(hMatlab,"matStr",&cStr);
            if (result != SUCCESS)
            {
             MessagePopup ("ERROR", "Error in getting String from MATLAB");
             return 0;
            }
            if(cStr == NULL)
            {
                MessagePopup ("ERROR", "NULL pointer returned from MATLAB");
                return 0;
            }
            MessagePopup ("MATLAB STRING: matStr", cStr);
            free(cStr);
            break;
        }
    return 0;
}
