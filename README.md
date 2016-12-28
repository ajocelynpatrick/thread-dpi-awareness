# thread-dpi-awareness
Find DPI of monitors reported by the system (windows 10) for various DPI awareness levels

### System : 
**OS**: Windows 10 Aniversary update - SetThreadDpiAwarenessContext() function was introduced in windows 10 aniversary update.

**Windows SDK version** : 10.0.14393.0

**Visual Studio**: 2017 RC

### A note on SetThreadDpiAwarenessContext()
SetThreadDpiAwarenessContext() function was introduced in windows 10 aniversary update. This allows a thread to change its DPI awareness level at will, i.e. for some time duration a thread can set itself as DPI unaware, while at other times it cadn set itself as per monitor aware, or system aware. This allows the thread to create windows which have different DPI awareness levels. Using this we can build an  application in which scaling of some UI elements is handled by windows, while scaling for others is handled by the application itself.
A related function introduced in **windows 8.1** called **SetProcessDpiAwareness()**. This allows to set DPI awareness of a process to either per monitor, system, or unaware. This is similar to SetThreadDpiAwarenessContext(), however unlike SetThreadDpiAwarenessContext() a process cannot change its DPI awareness level at any time. Thus all the windows created by the process have asme DPI awareness level. A thing to note about SetThreadDpiAwarenessContext() is that the **windowProc** can be same for all the windows created by it, irrespective of their DPI awareness. Dpeending on the window through which call to windowProc is made, OS will automatically change the effective DPI awareness while windowProc is executing.

### Getting monitor DPI for connected monitors

```
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


void enumrate_monitors() {
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc/*this handler will be called for every monitor on the system*/,NULL);
}
```

**Raw DPI** -> Actual DPI of the monitor

**Effective DPI** -> DPI which windows OS wants us to consider for drawing objects on monitor

As an example consider a monitor which has raw DPI of 96, now if user selects 150% display scaling from control panel, the raw DPI reported by windows will still be 96, but effective DPI will be 144. If we assume the display is of 144 DPI, and try to render a erctangle of same size as on 96 DPI => if on 96 DPI rectangle was 10 pixels wide, it should be 15 pixels wide at 144 DPI. Now, since RAW DPI is still the same, the drawn rectangle gets drawn with 150% size of earlier.

### Per monitor DPI awareness -
```
DPI_AWARENESS_CONTEXT old_dpi_ctx = SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
```
