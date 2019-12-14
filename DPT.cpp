// DPT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>

//WHJ.20191119
//
//DPT.exe HWND YYYYMMDD

#define DPT_VERSION 20191119

int main(int argc, char* argv[])
{
	DWORD nhwnd;
	char* ymd=NULL;

	if(argc < 3){
		printf("VERSION: %d\n",DPT_VERSION);
		printf("INFO   : DPT.EXE HWND YYYYMMDD\n");
		return 0;
	}

	nhwnd = atoi(argv[1]);
//	sscanf(argv[1],"%u",&nhwnd);
//	strtol(argv[1],
	
	ymd = argv[2];


	HWND hwnd = (HWND)nhwnd; // (HWND)0x10356;

	DWORD pid,tid;
	DWORD n;

	SYSTEMTIME st;

	tid = ::GetWindowThreadProcessId(hwnd,&pid);
	HANDLE hp = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(!hp){
			printf("ERROR: OpenProcess pid=%d\n",pid);
			return 1;
	}
	LPVOID lpbuf = ::VirtualAllocEx(hp,NULL,sizeof(SYSTEMTIME),MEM_COMMIT,PAGE_READWRITE);
	if(!lpbuf){
			printf("ERROR: VirtualAllocEx\n");
			return 2;
	}
	
	memset(&st,0,sizeof(st));
	
	char xx[5];
	strncpy(xx,ymd,4);
	xx[4]=0;
	st.wYear = atoi(xx);

	strncpy(xx,ymd+4,2);
	xx[2]=0;
	st.wMonth = atoi(xx);

	strncpy(xx,ymd+6,2);
	xx[2]=0;
	st.wDay= atoi(xx);

//	st.wYear = 2019;
//	st.wMonth = 10;
//	st.wDay=18;

	printf("DEBUG: yyyymmdd=%d%02d%02d\n",st.wYear,st.wMonth,st.wDay);

	::WriteProcessMemory(hp,lpbuf,&st,sizeof st,&n);
	::SendMessage(hwnd,DTM_SETSYSTEMTIME,0,(LPARAM)lpbuf);

	::VirtualFreeEx(hp,lpbuf,0,MEM_FREE);
	CloseHandle(hp);

	BOOL bret;
	bret = ::AttachThreadInput(tid,::GetCurrentThreadId(),TRUE);
	::SetFocus(hwnd);

	char buf[10];
	sprintf(buf,"%d",st.wYear);
	char * p = buf;
	while(*p){
		::keybd_event((BYTE)*p,0,0,0);
		::keybd_event((BYTE)*p,0,KEYEVENTF_KEYUP,0);

		p++;
	}

	bret = ::AttachThreadInput(tid,::GetCurrentThreadId(),FALSE);

	printf("DEBUG: OVER.\n");

	return 0;
}

