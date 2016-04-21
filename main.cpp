/* 
*		file name :main.cpp
*		descriptioin: provide a windows with a visible view of square in  ConnectSimulation class.
*					  A button is in the windows which can restart the procedure of simulation.
*					
*/
#include <windows.h>
#include <time.h>		
#include "WQUPC.h"
#define N 20				//the count of little square column and row 
#define TIMES_PER_SECOND 10  //the frequency of create a new opened point (Greater than 0 and smaller than 1000)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("UnionFindTestApp") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"),
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     hwnd = CreateWindow (szAppName,                  // window class name
                          TEXT ("Union Find Test App"), // window caption
                          WS_OVERLAPPEDWINDOW,        // window style
                          GetSystemMetrics(SM_CXSCREEN)/2-260,              // initial x position
                          GetSystemMetrics(SM_CYSCREEN)/2-350,              // initial y position
                          530,							// initial x size
                          600,							// initial y size
                          NULL,                       // parent window handle
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL) ;                     // creation parameters

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
	 HRGN   hDestRgn, hSrcRgn1, hSrcRgn2;
     PAINTSTRUCT ps ;
     RECT        gridRect,littleRect ;
     static HWND  hRestartButton;
	 static int cxClient,cyClient,i,j,cxGrid,cyGrid,cxOrigin,cyOrigin;
	 static ConnectSimulation *sim=new ConnectSimulation(N); 
	 static POINT p;
	 static HBRUSH BlueBrush;
	 static bool wantToDrawBackground = true;

     switch (message)
     {
     case WM_CREATE:

		 //define a button for restart simulation
          hRestartButton = (HWND)CreateWindow (TEXT("button"), 
                          TEXT ("重新开始"), WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,  
                          0,0,0,0,
                          hwnd, (HMENU)1,  (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),  NULL) ; 
		  
		  //设置定时器 create a timer
		  SetTimer(hwnd,1,1000/TIMES_PER_SECOND,NULL);
		  
		  //设置grid显示区域 
		  cxOrigin=cyOrigin=5;
		  cxGrid = 500;
		  cyGrid = 500;
		  
		  //生成一个画刷
		  BlueBrush = CreateSolidBrush(RGB(0,0,255));

		  //为随机时间生成种子
		  srand(time(NULL));

          return 0 ;
	 case WM_SIZE:
		   cxClient = LOWORD (lParam) ;
           cyClient = HIWORD (lParam) ;
			
		   //adjust the position of button to the right-bottom corner
		   MoveWindow(hRestartButton,cxClient-130,cyClient-40,100,25,TRUE);

		 return 0;

     case WM_PAINT:

          hdc = BeginPaint (hwnd, &ps) ;

		  //画一个背景矩形
		  gridRect.left = cxOrigin;
		  gridRect.top = cyOrigin;
		  gridRect.right = cxOrigin+cxGrid;
		  gridRect.bottom = cyOrigin+cyGrid;
		  FillRect(hdc,&gridRect,(HBRUSH)GetStockObject(BLACK_BRUSH));

		   //如果方格中的点为开放则画【白色】正方形，如果还是连接topPoint的画则画【蓝色】
		   for(i=0;i<N;i++)
			  for(j=0;j<N;j++)
			  {
				p.x = i; 
				p.y = j;
				littleRect.left =cxOrigin+ i* cxGrid/N +1;
				littleRect.right =cxOrigin+ (i+1)* cxGrid/N -1;
				littleRect.top =cyOrigin+ j * cyGrid/N+1;
				littleRect.bottom = cyOrigin+(j+1) * cyGrid/N-1;

				if(sim->isOpen(p))
				{
					if(sim->IsCTConective(p)){
						FillRect(hdc,&littleRect,BlueBrush);
					}
					else{
						FillRect(hdc,&littleRect,(HBRUSH)GetStockObject(WHITE_BRUSH));
					}
				}
			  }

          EndPaint (hwnd, &ps) ;
          return 0 ;
	 case WM_TIMER:
		
		 //如果底部和顶部没有连接则接续 随机生成点来 开放新的点
		 if(!sim->IsBTConective())
		 {
			p.x = rand()%N;
			p.y = rand()%N;
			sim->setOpen(p);

			//预备三个区域
			hDestRgn = CreateRectRgn (0,0,0,0) ;
			hSrcRgn1 = CreateRectRgn (0, 0, 0, 0) ;
			hSrcRgn2 = CreateRectRgn (0, 0, 0, 0) ;

			//如果方格中的点为开放则创建区域
		   for(i=0;i<N;i++)
			  for(j=0;j<N;j++)
			  {
				p.x = i; 
				p.y = j;
				littleRect.left =cxOrigin+ i* cxGrid/N +1;
				littleRect.right =cxOrigin+ (i+1)* cxGrid/N -1;
				littleRect.top =cyOrigin+ j * cyGrid/N+1;
				littleRect.bottom = cyOrigin+(j+1) * cyGrid/N-1;

				if(sim->isOpen(p))
				{
					hSrcRgn2 = CreateRectRgn (littleRect.left, littleRect.top, littleRect.right, littleRect.bottom) ;
					CombineRgn (hDestRgn, hSrcRgn1, hSrcRgn2, RGN_OR) ;
					DeleteObject(hSrcRgn2);

					CombineRgn (hSrcRgn1, hDestRgn, hSrcRgn2, RGN_COPY) ;
				}

			  }
			// 定义自定义的重画区域来重画 以减小闪烁现象
			InvalidateRgn (hwnd, hDestRgn, TRUE) ;

			DeleteObject(hDestRgn);
			DeleteObject(hSrcRgn1);

		 }



		 return 0;
	 case WM_COMMAND:
		
		 //restart the simulation
		 if((HWND)lParam==hRestartButton)
		 {
			sim->reinitialize();
			InvalidateRect(hwnd,NULL,TRUE);
		 }

		 return 0;

     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
