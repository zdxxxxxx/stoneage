/*-----------------------------------------------------------------------------
    caryMemory_lib.h -- cary's Virtual memory �޲z�C
    Waei(c) All rights reserved
    author: �\�ʳ�Cary Hsu, 2000.10.8
-----------------------------------------------------------------------------*/
#ifndef __MEMDLL
#define __MEMDLL

#define _MEM_LIB

#ifdef _MEM_LIB
	#define MEMDLL_API
#else
	#ifdef MEMDLL_EXPORTS
		#define MEMDLL_API __declspec(dllexport)
	#else
		#define MEMDLL_API __declspec(dllimport)
	#endif
#endif

//�^�Ǫ����~***************************************************************************************
#define ERROR_MEMORY_OK				0x00000000
#define ERROR_MEMORY_INITFAILED		0x00000001	//�t�mHandle�}�C���~

#define ERROR_HEAP_NOT_ENOUGH		0x00000010
#define ERROR_HEAP_LIST_NOT_ENOUGH	0x00000020

//*************************************************************************************************
/*	��l�ưO����禡�w
return:	ERROR_MEMORY_OK:		���\
		ERROR_MEMORY_INITFAILED:��l�ƥ���	*/
DWORD MemoryInit();

/*	����O����禡�w���O����	*/
void MemoryRelease();

//*************************************************************************************************
/*	�O�d�@�j�d��Virtual Memory�A�öǦ^Handle�A��ĳ�Hpage�����(���t�m1��commit page���O����)
parameter:	lpdwHandle:	�Ǧ^Handle
			iSize:		�n�t�m�h�ְO����A�æ^�ǫإߪ��j�p
return:		�DNULL:		�^�ǰt�m�O���骺��}
			NULL:		����					*/
MEMDLL_API LPVOID VirtualReserve( LPDWORD lpdwHandle, int &iSize);

/*	����Handle�t�m���O����
parameter:	lpdwHandle:	Handle��	*/
MEMDLL_API void VirtualRelease( LPDWORD lpdwHandle);

/*	�o��Exception���B�z�禡
parameter:	dwHandle:						Handle��
			lpEP:							EXCEPTION_POINTERS��structure
return:		EXCEPTION_CONTINUE_EXECUTION:	�~�����
			EXCEPTION_CONTINUE_SEARCH:		�~��M��i�B�zexception handle	*/
MEMDLL_API int ExpFilter( DWORD dwHandle, LPEXCEPTION_POINTERS lpEP);

//*************************************************************************************************
//*************************************************************************************************
/*	�t�miSize�ӰO�����Heap
parameter:	lpdwHandle:		�Ǧ^Handle
			iSize:			�n�t�m�h�֪��O����
return:		TRUE:			�t�m���\
			FALSE:			����				*/
MEMDLL_API BOOL HeapCreate( LPDWORD lpdwHandle, int &iSize);

/*	����Heap���Ҧ��O����
parameter:	lpdwHandle:	Handle��	*/
MEMDLL_API void HeapRelease( LPDWORD lpdwHandle);

/*	�M��Heap��List�A�A�ϥ�
parameter:	dwHandle:	Handle��	*/
MEMDLL_API void HeapReset( DWORD dwHandle);

/*	�qlpheap���A�t�mdwSize�j�p���O����
parameter:	dwHandle:	Handle��
			dwSize:		�n�t�m�h�ְO����
return:		�DNULL:		�Ǧ^�t�m���O����
			NULL:		����				*/
MEMDLL_API LPVOID HeapAlloc( DWORD dwHandle, DWORD dwSize);

/*	�qlpheap������Heap��memlt
parameter:	dwHandle:	Handle��
			lpAddress:	�n���񪺰O�����}����}				*/
MEMDLL_API void HeapFree( DWORD dwHandle, LPVOID *lpAddress);

//*************************************************************************************************
//*************************************************************************************************
typedef struct USEDMEMORY{
	LPBYTE			lpbyStart;
	DWORD			dwSize;
}*LPUSEDMEMORY;

/*	���^lpheap���Ĥ@��Heap list
parameter:	dwHandle:	Handle��
return:		�Ǧ^�ϥΤ���address�ANULL���̫�@��	*/
MEMDLL_API LPUSEDMEMORY GetMemoryListHead( DWORD dwHandle);

/*	���^lpheap���U�@��Heap list
parameter:	dwHandle:	Handle��
return:		�Ǧ^�ϥΤ���address�ANULL���̫�@��	*/
MEMDLL_API LPUSEDMEMORY GetMemoryListNext( DWORD dwHandle);

/*	���^���~��
parameter:	dwHandle:	Handle��
return:		�^�ǿ��~��			*/
MEMDLL_API DWORD GetCaryError( DWORD dwHandle);

//*************************************************************************************************
//*************************************************************************************************
/*	�h���O����A�ϥ�MMX���w��pentum II�̨Τƪ��B�z
parameter:	dest:	�ت���}
			src:	�ӷ���}
			cnt:	�h����byte��
return:		TRUE:	���\����
			FALSE:	��}���X�k						*/
MEMDLL_API BOOL mmx_CopyMemory( LPVOID dest, LPVOID src, int cnt);

/*	�h���O����A�ϥ�MMX���w��pentum II�̨Τƪ��B�z
parameter:	dest:	�ت���}
			src:	�ӷ���}
			cnt:	�h����byte��
return:		TRUE:	���\����
			FALSE:	��}���X�k						*/
MEMDLL_API BOOL mmx_CopyMemory_test( LPVOID dest, LPVOID src, int cnt);

#endif
