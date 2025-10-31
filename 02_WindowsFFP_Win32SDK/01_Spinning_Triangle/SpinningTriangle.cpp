/************************************************************
 *                                                          *
 *                                                          *
 * Description :=>      This program diplays the Blue       *
 *                      Screen.                             *
 *                      It also include   startup template  *
 *                      -   Blank Window                    *
 *                      -   Centering Window                *
 *                      -   FullScreen                      *
 *                      -   File I/O                        *
 *                      -   Icon                            *
 *                      -   Game Loop                       *
 * ********************************************************/


/************************************************************
 *  Header Files    :=>                                     *
 *                      Windows.h   => win32 API            *
 *                      stdio.h     => File I/O             *
 *                      GameLoop.h  => Icon                 *
 *                      gl/GL.h     => OpenGL API           *
 * **********************************************************/
#include<Windows.h>
#include<stdio.h>
#include"SpinningTriangle.h"
#include<gl/GL.h>
//added Graphic Libarry Utility
#include<gl/GLU.h>

/************************************************************
 * MACRO        :=>                                         *
 *                  WIN_WIDTH_SAT   => default screen       *
 *                                      width               *
 *                  WIN_HEIGHT_SAT  =>  default screen      *
 *                                      height              *
 *                                                          *
 * Note         :=> This resolution supports all kind of    *
 *                  monitor.                                *
 * **********************************************************/
#define WIN_WIDTH_SAT 800
#define WIN_HEIGHT_SAT 600


/***********************************************************
 * Library      :=>                                         *
 *                  Add opengl32.lib by using               *
 *                  #pragma comment()                       *
 *                  or                                      *
 *                  Add this library while linking using    *
 *                  link command                            *         
 * ********************************************************/
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")



/***********************************************************
 * Global Variables                                         *
 * *********************************************************/
DWORD dwstyle_SAT;                                          // Window Style
WINDOWPLACEMENT wpPrev_SAT = {                              // Window Previous state
    sizeof(WINDOWPLACEMENT)
};
bool gbFullScreen_SAT = false;                              // toggleFullScreen  
HWND ghwnd_SAT = NULL;                                      // current window handle kept Global
int DesktopWindowCenterX_SAT = 0;                           // Desktop Center of x-axis
int DesktopWindowCenterY_SAT = 0;                           // Desktop center of y-axis
int DesktopXHalf_SAT , DesktopYHalf_SAT;                    // desktop parent window center
int WindowXHalf_SAT = 800/ 2;                               // current window width center
int WindowYHalf_SAT = 600 / 2;                              // current window height center
FILE *gpFile_SAT = NULL;                                    // Log file pointer
bool gbActivatWindow_SAT = false;                           // check the window has focus or not
HDC ghdc = NULL;                                            
HGLRC ghrc = NULL;

