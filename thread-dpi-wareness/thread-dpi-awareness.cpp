#include<afxwin.h>
#include "thread-dpi-awareness.h"
#include<ShellScalingApi.h>

CMyApp myApp;

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMainWindow;
	//m_pMainWnd->ShowWindow(m_nCmdShow);
	return FALSE;
}

LRESULT CMainWindow::OnPaint2(WPARAM wParam, LPARAM lParam) 
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	CPaintDC dc(this);
	//TRACE("OnPaint2\n");

	return NULL;
}

BEGIN_MESSAGE_MAP(CMainWindow, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//ON_MESSAGE(0x000f,OnPaint2)
	ON_MESSAGE(WM_DPICHANGED,OnDpiChanged)
END_MESSAGE_MAP()


BOOL CALLBACK MonitorEnumProc(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData)
{
	//this callback function will be called for each monitor in the system
	UINT xdpi, ydpi;
	if (SUCCEEDED( GetDpiForMonitor(hMonitor, MDT_RAW_DPI, &xdpi, &ydpi)))
	{
		TRACE("\n#Monitor raw DPI %u Monitor rect: (left,top)=(%d,%d), (right,bottom)=(%d,%d)#\n", xdpi, lprcMonitor->left, lprcMonitor->top, lprcMonitor->right, lprcMonitor->bottom);
	}
	else {
		TRACE("\n#Error getting monitor DPI#\n");
		
	}
	if (SUCCEEDED(GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi)))
	{
		TRACE("\n#Monitor effective DPI %u Monitor rect: (left,top)=(%d,%d), (right,bottom)=(%d,%d)#\n", xdpi,lprcMonitor->left,lprcMonitor->top,lprcMonitor->right,lprcMonitor->bottom);
	}
	else {
		TRACE("\n#Error getting monitor DPI#\n");
	}
	return TRUE;
}

void enumerate_display_devices()  //enumerate display devices/adapters - when lpDevice parameter passed to EnumDisplayDevices() is NULL, it returns infomation about display adapters.
{
	for (int dev_index = 0;; dev_index++)
	{
		DISPLAY_DEVICE dd;
		dd.cb = sizeof(DISPLAY_DEVICE);
		if (EnumDisplayDevices(NULL,dev_index,&dd, EDD_GET_DEVICE_INTERFACE_NAME) == 0)
		{//no display adapter of the given index
			break;
		}
		else 
		{
			if (dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
			{//display device in desktop
				TRACE("\n#DispDevDesktop %ls#\n",dd.DeviceName);
				for (int monitor_index = 0;; monitor_index++)
				{
					DISPLAY_DEVICE monitor;	
					monitor.cb = sizeof(DISPLAY_DEVICE);
					if (EnumDisplayDevices(dd.DeviceName,monitor_index,&monitor, EDD_GET_DEVICE_INTERFACE_NAME) == 0)
						break;
					else
					{
						TRACE("\n#\tMonitor %ls#\n",monitor.DeviceString);
					}
				}
			}
			else 
			{//display device not in a desktop
				TRACE("\n#Display device not in desktop %ls#\n", dd.DeviceName);
			}
		}
	}

}

void enumrate_monitors() {
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc,NULL);
}

void PrintDPIAwrenessString(DPI_AWARENESS_CONTEXT ctx) 
{
	
	DPI_AWARENESS dpiAwareness = GetAwarenessFromDpiAwarenessContext(ctx);
	switch (dpiAwareness)
	{
	case DPI_AWARENESS_SYSTEM_AWARE:
		TRACE("DPI_AWARENESS_SYSTEM_AWARE");
		break;
	case DPI_AWARENESS_PER_MONITOR_AWARE:
		TRACE("DPI_AWARENESS_PER_MONITOR_AWARE");
		break;
	case DPI_AWARENESS_UNAWARE:
		// intentional fallthrough
		TRACE("DPI_AWARENESS_UNAWARE");
	default:
		TRACE("invalid");
	}
	TRACE("\n");
}

CMainWindow::CMainWindow()
{
	
	DPI_AWARENESS_CONTEXT old_dpi_ctx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
	//DPI_AWARENESS_CONTEXT old_dpi_ctx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

	if (old_dpi_ctx == NULL)
	{
		TRACE("Error setting thread DPI wareness context\n");
	}
	DPI_AWARENESS_CONTEXT ctx = GetThreadDpiAwarenessContext();
	
	TRACE("\n-> old DPI ctx:"); PrintDPIAwrenessString(old_dpi_ctx);
	TRACE("\n-> current DPI ctx:"); PrintDPIAwrenessString(ctx);


	//TRACE("Thread DPI context old:%ls current:%ls\n", str_old,str_curr);

	TRACE("##%d##", DPI_AWARENESS_CONTEXT_UNAWARE);
	DPI_AWARENESS_CONTEXT dpiAwarenessContext = GetThreadDpiAwarenessContext();
	
	
	// Convert DPI awareness to a string
	

	enumerate_display_devices();
	//TRACE("\n\n");
	enumrate_monitors();
	//Create(NULL,_T("Thread dpi awareness"));
}

void CMainWindow::OnPaint()
{
	CPaintDC dc(this);
	//TRACE("OnPaint\n");
}

LRESULT CMainWindow::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	int x_axis_dpi = LOWORD(wParam);
	int y_axis_dpi = HIWORD(wParam);
	
	RECT * pRect;
	pRect = (RECT *)lParam;

	TRACE("WM_DPICHANGED (x_axis_dpi,%d), (y_axis_dpi,%d), rect : (left,top)=(%d,%d) (right,bottom)=(%d,%d) \n",x_axis_dpi,y_axis_dpi,pRect->left,pRect->top,pRect->right, pRect->bottom);
	return NULL;
}
