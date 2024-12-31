/******************************************************************************
	caryIme.h:	cary's Ime��DllMain�C
												(c) �\�ʳ�Cary Hsu,	2000.7.10
******************************************************************************/
#ifndef __CARYIME
#define __CARYIME

typedef void(*RESULTCALLBACK)(LPSTR);

/*	IME����l��
parameter:	hwnd:			�ҨϥΪ�HWND
			ResultCallback:	�����J�k�����G�^��
return:		TRUE:			�Y�������~�A�е����{���A�_�h�L�k�ϥΤ����J�k*/
BOOL InitIme(HWND hwnd,RESULTCALLBACK ResultCallback);

/*	IME����*/
void EndIme();

/*	������ܪ��Ҧ��Awindow mode��fullscreen mode
parameter:	bWindowMode:	�ϥΪ������Ҧ�		*/
void ChangeMode(BOOL bWindowMode);

/*	�B�zIME��message
parameter:	Message:	�����T��
			wParam,lParam:�����T�����Ѽ�
return:	TRUE:	�w�g���B�zIME��message
		FALSE:	�٦���]��message�ݭn�B�z	*/
BOOL ImeMessage(UINT Message,WPARAM wParam,LPARAM lParam);

/*	clear IME buffer	*/
void ClearIme();

/*	IME��ON,OFF���A�T�{
return:	ON:	TRUE
		OFF:FALSE		*/
BOOL ImeOpenState();

/*	���o��J�k���r��A�i��candiate��compsition	
return:	NULL:	�S���r��A�Цۦ�B�z
		�DNULL:	����J�k���r��				*/
LPSTR GetImeString();

/*	���o��J�k���y�z�r��
return:	��J�k���y�z�r��	*/
LPSTR GetImeDescString();

///////////////////////////////////////////////////////////////////////////////

/*	���olpstr���W�@�Ӧr����}
parameter:	lpstrHead:	�r�ꪺ�Y
			lpstr:		�n���^�W�@�Ӧr���r��
return:		�W�@�Ӧr����}					*/
char *GetCharPrev(char *lpstrHead,char *lpstr);

#endif