/************************************************************
 * Global Function  :=>                                     *
 *                                                          *
 *                      CALLBACK :  WndProc                 *             
 * *********************************************************/
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/************************************************************************
* Entry Point function                      							*
*									                                    *
*	Name		:	WinMain					                            *
*	Parameter	:	HINSTANCE => current Window Instance                * 
*				    HINSTANCE => Parent Window Instance	                *			
*				    LPSTR	  => Command Line Arguments	                *			
*				    iCmdShow  => Window Display		                    *				
*	Return Type :	int 					                            *					
*************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

    
    void Initialize(void);
    void Display(void);


    WNDCLASSEX wndclass;                                                   // Structure of window features     
    HWND hwnd;                                                             // Unique Id store, window handle
    MSG msg;                                                               // Structure for message 
    TCHAR szAppName[] = TEXT("GameLoopTemplateApplication");               // Class name that Os will register     
    bool bDone_SAT = false;

    // Opening the file to store the log
    if(fopen_s(&gpFile_SAT, "SAT_log.txt", "w") != 0){
        
        MessageBox(NULL, TEXT("Cannot Open the File"), TEXT("Message"), MB_OK | MB_ICONERROR);
        exit(0);
    }
    else{
        fprintf(gpFile_SAT,"Log file successfully created, Program started\n");
    }

    // Calculation for Centering the Window
    DesktopXHalf_SAT = GetSystemMetrics(SM_CXSCREEN) / 2;
    DesktopYHalf_SAT = GetSystemMetrics(SM_CYSCREEN) / 2;
    DesktopWindowCenterX_SAT = DesktopXHalf_SAT - WindowXHalf_SAT;
    DesktopWindowCenterY_SAT = DesktopYHalf_SAT - WindowYHalf_SAT;

    wndclass.cbSize = sizeof(WNDCLASSEX);                                   // Size of Structure WNDCLASSEX                                                               
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;                    // Vertical & Hosrizontal & flag that tell DC should not swap
    wndclass.cbClsExtra = 0;                                                // Extra information about class
    wndclass.cbWndExtra = 0;                                                // Extra Information about window
    wndclass.lpfnWndProc = WndProc;                                         // Register Callback
    wndclass.hInstance = hInstance;                                         // Current Window Instance
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON_SAT));      // Window Customize Icon
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                         // Load default Cursor
    wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);          // Client Area Background
    wndclass.lpszClassName = szAppName;                                     // Class Name
    wndclass.lpszMenuName = NULL;                                           // Default Menu
    wndclass.hIconSm = LoadCursor(hInstance, MAKEINTRESOURCE(MYICON_SAT));  // Task bar Icon

    // Register the Window to the OS
    RegisterClassEx(&wndclass);                                             

    /********************************************************
	*							                            *
	*	Create the Registered Window according to the	    *
	*	parameters. It returns a Unique Id and store in     *
	*	"hwnd" which helps to access window elements	    *
	*							                            *
	*********************************************************/
    hwnd = CreateWindowEx( WS_EX_APPWINDOW,                                     // The screen should be above of taskbar
        szAppName,                                                              // Class Name
        TEXT("Double Buffer Triangle: Shruti Tiwarkar"),                            // Tittle of Wiondow 
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,   // Style Macro
        // Centering the Window             
        DesktopWindowCenterX_SAT,                                               // x - cordinates
        DesktopWindowCenterY_SAT,                                               // y - cordinates
        800,                                                                    // width
        600,                                                                    // height
        //0,    
        //0,
        //WIN_WIDTH_SAT,
        //WIN_HEIGHT_SAT,
        NULL,                                                                   // Parent Window
        NULL,                                                                   // Menu Handler
        hInstance,                                                              // Current Window Instance
        NULL);

    ghwnd_SAT = hwnd;                                                           // assign global handle with current wiondow handle

    Initialize();

    ShowWindow(hwnd, iCmdShow);
    // UpdateWindow(hwnd);

    SetForegroundWindow(hwnd);                                                  // Set the current window with the top most window
    SetFocus(hwnd);                                                             // give that window Focus
    /*
    while(GetMessage(&msg, NULL, 0,0)){

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    */

   /*****************************************************************
	*	Game Loop                           						*
	*								                                *
	*	Description :=>     This loop does not get stuck if no      *
                            no message is received                  *
	*								                                *
	*****************************************************************/
    while(bDone_SAT == false) {                                                         
        /****************************************************************
	    *	PeekMessage()                           					*
    	*                               								*
    	*	Parameters : msg  => Addeess of structure MSG       		*
	    *		         NULL => Current and its Child Window       	*
	    *		         0,0  => Range                  				*
	    *				 PM_REMOVE => Remove the values in Os Message   *
        *                               Gola                            *
        *  Return Value : true                                          *
        *                   Message id received                         *
        *                 false                                         *
        *                    If No Message is received                  *
	    *****************************************************************/
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            // checking message is WM_QUIT
            if(msg.message == WM_QUIT){
                bDone_SAT = true;
            }
            else{
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else{
            if(gbActivatWindow_SAT == true){
                //code remaining
                Display();
            }
        }
    }

    
    return((int)msg.wParam);
}

/************************************************************************
* CallBack Function                         							*							
*									                                    *
*	Name		:	WndProc					                            *							
*	Parameters	:	HWND => Handle of Window(unsigned int)	            *          
*				    UNIT	=> Message(unsigned int)	                *
*				    WPARAM	=> Word Parameter		                    *
*				    LPARAM  => Long Parameter		                    *
*	Return Type :	LRESULT	CALLBACK				                    *
*************************************************************************/

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    
    //Local Functions
    void ToggleFullScreen(void);
    void Resize(int, int);
    void UnInitialize(void);
    //void Display(void);

    switch(iMsg){

        // If focus is gain
        case WM_SETFOCUS:
            gbActivatWindow_SAT = true;
            break;

        // If focus is fetch
        case WM_KILLFOCUS:
            gbActivatWindow_SAT = false;
            break;
        
        case WM_ERASEBKGND:
            return 0;
        
        // case WM_PAINT:
        //     Display();
        //     break;

        // if the window size changes
        case WM_SIZE:
            Resize( (LOWORD(lParam)), (HIWORD(lParam)) );
            break;

        // If ESCAPE and 'F' pressed
        case WM_KEYDOWN:
                
                switch(wParam){
                    case VK_ESCAPE:
                        DestroyWindow(hwnd);
                        break;
                    case 0x46:
                    case 0x66:
                        ToggleFullScreen();
                        break;
                    default:
                        break;
                }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            UnInitialize();
            PostQuitMessage(0);
            break;
    }

    return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

/************************************************************************
* Function                                   							*							
*									                                    *
*	Name		:	ToggleFullScreen		                            *							
*	Parameters	:	None                                                *
*   Description :   This function changes the window normal to          *
*                   FullScreen and FullScreen to normal                 *
*	Return Type :	void            				                    *
*************************************************************************/
void ToggleFullScreen(void) {

    // LOCAL Variable Declaration
    MONITORINFO mi_SAT = {
        sizeof(MONITORINFO)
    };

    if(gbFullScreen_SAT == false){

        dwstyle_SAT = GetWindowLong(ghwnd_SAT, GWL_STYLE);

        if(GetWindowPlacement(ghwnd_SAT, &wpPrev_SAT) && ( GetMonitorInfo(MonitorFromWindow(ghwnd_SAT, MONITORINFOF_PRIMARY), &mi_SAT) ) ) {

            SetWindowLong(ghwnd_SAT, GWL_STYLE, (dwstyle_SAT & ~WS_OVERLAPPEDWINDOW));
            SetWindowPos(ghwnd_SAT,
                HWND_TOP,
                mi_SAT.rcMonitor.left,
                mi_SAT.rcMonitor.top,
                mi_SAT.rcMonitor.right - mi_SAT.rcMonitor.left,
                mi_SAT.rcMonitor.bottom - mi_SAT.rcMonitor.top,
                SWP_NOZORDER | SWP_FRAMECHANGED
            );
        }
        
        ShowCursor(FALSE);
        gbFullScreen_SAT = true; 
    }
    else {

        SetWindowLong(ghwnd_SAT, GWL_STYLE, dwstyle_SAT | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd_SAT, &wpPrev_SAT);
        SetWindowPos(ghwnd_SAT,
            HWND_TOP,
            0,
            0,
            0,
            0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED
        );

        ShowCursor(TRUE);
        gbFullScreen_SAT = false;
    }
}

void Initialize(void) {

    //function Declaration
    void Resize(int, int);

    //Variable Declaration
    PIXELFORMATDESCRIPTOR pfd;
    int iPixelFormatIndex;

    // Code

    ghdc = GetDC(ghwnd_SAT);

    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    // Double Buffer change
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;

    iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
    if(iPixelFormatIndex == 0){

        fprintf(gpFile_SAT,"ChoosePixelFormat() function failed\n");
        DestroyWindow(ghwnd_SAT);
    }

    if(SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE){

        fprintf(gpFile_SAT,"SetPixelFormat() function failed\n");
        DestroyWindow(ghwnd_SAT);    
    }

    ghrc = wglCreateContext(ghdc);
    if(ghdc == NULL){

        fprintf(gpFile_SAT,"wglCreateContext() function failed\n");
        DestroyWindow(ghwnd_SAT);
    }

    if(wglMakeCurrent(ghdc, ghrc) == FALSE){

        fprintf(gpFile_SAT,"wglMakeCurrent() function failed\n");
        DestroyWindow(ghwnd_SAT);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Resize(WIN_WIDTH_SAT, WIN_HEIGHT_SAT);

}

void Resize(int width, int height){

    if(height == 0){
        height = 1;
    }

    glViewport(0,0,(GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
}

void Display(void) {

    static GLfloat angle = 0.0f;
    //code
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //added
    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    
    glBegin(GL_TRIANGLES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);


    glEnd();
    angle = angle + 0.1f;

    // Removed because of double buffer
    //glFlush();

    SwapBuffers(ghdc);
}

void UnInitialize() {

    if(gbFullScreen_SAT == true){

        dwstyle_SAT = GetWindowLong(ghwnd_SAT, GWL_STYLE);

        SetWindowLong(ghwnd_SAT, GWL_STYLE, dwstyle_SAT | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd_SAT, &wpPrev_SAT);
        SetWindowPos(ghwnd_SAT,
            HWND_TOP,
            0,
            0,
            0,
            0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED
        );

        ShowCursor(TRUE);
        
    }

    if(wglGetCurrentContext() == ghrc){

        wglMakeCurrent(NULL, NULL);
    }

    if(ghrc){

        wglDeleteContext(ghrc);
        ghrc = NULL;
    }

    if(ghdc){

        ReleaseDC(ghwnd_SAT, ghdc);
        ghdc = NULL;
    }

    if(gpFile_SAT){

        fprintf(gpFile_SAT,"Log file closed successfully\n");
        fclose(gpFile_SAT);
        gpFile_SAT = NULL;
    }

}

