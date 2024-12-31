﻿#include "windows.h"
#include<direct.h>
#include "../systeminc/version.h"
#include "../systeminc/process.h"
#include "../systeminc/t_music.h"
#include "../systeminc/handletime.h"
#include "../systeminc/savedata.h"
#include "../systeminc/system.h"
#define ACTIVE	1
//#include "system.h"
//#include "../oft/vg410.h"

extern	DWORD NowTime;
//extern double NowTime;	// ???????

/*???????????*/
#define DEBUG_ON	1
#define THREAD_ON	1
#define MODE98		0
#define CLASS_UP	1
#define VOICE_KILL_ON	0

//噙秞梓祩
BOOL MuteFlag = FALSE;

typedef struct{
	int name;
	int tone_no;
	int volume;
	int note_no;
	int release_cnt;
	int release_spd;
	int lfo_h;
	int lfo_m;
	int lfo_s;
	int lfo_s_cnt;
	int lfo_s_ang;
	int pan;
	int loop_flg;
	int delay_cnt;
	long freq;
} VOICE_EQU;

#define	MAX_ENV		20
typedef struct{
	int voice_address;	//???????????????
	int tone;
	int count;
	int distance;
	int volume;
	int volume_old;
	int side;
	int panpot;
	int panpot_old;
} ENVIRONMENT_EQU;
ENVIRONMENT_EQU env_tbl[MAX_ENV];

#define DEBUG	0
#define MY_TIMER	1
#define VOICE_MAX	64
#define TRACK_MAX	32
//#define STRICT
#include <windows.h>
#include <dsound.h>
#include <stdio.h>
#include <winuser.h>
#include <winbase.h>
#include "../systeminc/math2.h"
#include <math.h>
#include "../systeminc/main.h"

#include <tchar.h>
#include <crtdbg.h>
#include <mmsystem.h>
extern int MessageBoxNew(HWND hWnd,LPCSTR lpText,LPCSTR lpCaption,UINT uType);
#define RELEASE(x) 	if(x){x->Release();x=NULL;}
#define	LOG(a,b)	(log((float)b)/log((float)a))

static int mes_flg=0;

#if THREAD_ON
int thread_flg=0;
#endif
int dsound_flg=0;

/*-------------------------------------------
	??阪?
--------------------------------------------*/
LPDIRECTSOUND pDSound = NULL;
LPDIRECTSOUNDBUFFER pDSPrimary = NULL;
LONG primaryVolume;
LONG primaryPan;
LONG primaryFrequency;

LPDIRECTSOUNDBUFFER pDSData_tone[TONE_MAX + 1];
LPDIRECTSOUNDBUFFER pDSData_stream;
LPDIRECTSOUNDBUFFER pDSData[VOICE_MAX];
BOOL dataPlay[VOICE_MAX];
int dataVolume[TRACK_MAX];
int dataPan[TRACK_MAX];
LONG dataFrequency[TRACK_MAX];
VOICE_EQU voice[VOICE_MAX];
TONE_EQU tone_tbl[TONE_MAX];
int tone_max_buf;

#if VOICE_KILL_ON
int voice_kill_buf[VOICE_MAX];
int voice_kill_point;
#endif

int t_music_playing_flg = 0;
int t_music_fade_flg = 0;
int t_music_fade_vol;
int draw_map_bgm_flg = 0;
static int map_bgm_vct_no = 0;
int map_bgm_no = 0;

/*-------------------------------------------
	Wave?????????
--------------------------------------------*/
BOOL LoadWave(
			  LPCTSTR lpName,	// Wave????
			  WAVEFORMATEX* (&pWaveHeader),	// WAVEFORMATEX?﹨▍??????
			  BYTE* (&pbWaveData),	// ??????????????
			  DWORD &cbWaveSize)	// ????????????
{
	pWaveHeader = NULL;	// WAVEFORMATEX?﹨▍??????
	pbWaveData = NULL;	// ??????????????
	cbWaveSize = 0;	// ????????????
	const char c_szWAV[] = "WAVE";	// ???????Wave????????"WAVE"???

	// ???????????????????
	HRSRC hResInfo = FindResource(NULL, lpName, c_szWAV);
	if(hResInfo == NULL)
	    return FALSE;
	HGLOBAL hResData = LoadResource(NULL, hResInfo);
	if(hResData == NULL)
	    return FALSE;
	void *pvRes = LockResource(hResData);
	if(pvRes == NULL)
	    return FALSE;

	// ?????????????????╱??????????????????台??
	DWORD *pdw = (DWORD *)pvRes;
	DWORD *pdwEnd;
	DWORD dwRiff = *pdw++;
	DWORD dwLength = *pdw++;
	DWORD dwType = *pdw++;

	if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
	    return FALSE;      // ???????RIFF?????????
	if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
	    return FALSE;      // ???????Wave?????????
	pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

	while (pdw < pdwEnd)
	{
	    dwType = *pdw++;
	    dwLength = *pdw++;

	    switch (dwType)
	    {
	    case mmioFOURCC('f', 'm', 't', ' '):
	        if (pWaveHeader == NULL)
	        {
	            if (dwLength < sizeof(WAVEFORMAT))
	                return FALSE;      // ?????????Wave?????????
	            pWaveHeader = (WAVEFORMATEX *)pdw;
	        }
	        break;
	    case mmioFOURCC('d', 'a', 't', 'a'):
	        if ((pbWaveData == NULL) || (!cbWaveSize))
	        {
	            pbWaveData = (LPBYTE)pdw;
	            cbWaveSize = dwLength;
	        }
	        break;
	    }
	    if(pWaveHeader && (pbWaveData != NULL) && cbWaveSize)
	        break;
	    pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}
	if(pdwEnd <= pdw)
	    return FALSE;	// ?????????Wave?????????

	// Wave???台?

	return TRUE;
}

/*-------------------------------------------
	????????????????????
--------------------------------------------*/
BOOL CreateSoundData(LPCSTR pName, LPDIRECTSOUNDBUFFER &pDSData)
{
	// ??????Wave????????
	WAVEFORMATEX *pWaveHeader = NULL;	// WAVEFORMATEX?﹨▍??????
	BYTE *pbWaveData = NULL;	// ??????????????
	DWORD cbWaveSize = 0;	// ????????????
	if(!LoadWave(pName, pWaveHeader, pbWaveData, cbWaveSize))
	{
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "黍Wave腔訧蹋囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}

	// ??????????????
	DSBUFFERDESC dsbdesc; 
	HRESULT hr; 
	// DSBUFFERDESC?﹨▍?　叉
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	// ??????????????
#if ACTIVE
	dsbdesc.dwFlags = DSBCAPS_STATIC | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS; 
#else
	dsbdesc.dwFlags = DSBCAPS_STATIC | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2; 
#endif
	dsbdesc.dwBufferBytes = cbWaveSize;
	dsbdesc.lpwfxFormat = pWaveHeader;
	// ???????
	hr = pDSound->CreateSoundBuffer(&dsbdesc, &pDSData, NULL); 
	if(DS_OK != hr) { 
	    // ????????
		return FALSE;
	} 

	// ???????????Wave????????
	LPVOID lpvPtr1; 	// ????????????
	DWORD dwBytes1; 	// ???????????
	LPVOID lpvPtr2; 	// ?吻???????????
	DWORD dwBytes2; 	// ?吻??????????

	hr = pDSData->Lock(0, cbWaveSize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 

	// DSERR_BUFFERLOST?刺?????Restore????????????????
	if(DSERR_BUFFERLOST == hr) { 
	    pDSData->Restore(); 
	    hr = pDSData->Lock(0, cbWaveSize, &lpvPtr1, &dwBytes1, &lpvPtr2, &dwBytes2, 0); 
	} 
	if(DS_OK == hr) { 
	    // ????

	    // ?????????????
		// ??????????????
		CopyMemory(lpvPtr1, pbWaveData, dwBytes1);
		if ( 0 != dwBytes2 )
		    CopyMemory(lpvPtr2, pbWaveData + dwBytes1, dwBytes2);

	   // ????????????Unlock???
	    hr = pDSData->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2); 
	}

	return TRUE;
}

// ????????????
//
// ?  ?ㄩ	hmmio     ... ????????????
//			pWfmtx    ... ?????宇??????
//			pDataSize ... ??????
// ??πㄩ	TRUE  ... ?
//			FALSE ... ?
static BOOL WaveFormatRead( HMMIO hmmio, WAVEFORMATEX *pWfmtx, DWORD *pDataSize )
{
	MMCKINFO parent, child;

	parent.ckid = (FOURCC)0;
	parent.cksize = 0;
	parent.fccType = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags = 0;
	child = parent;

	//WAVE?????Π?
	parent.fccType = mmioFOURCC('W','A','V','E');
	if ( (int)mmioDescend(hmmio,&parent,NULL,MMIO_FINDRIFF) ){
//		ERRPRINT("WAVE?????Π?");
		return FALSE;
	}

	//fmt?????向?
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if ( mmioDescend( hmmio , &child , &parent , MMIO_FINDCHUNK ) ){
//		ERRPRINT("fmt?????向?");
		return FALSE;
	}

	//??????????台
	if (mmioRead( hmmio , (char *)pWfmtx , sizeof(WAVEFORMATEX) ) != sizeof(WAVEFORMATEX) ){
//		ERRPRINT("??????????台");
		return FALSE;
	}

	//??????????????????
	if (pWfmtx->wFormatTag != WAVE_FORMAT_PCM ){
//		ERRPRINT("????????????????");
		return FALSE;
	}
	//?????????兌??
	if (mmioAscend( hmmio , &child , 0 )){
//		ERRPRINT("?????????兌??");
		return FALSE;
	}
	
	//data?????向?
	child.ckid = mmioFOURCC( 'd' , 'a' , 't' , 'a' );
	if ( mmioDescend( hmmio , &child , &parent , MMIO_FINDCHUNK ) ){
//		ERRPRINT("data?????向?");
		return FALSE;
	}

	*pDataSize = child.cksize;

	return TRUE;
}

static BYTE	*pDSbuffData;

//
// ???????????
//
// ?  ?ㄩ	hmmio     ... ????????????
//			pDataSize ... ??????????
//			pData     ... DirectSoundBuffer
// ??πㄩ	TRUE  ... ?
//			FALSE ... ?
static BOOL WaveDataRead( HMMIO hmmio , DWORD *pDataSize, LPDIRECTSOUNDBUFFER pData )
{
//	BYTE	*pDSbuffData;

	//??????????
	if( pData->Lock(0 , *pDataSize , (LPVOID*)&pDSbuffData , pDataSize , NULL , 0, 0 ) != DS_OK )
		return FALSE;

	//???????????
	*pDataSize = (DWORD)mmioRead(hmmio , (char*)pDSbuffData , *pDataSize);

	//?????
    if( pData->Unlock( pDSbuffData , *pDataSize , NULL , 0 ) != DS_OK )
		return FALSE;

	return TRUE;
}


//
// ??????????台
//
// ?  ?ㄩ	pWfmtx ... ???????????
//			DataSize ... ????????
//			ppData ... 台????????????????
// ??πㄩ	TRUE  ... ?
//			FALSE ... ?
static BOOL dwSoundInit( WAVEFORMATEX *pWfmtx, DWORD DataSize, LPDIRECTSOUNDBUFFER *ppData )
{
	DSBUFFERDESC  dsbufferdesc;

	//??????????????
	memset( &dsbufferdesc , 0  , sizeof( DSBUFFERDESC ) );
	dsbufferdesc.dwSize = sizeof( DSBUFFERDESC );
//	dsbufferdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLDEFAULT | DSBCAPS_GETCURRENTPOSITION2; 
#if ACTIVE
	dsbufferdesc.dwFlags = DSBCAPS_STATIC | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS; 
#else
	dsbufferdesc.dwFlags = DSBCAPS_STATIC | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2; 
#endif
    dsbufferdesc.dwBufferBytes = DataSize;
    dsbufferdesc.lpwfxFormat   = pWfmtx;
    if ( pDSound->CreateSoundBuffer( &dsbufferdesc , ppData , NULL ) != DS_OK )
		return FALSE;

	return TRUE;
}

int f_get(FILE *fp);
int get_num(FILE *fp);
int num_check(FILE *fp);

// ??????〈????更??
#define mxSTR	200
/*-------------------------------------------
	DirectSound ???
--------------------------------------------*/
int InitDSound(void)
{
	HRESULT ret;
	int d7;

	pDSound = NULL;
	pDSPrimary = NULL;
	for(d7=0;d7<VOICE_MAX;d7++)
		pDSData[d7]=NULL;
	for(d7=0;d7<VOICE_MAX;d7++)
		dataPlay[d7]=FALSE;

	ret = DirectSoundCreate(NULL, &pDSound, NULL);
	if(ret != DS_OK){
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "膘蕾DirectSound賡醱囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}
	//?????????　叉
	ret = pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(ret != DS_OK){
	    // ??????　叉??
		RELEASE(pDSound);
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "扢隅衪覃脹撰囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}
	// ?????　叉
	pDSound ->SetSpeakerConfig(DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE));
	// ????????????
	// DSBUFFERDESC?﹨▍?　叉
	DSBUFFERDESC dsbdesc; 
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
	// ???????????叉
	dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER; 
	dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat = NULL;
	// ???????
	ret = pDSound->CreateSoundBuffer(&dsbdesc, &pDSPrimary, NULL); 
	if(ret != DS_OK) { 
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "膘蕾翋猁buffer囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}
	// ???????????Wave???????　叉
	//       ?！?????????????　叉?????╱???????
	WAVEFORMATEX pcmwf; 
	ZeroMemory(&pcmwf, sizeof(WAVEFORMATEX)); 
	pcmwf.wFormatTag = WAVE_FORMAT_PCM; 
	pcmwf.nChannels = 2;		// ????????????
	pcmwf.nSamplesPerSec = 22050;	// ??????????  22.05kHz
	pcmwf.nBlockAlign = 4;
	pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign; 
	pcmwf.wBitsPerSample = 16;		// 16???
	ret = pDSPrimary->SetFormat(&pcmwf);
	if(ret != DS_OK){
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "翋猁buffer腔跡宒趙囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}
/*
//cary	妏蚚resource膘蕾sound data
	// ?????????????????????
	for(d7=0;d7<TONE_MAX;d7++){
		BOOL f;
		f = CreateSoundData(MAKEINTRESOURCE(101+d7), pDSData_tone[d7]);
		if(!f){
			for(d7=0;d7<TONE_MAX;d7++)
				RELEASE(pDSData_tone[d7]);
			RELEASE(pDSPrimary);
			RELEASE(pDSound);
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "膘蕾汒秞訧蹋囮啖ㄐ", "Error", MB_OK);
#endif
			return FALSE;
		}
	}
*/
	HMMIO hmmio;			//????????
    WAVEFORMATEX	Wfmtx;			//WAVE??????????????
	DWORD			DataSize;		//?????????
	DWORD			WaveSize;		//Wave????????
	int program_no = 0;
	char fname[mxSTR];

	hmmio = (HMMIO)-1;		//?????????
	tone_max_buf = 0;
	typedef struct
	{
		short no;
		char name[mxSTR];
		char vol;
		char note;
		char loop_flg;
	} SNDCONFIG;
	#include "../systeminc/sndcnf.h"
	for(d7=0;d7<TONE_MAX;d7++){		//????????
		tone_tbl[d7].voice_place = -1;
	}
	//??????????  ?叉??????  ??????????
	int d1 = 0;		//SNDCONFIG????????
	while(1){
		if(sndconfig[ d1 ].no == -1){		//???
			goto InitDSound_ok;
		}
		program_no = sndconfig[ d1 ].no;		//?吻????
		if(program_no > TONE_MAX){		//???????
			goto InitDSound_err2;	//???
		}
		tone_tbl[ program_no ].voice_place = tone_max_buf;		//????！卒㎏
		//?〈?台
		strcpy( fname, sndconfig[ d1 ].name );

		if(sndconfig[ d1 ].vol < 0){		//????????????
			goto InitDSound_err2;
		}
		tone_tbl[ program_no ].voice_volume = sndconfig[ d1 ].vol;		//??????卒㎏

		if(sndconfig[ d1 ].note > 60){		//???吻??????
			goto InitDSound_err2;
		}
		if(sndconfig[ d1 ].note){		//???吻?卒㎏
			tone_tbl[ program_no ].voice_note = sndconfig[ d1 ].note;
		} else {
			tone_tbl[ program_no ].voice_note = 0;
		}
		//?????????
		tone_tbl[ program_no ].voice_loop = sndconfig[ d1 ].loop_flg;

		//???????????
		int d2;
		FILE *fp;
		fp = fopen( fname, "rb" );
		if( fp == NULL ){
			d1++;		//SNDCONFIG??????
			continue;
		}
		d2 = 8;						//??????????????
		d2 += 4;					//??????????????
		d2 += 4;					//????  ?????????
		d2 += 8;					//????????
		fseek(fp,d2,SEEK_CUR);		//??????????去
		d2 = fgetc(fp);				//???????????
		d2 |= fgetc(fp) << 8;		//???????????
		d2 |= fgetc(fp) << 16;		//???????????
		d2 |= fgetc(fp) << 24;		//???????????
		switch(d2){
			case 11025:
				tone_tbl[ program_no ].voice_rate = 25;
				break;
			case 22050:
				tone_tbl[ program_no ].voice_rate = 37;
				break;
			case 44100:
				tone_tbl[ program_no ].voice_rate = 49;
				break;
		}
		fclose(fp);

		//????????????
		if( (hmmio = mmioOpen(fname, NULL, MMIO_READ | MMIO_ALLOCBUF )) == NULL){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "湖羲WAV紫囮啖ㄐ", "Error", MB_OK);
#endif
			goto InitDSound_err;
		}
		//???????????
		if( WaveFormatRead( hmmio , &Wfmtx , &WaveSize ) == FALSE ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "黍WAV跡宒囮啖ㄐ", "Error", MB_OK);
#endif
			goto InitDSound_err;
		}
		DataSize = WaveSize;
		//??????????????
		if( dwSoundInit( &Wfmtx, DataSize, &pDSData_tone[tone_max_buf] ) == FALSE ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "場宎趙汒秞buffer囮啖ㄐ", "Error", MB_OK);
#endif
			goto InitDSound_err;
		}
		//???????
		if( WaveDataRead( hmmio, &DataSize, pDSData_tone[tone_max_buf] ) == FALSE  ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "黍WAV紫腔訧蹋囮啖ㄐ", "Error", MB_OK);
#endif
			goto InitDSound_err;
		}
		mmioClose( hmmio , 0 );
		hmmio = (HMMIO)-1;		//?????????
		d1++;		//SNDCONFIG??????
		tone_max_buf++;		//????！?
	}

InitDSound_err:		//?????????????
	mmioClose( hmmio , 0 );
	for(d7=0;d7<tone_max_buf;d7++)
		RELEASE(pDSData_tone[d7]);
	RELEASE(pDSPrimary);
	RELEASE(pDSound);
	return FALSE;

InitDSound_err2:	//叉????????
	if(hmmio != (HMMIO)-1){		//????????????
		mmioClose( hmmio , 0 );
	}
#ifdef _STONDEBUG_
	MessageBoxNew(hWnd, "?　叉???????", "Error", MB_OK);
#endif
	for(d7=0;d7<tone_max_buf;d7++)
		RELEASE(pDSData_tone[d7]);
	RELEASE(pDSPrimary);
	RELEASE(pDSound);
	return FALSE;

InitDSound_ok:		//?
	// ????希?吻???佃??
	for(d7=0;d7<VOICE_MAX;d7++){
		if( pDSound->DuplicateSoundBuffer(pDSData_tone[0],&pDSData[d7]) != DS_OK ){
			for(d7=0;d7<tone_max_buf;d7++)
				RELEASE(pDSData_tone[d7]);
			for(d7=0;d7<VOICE_MAX;d7++)
				RELEASE(pDSData[d7]);
			RELEASE(pDSPrimary);
			RELEASE(pDSound);
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "葩秶汒秞buffer囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
		}
	}
	// ????????π?台??
	pDSPrimary->GetVolume(&primaryVolume);
	// ?????
	pDSPrimary->GetPan(&primaryPan);
	// ??
	pDSPrimary->GetFrequency((DWORD*)&primaryFrequency);
	return TRUE;
}

volatile static char exit_thread_flg = 0;
static int thread_loop_start;
//static int thread_loop_end;
static int thread_stop_flg = 0;
static LPDIRECTSOUNDNOTIFY pDSNotify = NULL;
static char stream_flg = 0;
/*-------------------------------------------
	????于??
--------------------------------------------*/
void stop_thread(void)
{
	//????于??
	if(thread_stop_flg){
		//???于
		pDSData_tone[ TONE_MAX ]->Stop();
//		pDSData_stream->Stop();
		while(1){
			if(exit_thread_flg == 1)
				break;
		}
		thread_stop_flg = 0;
		exit_thread_flg = 0;
		RELEASE(pDSData_stream);
	}
	//???????????
	RELEASE(pDSData_tone[TONE_MAX]);
}

/*-------------------------------------------
	???于??
--------------------------------------------*/
void stop_bgm(void)
{
	if (MuteFlag) return;
	//????于??
	if(thread_stop_flg){
		//????于?
		stop_thread();
	} else {
		//???????????
		RELEASE(pDSData_tone[TONE_MAX]);
	}
	//??????????
	t_music_fade_flg = 0;
	//????????
	t_music_playing_flg = 0;
	//???阪?????吻????
	map_bgm_vct_no = 0;
	 map_bgm_no = t_music_bgm_no;
}

/*-------------------------------------------
	?????????
--------------------------------------------*/
void fade_out_bgm(void)
{
	//????????
	t_music_fade_flg = 1;
	//?????卒㎏
	t_music_fade_vol = bgm_tbl[t_music_bgm_no].volume * t_music_bgm_volume / 15;

}

/*-------------------------------------------
	????
--------------------------------------------*/
int EndApp(void)
{
	int d7;
	//????于?
	stop_thread();
	RELEASE(pDSData_tone[TONE_MAX]);
	//??????
	if(stream_flg == 1){
		RELEASE(pDSNotify);
	}
	for(d7=0;d7<VOICE_MAX;d7++){
		pDSData[d7]->Stop();
		RELEASE( pDSData[d7] );
	}
	for(d7=0;d7<tone_max_buf;d7++){
		pDSData_tone[d7]->Stop();
		RELEASE( pDSData_tone[d7] );
	}
	RELEASE(pDSPrimary);
	RELEASE(pDSound);

	return TRUE;
}

#define MUSIC_END	-1
#define MUSIC_R		-2
#define MUSIC_LOOP	-3
#define MUSIC_FREQ	-4
#define MUSIC_VOL	-5
#define MUSIC_PAN	-6
#define MUSIC_TEMPO	-7
#define MUSIC_PEDAL	-8
#define MUSIC_NEIRO_NO	-9
#define MUSIC_KURI_RET	-10
#define MUSIC_KURI_SKIP	-11
#define MUSIC_KURI_START	-12
#define MUSIC_WAVE	-13
#define MUSIC_WAON	-14
#define MUSIC_DETUNE	-15
#define MUSIC_LFO_H		-16
#define MUSIC_LFO_M		-17
#define MUSIC_LFO_S		-18
static int cnt=0,cnt2=0;
static LARGE_INTEGER d_cnt;
static int ch;
static int step_time[TRACK_MAX];
static int gate_time[TRACK_MAX];
static int d_pedal[TRACK_MAX];
static int wave[TRACK_MAX];
static int step_point[TRACK_MAX];
static int step_def[TRACK_MAX];
static int play_flg[TRACK_MAX];
static int play_track_flg[TRACK_MAX];
static int play_kuri_num[8][TRACK_MAX];
static int play_kuri_lv[TRACK_MAX];
static int play_use_task[TRACK_MAX][8];
static int dataToneNo[TRACK_MAX];
static int tie_flg[TRACK_MAX];
static int note_old[TRACK_MAX][8];
static int play_use_task_cnt[TRACK_MAX];
static int play_detune[TRACK_MAX];
static int play_lfo_h[TRACK_MAX];
static int play_lfo_m[TRACK_MAX];
static int play_lfo_s[TRACK_MAX];
static int pause_flg;
static LARGE_INTEGER tempo;
static int step[TRACK_MAX][16384];
typedef enum {
	M_OPEN_ERR = 0,
	M_STEP_ERR,
	M_TRACK_ERR,
	M_LCOMMAND_ERR,
	M_TCOMMAND_ERR,
	M_VCOMMAND_ERR,
	M_V2COMMAND_ERR,
	M_P2COMMAND_ERR,
	M_F2COMMAND_ERR,
	M_2COMMAND_ERR,
	M_KAKKO_ERR,
	M_MML_ERR,
	M_D2COMMAND_ERR,
	M_NOTE_ERR,
	M_PLAY_ERR,
	M_KURI_ERR,
	M_KURISU_ERR,
	M_MUTE_ERR,
	M_W2COMMAND_ERR,
	M_TOCOMMAND_ERR,
	M_QCOMMAND_ERR,
	M_K2COMMAND_ERR,
	M_WAON_ERR,
	M_OCOMMAND_ERR,
	M_H2COMMAND_ERR,
	M_M2COMMAND_ERR,
	M_S2COMMAND_ERR,
} err_number;

static char *err_msg[]={
	"紫偶羲囮啖",
	"????奀潔渣昫",
	"秞寢硌隅渣昫",
	"?硌鍔渣昫",
	"?硌鍔渣昫",
	"?硌鍔渣昫",
	"??硌鍔渣昫",
	"??硌鍔渣昫",
	"??硌鍔渣昫",
	"?硌鍔渣昫",
	"????硌鍔渣昫",
	"???渣昫",
	"??硌鍔渣昫",
	"笚疏杅閉徹渣昫",
	"栳軠秞寢硌隅渣昫",
	"毀葩畢溫硌鍔渣昫",
	"毀葩畢溫杅祥磁",
	"噙秞寢硌隅渣昫",
	"??硌鍔渣昫",
	"誹軠硌鍔渣昫",
	"?硌鍔渣昫",
	"??硌鍔渣昫",
	"睿秞渣昫",
	"?硌鍔渣昫",
	"??硌鍔渣昫",
	"??硌鍔渣昫",
	"??硌鍔渣昫",
};
//c c+ d d+ e f f+ g g+ a a+ b
//0 1  2 3  4 5 6  7 8  9 10 11
static int note_change_tbl[]={9,11,0,2,4,5,7};
static int freq_tbl[]={
	(int)(44100.0 / 261.63 * 277.18 / 32.0),	//b (o-1) 0	???
	2756,										//c (o0) 1
	(int)(44100.0 / 261.63 * 277.18 / 16.0),	//c+
	(int)(44100.0 / 261.63 * 293.67 / 16.0),	//d	
	(int)(44100.0 / 261.63 * 311.13 / 16.0),	//d+
	(int)(44100.0 / 261.63 * 329.63 / 16.0),	//e
	(int)(44100.0 / 261.63 * 349.23 / 16.0),	//f
	(int)(44100.0 / 261.63 * 366.99 / 16.0),	//f+
	(int)(44100.0 / 261.63 * 392.00 / 16.0),	//g
	(int)(44100.0 / 261.63 * 415.31 / 16.0),	//g+
	(int)(44100.0 / 261.63 * 440.00 / 16.0),	//a
	(int)(44100.0 / 261.63 * 466.16 / 16.0),	//a+
	(int)(44100.0 / 261.63 * 493.88 / 16.0),	//b
	5512,									//c (o1) 13
	(int)(44100.0 / 261.63 * 277.18 / 8.0),	//c+
	(int)(44100.0 / 261.63 * 293.67 / 8.0),	//d
	(int)(44100.0 / 261.63 * 311.13 / 8.0),	//d+
	(int)(44100.0 / 261.63 * 329.63 / 8.0),	//e
	(int)(44100.0 / 261.63 * 349.23 / 8.0),	//f
	(int)(44100.0 / 261.63 * 366.99 / 8.0),	//f+
	(int)(44100.0 / 261.63 * 392.00 / 8.0),	//g
	(int)(44100.0 / 261.63 * 415.31 / 8.0),	//g+
	(int)(44100.0 / 261.63 * 440.00 / 8.0),	//a
	(int)(44100.0 / 261.63 * 466.16 / 8.0),	//a+
	(int)(44100.0 / 261.63 * 493.88 / 8.0),	//b
	11025,									//c (o2) 25
	(int)(44100.0 / 261.63 * 277.18 / 4.0),	//c+
	(int)(44100.0 / 261.63 * 293.67 / 4.0),	//d
	(int)(44100.0 / 261.63 * 311.13 / 4.0),	//d+
	(int)(44100.0 / 261.63 * 329.63 / 4.0),	//e
	(int)(44100.0 / 261.63 * 349.23 / 4.0),	//f
	(int)(44100.0 / 261.63 * 366.99 / 4.0),	//f+
	(int)(44100.0 / 261.63 * 392.00 / 4.0),	//g
	(int)(44100.0 / 261.63 * 415.31 / 4.0),	//g+
	(int)(44100.0 / 261.63 * 440.00 / 4.0),	//a
	(int)(44100.0 / 261.63 * 466.16 / 4.0),	//a+
	(int)(44100.0 / 261.63 * 493.88 / 4.0),	//b
	22050,									//c (o3) 37
	(int)(44100.0 / 261.63 * 277.18 / 2.0),	//c+
	(int)(44100.0 / 261.63 * 293.67 / 2.0),	//d
	(int)(44100.0 / 261.63 * 311.13 / 2.0),	//d+
	(int)(44100.0 / 261.63 * 329.63 / 2.0),	//e
	(int)(44100.0 / 261.63 * 349.23 / 2.0),	//f
	(int)(44100.0 / 261.63 * 366.99 / 2.0),	//f+
	(int)(44100.0 / 261.63 * 392.00 / 2.0),	//g
	(int)(44100.0 / 261.63 * 415.31 / 2.0),	//g+
	(int)(44100.0 / 261.63 * 440.00 / 2.0),	//a
	(int)(44100.0 / 261.63 * 466.16 / 2.0),	//a+
	(int)(44100.0 / 261.63 * 493.88 / 2.0),	//b
	44100,									//c (o4) 49
	(int)(44100.0 / 261.63 * 277.18),		//c+
	(int)(44100.0 / 261.63 * 293.67),		//d
	(int)(44100.0 / 261.63 * 311.13),		//d+
	(int)(44100.0 / 261.63 * 329.63),		//e
	(int)(44100.0 / 261.63 * 349.23),		//f
	(int)(44100.0 / 261.63 * 366.99),		//f+
	(int)(44100.0 / 261.63 * 392.00),		//g
	(int)(44100.0 / 261.63 * 415.31),		//g+
	(int)(44100.0 / 261.63 * 440.00),		//a
	(int)(44100.0 / 261.63 * 466.16),		//a+
	(int)(44100.0 / 261.63 * 493.88),		//b
	88200,									//b+ (o5) 61
	(int)(44100.0 / 261.63 * 277.18 * 2.0),	//c+
	(int)(44100.0 / 261.63 * 293.67 * 2.0),	//d		?????63
	(int)(44100.0 / 261.63 * 311.13 * 2.0),	//d+
	(int)(44100.0 / 261.63 * 329.63 * 2.0),	//e
	(int)(44100.0 / 261.63 * 349.23 * 2.0),	//f
	(int)(44100.0 / 261.63 * 366.99 * 2.0),	//f+
	(int)(44100.0 / 261.63 * 392.00 * 2.0),	//g
	(int)(44100.0 / 261.63 * 415.31 * 2.0),	//g+
	(int)(44100.0 / 261.63 * 440.00 * 2.0),	//a
	(int)(44100.0 / 261.63 * 466.16 * 2.0),	//a+
	(int)(44100.0 / 261.63 * 493.88 * 2.0),	//b
};
static long volume_tbl[128];
static long panpot_tbl[128];
static int detune_tbl[12*5+5];

int get_num(FILE *fp);
/*-------------------------------------------
	??????????
--------------------------------------------*/
int f_get(FILE *fp)
{
	int d0;
	while(1){
		d0 = fgetc( fp );		//????吻???
		if( d0 == EOF )
			return d0;
		if( d0 == '\t' )		//???
			continue;
		if( d0 != 0x20 )		//?????
			break;
	}
	if(d0 >= 'A' && d0 <= 'Z')		//?更??更?阪?
		d0+=0x20;					//
	return d0;
}

/*-------------------------------------------
	?π??????
--------------------------------------------*/
int get_num(FILE *fp)
{
	int d0,d1;
	while(1){
		d0 = f_get( fp );		//????吻???
		if( d0 == EOF )
			return d0;
		if( d0 >= '0' && d0<= '9' )		//???
			break;						//??????
	}
	d1=d0-'0';		//?????

	while(1){
		d0 = fgetc( fp );		//????吻???
		if( d0 == EOF )
			break;
		if( d0 < '0' || d0> '9' )		//???
			break;						//?????????
		d1*=10;		//???????
		d1+=d0-'0';	//
	}
	fseek(fp,-1,SEEK_CUR);		//??????
	return d1;
}
/*-------------------------------------------
	?π???????????????
--------------------------------------------*/
int get_num2(FILE *fp)
{
	int d0,d1,d2=1;
	d0 = f_get( fp );		//??????
	if(d0=='-')		//??????
		d2=-1;
	else
		fseek(fp,-1,SEEK_CUR);		//??????

	while(1){
		d0 = fgetc( fp );		//????吻???
		if( d0 == EOF )
			return d0;
		if( d0 >= '0' && d0<= '9' )		//???
			break;						//??????
	}
	d1=d0-'0';		//?????

	while(1){
		d0 = fgetc( fp );		//????吻???
		if( d0 == EOF )
			break;
		if( d0 < '0' || d0> '9' )		//???
			break;						//?????????
		d1*=10;		//???????
		d1+=d0-'0';	//
	}
	fseek(fp,-1,SEEK_CUR);		//??????
	return d1*d2;
}
/*-------------------------------------------
	?????????????????
--------------------------------------------*/
int num_check2(FILE *fp)
{
	int d0;
	d0 = f_get( fp );		//??????
	if(d0=='-'){			//??????
		d0 = fgetc( fp );		//?????????
		fseek(fp,-2,SEEK_CUR);		//??????
		if( d0 >= '0' && d0<= '9' )		//???
			return 1;		//??
		else
			return 0;		//???
	} else {
		fseek(fp,-1,SEEK_CUR);		//??????
		if( d0 >= '0' && d0<= '9' )		//???
			return 1;		//??
		else
			return 0;		//???
	}
}

/*-------------------------------------------
	????????
--------------------------------------------*/
int num_check(FILE *fp)
{
	int d0;
	d0 = f_get( fp );		//??????
	fseek(fp,-1,SEEK_CUR);		//??????
	if( d0 >= '0' && d0<= '9' )		//???
		return 1;		//??
	else
		return 0;		//???
}
/*-------------------------------------------
	?????????????
--------------------------------------------*/
int get_step(FILE *fp,int ch)
{
	int d0,futen,total;
	total = 0;
	futen = 0;
	while(1){
		d0 = f_get( fp );		//?????
		if( d0 >= '0' && d0<= '9' ){		//????
			fseek(fp,-1,SEEK_CUR);		//??????
			d0 = get_num( fp );
			if(d0 > 192)
				return -1;
			futen = 192 / d0;
			total += futen;
		} else if(d0 == '.'){		//至???
			if(!total){		//????　叉????
				total = step_def[ch];		//至??????
				futen = step_def[ch];		//
			}
			futen /= 2;
			total += futen;
		} else if(d0 == '^'){		//??????
			d0 = f_get( fp );		//?????
			fseek(fp,-1,SEEK_CUR);		//??????
			if(d0 == '*'){		//‧▋????
				continue;
			}
			if(!num_check( fp )){		//???????
				return -1;		//???
			}
			d0 = get_num( fp );
			if(d0 > 192)
				return -1;
			if(!total){		//????　叉????
				total = step_def[ch];		//至??????
			}
			futen = 192 / d0;
			total += futen;
		} else if(d0 == '*'){		//‧▋????
			if(!num_check( fp )){		//???????
				return -1;		//???
			}
			futen = get_num( fp );
			total += futen;
		} else {
			fseek(fp,-1,SEEK_CUR);		//??????
			if(total)		//????　叉????
				break;
			futen = step_def[ch];
			total = futen;
		}
	}
	return total;
}
/*-------------------------------------------
	???????????
--------------------------------------------*/
int music_read( void )
{
	int d0,d1,d7,ch,ch_p[TRACK_MAX],do_p[TRACK_MAX],ch_oct[TRACK_MAX],err_flg=-1,err_line=0;
	int ch_ret[8][TRACK_MAX],ch_skip[8][TRACK_MAX],ch_kuri_lv[TRACK_MAX],waon_delay[TRACK_MAX];
	int o_tempo,q_com[TRACK_MAX];
	long d6;
	char moji[256];
	FILE *fp;
	fp = fopen( "oft.mml", "r" );
	if( fp == NULL ){
		sprintf( moji ,"%s" ,err_msg[0] );
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, moji, "Error", MB_OK);
#endif
		return err_flg;
	}

	for(d7=0;d7<TRACK_MAX;d7++){
		ch_p[d7] = 0;
		do_p[d7] = 0;
		ch_oct[d7] = 4;
		step_def[d7] = 48;
		play_track_flg[ d7 ] = 0;
		ch_kuri_lv[d7] = -1;
		for(d6=0;d6<8;d6++)
			ch_skip[d6][d7] = -1;
		q_com[d7] = 8;
		waon_delay[d7] = 0;
	}
	o_tempo=0;

	//??????????  ??  ??????????

	while(1){

		//??????????  ????吻?　叉  ??????????

		err_line++;

		if((d0 = f_get( fp )) == EOF)		//????吻???
			break;		//?????Ψ?
		if( d0 == '\n' )		//???
			continue;			//????????
		if( d0 == '/' ){		//?????
			while(1){			//????????
				d0 = f_get( fp );
				if(d0 == '\n' || d0 == EOF )
					break;
			}
			continue;			//????????
		}
		if( d0 != '(' ){		//??????
			err_flg=M_TRACK_ERR;
			goto music_read_999;
		}
		if((d0 = f_get( fp )) == EOF)		//????吻???
			break;		//?????Ψ?
		if( d0 == 'p' ){		//??????叉?
			if((d0 = f_get( fp )) == EOF){		//「???????
				err_flg=M_PLAY_ERR;
				goto music_read_999;

			}
			if( d0 == ')'){		//??
				err_line--;		//??㎝??????????
				continue;
			}

			fseek(fp,-1,SEEK_CUR);		//??????
			for(d7=0;d7<TRACK_MAX;d7++)		//?????????
				play_track_flg[ d7 ] = 0;	//

			while(1){
				if((d0 = fgetc( fp )) == EOF){		//??
					err_flg=M_PLAY_ERR;
					goto music_read_999;
				}
				if( d0 == ')')			//??
					break;

				if( d0 >= '0' && d0<= '9' ){		//???
					fseek(fp,-1,SEEK_CUR);		//??????
					d0 = get_num( fp );
					if(d0 < 1 || d0 > TRACK_MAX){
						err_flg=M_PLAY_ERR;
						goto music_read_999;
					}
					play_track_flg[ d0-1 ] = 1;	//?????????
				}
			}
			err_line--;		//??㎝??????????
			continue;
		}

		if( d0 == 's' ){		//????????叉?
			while(1){
				if((d0 = fgetc( fp )) == EOF){		//??
					err_flg=M_MUTE_ERR;
					goto music_read_999;
				}
				if( d0 == ')')			//??
					break;

				if( d0 >= '0' && d0<= '9' ){		//???
					fseek(fp,-1,SEEK_CUR);		//??????
					d0 = get_num( fp );
					if(d0 < 1 || d0 > TRACK_MAX){
						err_flg=M_MUTE_ERR;
						goto music_read_999;
					}
					play_track_flg[ d0-1 ] = 0;	//???????????
				}
			}
			err_line--;		//??㎝??????????
			continue;
		}

		if( d0 == 'o' ){		//???叉?
			if(!num_check( fp )){		//???????
				err_flg=M_TOCOMMAND_ERR;
				goto music_read_999;
			}
			o_tempo = get_num( fp );		//?????
			if((d0 = fgetc( fp )) == EOF){		//??
				err_flg=M_TOCOMMAND_ERR;
				goto music_read_999;
			}
			if(d0 != ')'){
				err_flg=M_TOCOMMAND_ERR;
				goto music_read_999;
			}
			err_line--;		//??㎝??????????
			continue;
		}

		if( d0 != 't' ){		//?????
			err_flg=M_TRACK_ERR;
			goto music_read_999;
		}
		if((d0 = f_get( fp )) == EOF)		//????吻???
			break;		//?????Ψ?
		if( d0 < '0' || d0> '9' ){		//?????
			err_flg=M_TRACK_ERR;
			goto music_read_999;
		}
		fseek(fp,-1,SEEK_CUR);		//??????
		ch = get_num( fp );
		if((d0 = f_get( fp )) == EOF)		//????吻???
			break;		//?????Ψ?
		if( d0 != ')' ){		//?????
			err_flg=M_TRACK_ERR;
			goto music_read_999;
		}

		if(ch < 1 || ch > TRACK_MAX){
			err_flg=M_TRACK_ERR;
			goto music_read_999;
		}

		ch--;		//?????叉
		play_track_flg[ ch ] = 1;	//

		if(o_tempo!=0 && ch_p[ch]==0){		//???叉?????
			step[ch][ch_p[ch]++] = MUSIC_TEMPO;		//「???????????
#if MODE98
			step[ch][ch_p[ch]++] = (int)((float)120 / (float)o_tempo * (float)12600)*2;
#else
			step[ch][ch_p[ch]++] = (int)((float)120 / (float)o_tempo * (float)12600);
#endif
		}

		//??????????  ???????　叉  ??????????

music_read_100:
		if((d0 = f_get( fp )) == EOF)		//??????
			break;		//?????Ψ?
		switch(d0){
		case '\n':		//??
			break;
		case '/':		//?????
			while(1){			//????????
				d0 = f_get( fp );
				if(d0 == '\n' || d0 == EOF )
					break;
			}
			break;
		case '\'':		//????
			int waon_step,waon_note[8+1],waon_cnt,octave,waon_delay_point;
			waon_step = waon_cnt = 0;
			octave = ch_oct[ch];

			while(1){
				d0 = f_get( fp );		//?????
				if( d0 >= '0' && d0<= '9' ){		//????????
					err_flg=M_WAON_ERR;				//???
					goto music_read_999;
				}
				if(d0 =='<'){		//???????
					octave++;
					continue;
				}
				if(d0 =='>'){		//????????
					octave--;
					if(octave < 0){		//????????
						err_flg=M_NOTE_ERR;		//???
						goto music_read_999;	//
					}
					continue;
				}
				if(d0 =='\''){		//?????
					break;
				}
				switch(d0){
				case 'c':		//??
				case 'd':		//??
				case 'e':		//??
				case 'f':		//??
				case 'g':		//??
				case 'a':		//??
				case 'b':		//??
					d0-='a';
					d1 = note_change_tbl[d0];
					d0 = f_get( fp );		//?????
					if(d0 == '+')
						d1++;
					else if(d0 == '-')
						d1--;
					else fseek(fp,-1,SEEK_CUR);		//??????

					d0 = octave * 12 + d1 +1;	//???吻???????
					if(d0 < 1 || d0 > 62){		//
						err_flg=M_NOTE_ERR;		//???
						goto music_read_999;	//
					}
					waon_note[ waon_cnt++ ] = d0;	//???吻????

					d0 = f_get( fp );		//?????
					fseek(fp,-1,SEEK_CUR);		//??????
					if( d0 < '0' || d0> '9' ){		//???????
						if( d0 != '*' ){		//???????????
							break;
						}
					}
					waon_step = get_step( fp, ch );		//?????????
					if(waon_step == -1){		//?????
						err_flg=M_WAON_ERR;				//???
						goto music_read_999;
					}
					break;
				default:
					err_flg=M_WAON_ERR;				//???
					goto music_read_999;
				}
			}
			if(waon_cnt > 8 || waon_cnt == 0){		//????????
				err_flg=M_WAON_ERR;				//???
				goto music_read_999;
			}

			if(num_check( fp )){		//????
				waon_step = get_num( fp );		//??????????
			}
			step[ch][ch_p[ch]++] = MUSIC_WAON;		//?????
			step[ch][ch_p[ch]++] = waon_cnt;		//??????
			waon_delay_point = ch_p[ch];		//??????????卒㎏
			ch_p[ch]++;		//??　叉??

			for(d7=0;d7<waon_cnt;d7++){
				step[ch][ch_p[ch]++] = waon_note[d7];		//???吻????
			}
			if(waon_step){		//????????叉?????
				d0 = waon_step;		//??????????
			} else {
				d0 = step_def[ch];	//??????????
			}
			step[ch][ch_p[ch]++] = d0;		//??????????
			d1 = f_get( fp );		//????
			if(d1 == '&'){
				step[ch][ch_p[ch]++] = -1;	//?????????
			} else {
				fseek(fp,-1,SEEK_CUR);		//??????
				step[ch][ch_p[ch]++] = d0 * q_com[ch] / 8;	//?????????
			}

			d0 = f_get( fp );		//????叉????
			if(d0 == ','){
				if(num_check( fp )){		//????
					waon_delay[ch] = get_num( fp );		//????π???
				} else {		//???????
					err_flg=M_WAON_ERR;				//???
					goto music_read_999;
				}
			} else {		//????叉????
				fseek(fp,-1,SEEK_CUR);		//??????
			}
			step[ch][waon_delay_point] = waon_delay[ch];		//???????
			goto music_read_100;
		case 'l':		//???
			d0 = f_get( fp );		//???π?
			if( d0 >= '0' && d0<= '9' ){		//???
				fseek(fp,-1,SEEK_CUR);		//??????
				d0 = 192 / get_num( fp );
				if(d0 > 192 || d0 == 0){
					err_flg=M_LCOMMAND_ERR;
					goto music_read_999;
				}
				step_def[ ch ] = d0;
				goto music_read_100;
			}
			if( d0 != '*' ){		//‧▋???
				err_flg=M_LCOMMAND_ERR;
				goto music_read_999;
			}
			d0 = f_get( fp );		//???π?
			if( d0 >= '0' && d0<= '9' ){		//???
				fseek(fp,-1,SEEK_CUR);		//??????
				step_def[ ch ] = get_num( fp );
				goto music_read_100;
			}
			err_flg=M_LCOMMAND_ERR;
			goto music_read_999;
		case 'r':		//??
			step[ch][ch_p[ch]++]=MUSIC_R;
			d0 = get_step( fp, ch );		//?????????
			if(d0 == -1){		//????
				err_flg=M_STEP_ERR;		//??????
				goto music_read_999;
			}
			step[ch][ch_p[ch]++] = d0;		//??????????
			d1 = f_get( fp );		//????
			if(d1 == '&'){
				step[ch][ch_p[ch]++] = -1;	//?????????
			} else {
				fseek(fp,-1,SEEK_CUR);		//??????
				step[ch][ch_p[ch]++] = d0 * q_com[ch] / 8;	//?????????
			}
			goto music_read_100;
		case 'o':		//?????叉?
			if(!num_check( fp )){		//???????
				err_flg=M_OCOMMAND_ERR;		//???
				goto music_read_999;
			}
			ch_oct[ch] = get_num( fp );
			if(ch_oct[ch]<0 || ch_oct[ch]>5){		//吳????
				err_flg=M_OCOMMAND_ERR;		//???
				goto music_read_999;
			}
			goto music_read_100;
		case '<':		//??????
			ch_oct[ch]++;
			goto music_read_100;
		case '>':		//???????
			ch_oct[ch]--;
			if(ch_oct[ch] < 0){		//????????
				err_flg=M_NOTE_ERR;		//???
				goto music_read_999;	//
			}
			goto music_read_100;
		case 'q':		//???????
				if(!num_check( fp )){		//???????
						err_flg=M_QCOMMAND_ERR;
						goto music_read_999;
				}
				q_com[ch] = get_num( fp );		//?????????
				if(q_com[ch] > 8 || q_com[ch] == 0){		//吳????
					err_flg=M_QCOMMAND_ERR;
					goto music_read_999;
				}
			goto music_read_100;
		case 'c':		//??
		case 'd':		//??
		case 'e':		//??
		case 'f':		//??
		case 'g':		//??
		case 'a':		//??
		case 'b':		//??
//			if(ch >= 6){
				d0-='a';
				d1 = note_change_tbl[d0];
				d0 = f_get( fp );		//?????
				if(d0 == '+')
					d1++;
				else if(d0 == '-')
					d1--;
				else fseek(fp,-1,SEEK_CUR);		//??????

				if(ch_oct[ch] >= 5){			//???吻??????
					if(d1 >= 2){			//
						err_flg=M_NOTE_ERR;	//??
						goto music_read_999;	//
					}
				}
#if 0
				step[ch][ch_p[ch]++] = MUSIC_FREQ;
				d1 = freq_tbl[d1+1];
				if(ch_oct[ch] > 0){
					d0 = ch_oct[ch];
					d0 = 1 << d0;
					d1 *= d0;
				} else if(ch_oct[ch] < 0){
					d0 = -ch_oct[ch];
					d0 = 1 << d0;
					d1 /= d0;
				}
#endif
				d0 = ch_oct[ch] * 12 + d1 + 1;	//???吻???????
				if(d0 < 1 || d0 > 62){			//
					err_flg=M_NOTE_ERR;			//???
					goto music_read_999;		//
				}
				step[ch][ch_p[ch]++] = d0;	//???吻????
//			}
			d0 = get_step( fp, ch );		//?????????
			if(d0 == -1){		//????
				err_flg=M_STEP_ERR;		//??????
				goto music_read_999;
			}
			step[ch][ch_p[ch]++] = d0;		//??????????
			d1 = f_get( fp );		//????
			if(d1 == '&'){
				step[ch][ch_p[ch]++] = -1;	//?????????
			} else {
				fseek(fp,-1,SEEK_CUR);		//??????
				step[ch][ch_p[ch]++] = d0 * q_com[ch] / 8;	//?????????
			}
			goto music_read_100;
		case 't':		//???
			d0 = f_get( fp );		//?????
			if( d0 < '0' || d0> '9' ){		//???
				err_flg=M_TCOMMAND_ERR;
				goto music_read_999;
			}
			fseek(fp,-1,SEEK_CUR);		//??????
			d0 = get_num( fp );
			step[ch][ch_p[ch]++] = MUSIC_TEMPO;
#if MODE98
			step[ch][ch_p[ch]++] = (int)((float)120 / (float)d0 * (float)12600)*2;
#else
			step[ch][ch_p[ch]++] = (int)((float)120 / (float)d0 * (float)12600);
#endif
			goto music_read_100;
		case 'v':		//?????
			d0 = f_get( fp );		//????
			if( d0 < '0' || d0> '9' ){		//???
				err_flg=M_VCOMMAND_ERR;
				goto music_read_999;
			}
			fseek(fp,-1,SEEK_CUR);		//??????
			d0 = get_num( fp );
			if(d0>15){		//???????
				err_flg=M_VCOMMAND_ERR;
				goto music_read_999;
			}
			step[ch][ch_p[ch]++]=MUSIC_VOL;
			if( d0 ){
					d0 = 15 - d0;
//					d0 *= d0;
//					d0 = (int)((float)d0 * 39.0625);
//					step[ch][ch_p[ch]++] = -d0;
					d6 = d0;
					d6 = d6*d6*d6;
					d6 = (long)(10000.0 / ( 16.0 * 16.0 * 16.0 ) * d6 );
					step[ch][ch_p[ch]++] = -d6;
			} else {
				step[ch][ch_p[ch]++] = -10000;
			}
			goto music_read_100;
		case '@':		//?????
			d0 = f_get( fp );		//??????

			if( d0 >= '0' && d0<= '9' ){		//???
				fseek(fp,-1,SEEK_CUR);		//??????
				step[ch][ch_p[ch]++] = MUSIC_NEIRO_NO;		//?吻????
				step[ch][ch_p[ch]++] = get_num( fp ) - 1;
				goto music_read_100;
			}

			switch(d0){
			case 'v':		//?????
				d0 = f_get( fp );		//????
				if( d0 < '0' || d0> '9' ){		//???
					err_flg=M_V2COMMAND_ERR;
					goto music_read_999;
				}
				fseek(fp,-1,SEEK_CUR);		//??????
				d0 = get_num( fp );
				if(d0>127){		//???????
					err_flg=M_V2COMMAND_ERR;
					goto music_read_999;
				}
				step[ch][ch_p[ch]++]=MUSIC_VOL;
#if 0
				if( d0 ){
					d0 = 127 - d0;
//					d0 *= d0;
//					d0 = (int)((float)d0 * 0.61);
//					step[ch][ch_p[ch]++] = -d0;
					d6 = d0;
					d6 = d6*d6*d6;
					d6 = (long)(10000.0 / ( 128.0 * 128.0 * 128.0 ) * d6 );
					step[ch][ch_p[ch]++] = -d6;
				} else {
					step[ch][ch_p[ch]++] = -10000;
				}
#else
//				d6=(long)(LOG(100,127)*10000) - (long)(LOG(100,(double)(d0))*10000);
//				step[ch][ch_p[ch]++] = -d6;

				step[ch][ch_p[ch]++] = d0;

#endif
				goto music_read_100;
			case 'h':		//??
				if(!num_check( fp )){		//???????
					err_flg=M_H2COMMAND_ERR;
					goto music_read_999;
				}
				d0 = get_num( fp );
				step[ch][ch_p[ch]++] = MUSIC_LFO_H;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 'm':		//??
				if(!num_check( fp )){		//???????
					err_flg=M_M2COMMAND_ERR;
					goto music_read_999;
				}
				d0 = get_num( fp );
				if(d0 > 127){		//吳????
					err_flg=M_M2COMMAND_ERR;
					goto music_read_999;
				}
				step[ch][ch_p[ch]++] = MUSIC_LFO_M;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 's':		//??
				if(!num_check2( fp )){		//???????
					err_flg=M_S2COMMAND_ERR;
					goto music_read_999;
				}
				d0 = get_num2( fp );
				step[ch][ch_p[ch]++] = MUSIC_LFO_S;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 'p':		//?????
				d0 = f_get( fp );		//?去???
				if( d0 < '0' || d0 > '9' ){		//???
					err_flg=M_P2COMMAND_ERR;
					goto music_read_999;
				}
				fseek(fp,-1,SEEK_CUR);		//??????
				d0 = get_num( fp );
				if( d0 > 127 ){		//吳??????
					err_flg=M_P2COMMAND_ERR;
					goto music_read_999;
				}
				step[ch][ch_p[ch]++]=MUSIC_PAN;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 'f':		//??
				if(!num_check2( fp )){		//???????
					err_flg=M_F2COMMAND_ERR;
					goto music_read_999;
				}
				d0 = get_num2( fp );
				step[ch][ch_p[ch]++] = MUSIC_FREQ;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 'k':		//?????
				if(!num_check2( fp )){		//???????
					err_flg=M_K2COMMAND_ERR;
					goto music_read_999;
				}
				d0 = get_num2( fp );
				if(d0 > 64 || d0 < -64){		//吳????
					err_flg=M_K2COMMAND_ERR;
					goto music_read_999;
				}
				step[ch][ch_p[ch]++] = MUSIC_DETUNE;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 'd':		//???????
				d0 = f_get( fp );		//???????
				if( d0 < '0' || d0> '9' ){		//???
					err_flg=M_D2COMMAND_ERR;
					goto music_read_999;
				}
				fseek(fp,-1,SEEK_CUR);		//??????
				d0 = get_num( fp );
				step[ch][ch_p[ch]++] = MUSIC_PEDAL;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			case 'w':		//???????
				d0 = f_get( fp );		//???????
				if( d0 < '0' || d0> '9' ){		//???
					err_flg=M_W2COMMAND_ERR;
					goto music_read_999;
				}
				fseek(fp,-1,SEEK_CUR);		//??????
				d0 = get_num( fp );
				step[ch][ch_p[ch]++] = MUSIC_WAVE;
				step[ch][ch_p[ch]++] = d0;
				goto music_read_100;
			default:
				err_flg=M_2COMMAND_ERR;
				goto music_read_999;
			}
		case '[':		//???????
			d0 = f_get( fp );		//??????
			switch(d0){
			case 'l':
				d0 = f_get( fp );		//??????
				if(d0 != 'o'){
					err_flg=M_KAKKO_ERR;
					goto music_read_999;
				}
				d0 = f_get( fp );		//??????
				if(d0 != 'o'){
					err_flg=M_KAKKO_ERR;
					goto music_read_999;
				}
				d0 = f_get( fp );		//??????
				if(d0 != 'p'){
					err_flg=M_KAKKO_ERR;
					goto music_read_999;
				}
				d0 = f_get( fp );		//??????
				if(d0 != ']'){
					err_flg=M_KAKKO_ERR;
					goto music_read_999;
				}
				step[ch][ch_p[ch]++]=MUSIC_LOOP;
				step[ch][ch_p[ch]++]=do_p[ch];
				goto music_read_100;
			case 'd':		//????ㄅ
				d0 = f_get( fp );		//??????
				if(d0 != 'o'){
					err_flg=M_KAKKO_ERR;
					goto music_read_999;
				}
				d0 = f_get( fp );		//??????
				if(d0 != ']'){
					err_flg=M_KAKKO_ERR;
					goto music_read_999;
				}
				do_p[ch]=ch_p[ch];		//??！???
				goto music_read_100;
			default:
					err_flg=M_KAKKO_ERR;
				goto music_read_999;
			}
		case '|':		//??刺?????
			d0 = f_get( fp );		//??????
			if(d0 == ':'){		//??刺???
				ch_kuri_lv[ch]++;		//????????
				if(ch_kuri_lv[ch] == 8){		//??刺?????
					err_flg=M_KURI_ERR;
					goto music_read_999;
				}
				step[ch][ch_p[ch]++] = MUSIC_KURI_START;
				if(num_check( fp )){		//???
					step[ch][ch_p[ch]++] = get_num( fp );
				} else {
					step[ch][ch_p[ch]++] = 2;
				}
				ch_ret[ch_kuri_lv[ch]][ ch ] = ch_p[ch];
			} else {
				fseek(fp,-1,SEEK_CUR);		//??????
				step[ch][ch_p[ch]++]=MUSIC_KURI_SKIP;
				ch_skip[ch_kuri_lv[ch]][ ch ] = ch_p[ch];
				ch_p[ch]++;
			}
			goto music_read_100;

		case ':':		//??刺??????
			d0 = f_get( fp );		//??????
			if(d0 != '|'){		//??刺???
				err_flg=M_KURI_ERR;
				goto music_read_999;
			}
			step[ch][ch_p[ch]++] = MUSIC_KURI_RET;
			step[ch][ch_p[ch]++] = ch_ret[ch_kuri_lv[ch]][ ch ];		//??
			if(ch_skip[ch_kuri_lv[ch]][ ch ] >= 0){
				step[ch][ch_skip[ch_kuri_lv[ch]][ ch ]] = ch_p[ch];	//Α?
				ch_skip[ch_kuri_lv[ch]][ ch ] = -1;
			}
			ch_kuri_lv[ch]--;		//????????
			goto music_read_100;

		default:		//??????
			err_flg=M_MML_ERR;
			goto music_read_999;
		}
	}
	for(d7=0;d7<TRACK_MAX;d7++){
		step[d7][ch_p[d7]]=MUSIC_END;
	}
	err_flg=0;

music_read_999:
	fclose( fp );
	if(err_flg){
		sprintf_s( moji ,"菴 %d 俴 %s" ,err_line ,err_msg[err_flg] );
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, moji, "Error", MB_OK);
#endif
	} else {
		for(d7=0;d7<TRACK_MAX;d7++){
			if(ch_kuri_lv[d7]!=-1){
				err_flg=M_KURISU_ERR;
				sprintf_s( moji ,"菴 %d 秞寢 %s" ,d7+1 ,err_msg[err_flg] );
#ifdef _STONDEBUG_
				MessageBoxNew(hWnd, moji, "Error", MB_OK);
#endif
				break;
			}
		}
	}
	return err_flg;
}
/*-------------------------------------------
	??????????
--------------------------------------------*/
void music_init( void )
{
	int d7,d5;
	for(d7=0;d7<VOICE_MAX;d7++){
		voice[ d7 ].name = 0;
		voice[ d7 ].tone_no = -1;		//???????????吻?
		voice[ d7 ].loop_flg = 0;
	}
	for(d7=0;d7<TRACK_MAX;d7++){
		step_time[ d7 ] = 0;
		gate_time[ d7 ] = 0;
		step_point[ d7 ] = 0;
		play_flg[ d7 ] = 1;
		play_kuri_lv[ d7 ] = -1;
		dataToneNo[ d7 ] = 0;		//????????????吻?
		dataVolume[ d7 ] = 0;
		dataPan[ d7 ] = 0;
		dataFrequency[ d7 ] = 0;
		tie_flg[ d7 ] = 0;
		for(d5=0;d5<8;d5++)
			play_use_task[ d7 ][ d5 ] = -1;		//????吻????
		play_use_task_cnt[ d7 ] = 0;
		play_detune[ d7 ] = 0;
		play_lfo_h[ d7 ] = 0;
		play_lfo_m[ d7 ] = 0;
		play_lfo_s[ d7 ] = 0;
	}
	//??????
	for(d7=0;d7<MAX_ENV;d7++){
		env_tbl[d7].voice_address = -1;
		env_tbl[d7].count = 0;
	}
#define PAN_POINT	5000
	//???????
	int y;
	double d6;		//?
	for(d7=64,d6=127;d6>=0;d6--){
		if(!((int)d6&1))
			continue;
		y=(int)(LOG(100,127)*PAN_POINT) - (int)(LOG(100,d6)*PAN_POINT);
		if(y>PAN_POINT)
			y=10000;
		panpot_tbl[d7++]=y;
	}
	for(d7=64,d6=127;d6>=0;d6--){
		if(!((int)d6&1))
			continue;
		y=(int)(LOG(100,127)*PAN_POINT) - (int)(LOG(100,d6)*PAN_POINT);
		if(y>PAN_POINT)
			y=10000;
		panpot_tbl[d7--]=-y;
	}
	d6=panpot_tbl[0]=-10000;
	d6=panpot_tbl[127]=10000;
	//??????????
	for(d6=127;d6>=0;d6--){
		y=(int)(LOG(100,127)*10000) - (int)(LOG(100,d6)*10000);
		if(y>10000)
			y=10000;
		volume_tbl[(int)d6]=-y;
	}
	//??????????
	for(d7=0;d7<12*5+5;d7++)
		detune_tbl[d7]=freq_tbl[d7+1]-freq_tbl[d7];

//	play_bgm( 2);

}

/*-------------------------------------------
	??κ??
--------------------------------------------*/
void play_stop( void )
{
	int d7;
	for(d7=0;d7<VOICE_MAX;d7++){
		pDSData[d7]->Stop();
		pDSData[d7]->SetCurrentPosition(0);
//		play_flg[ d7 ] = 0;
	}
	pause_flg=1;
}
void stop_voice(int ch);
/*-------------------------------------------
	????
--------------------------------------------*/
void play_start( void )
{
	int d7;
	for(d7=0;d7<VOICE_MAX;d7++){
		pDSData[d7]->Stop();
		pDSData[d7]->SetCurrentPosition(0);
	}
	music_init();		//????????		
}

//??????????  ?????  ??????????
void voice_lfo( int a0 )
{
	long d0;
	if(!voice[ a0 ].lfo_m){		//???????
		return;
	}
	if(!voice[ a0 ].lfo_s){		//???????
		return;
	}
	if(voice[ a0 ].lfo_h){			//?????????
		voice[ a0 ].lfo_h--;
		return;
	}

	voice[ a0 ].lfo_s_cnt += voice[ a0 ].lfo_s_ang;		//???????

//	if(voice[ a0 ].lfo_s_ang >= 0){		//????????
	if(voice[ a0 ].lfo_s_cnt >= 0){		//????????
		d0 = detune_tbl[ voice[ a0 ].note_no ] * voice[ a0 ].lfo_m / 128 * voice[ a0 ].lfo_s_cnt / abs(voice[ a0 ].lfo_s);
	} else {
		d0 = detune_tbl[ voice[ a0 ].note_no-1 ] * voice[ a0 ].lfo_m / 128 * voice[ a0 ].lfo_s_cnt / abs(voice[ a0 ].lfo_s);
	}
	pDSData[ a0 ]->SetFrequency((DWORD)(voice[ a0 ].freq + d0));			// ?????

	if(abs(voice[ a0 ].lfo_s_cnt) == abs(voice[ a0 ].lfo_s)){		//?刺???
		voice[ a0 ].lfo_s_ang *= -1;
	}
}

//??????????  ??????  ??????????
#define VOICE_KEY_ON		1
#define VOICE_NORMAL		2
#define VOICE_TIE_SET		10
#define VOICE_KEY_OFF		100
#define VOICE_KEY_OFF_FAST	110
#define VOICE_KEY_OFF_WAIT	200

void voice_action( int a0 )
{
	DWORD	Status = 0;

	switch(voice[ a0 ].name){
	case VOICE_KEY_ON:		//二????
		if(voice[ a0 ].delay_cnt){		//?????????
			voice[ a0 ].delay_cnt--;
			return;
		}
		pDSData[ a0 ]->SetVolume(volume_tbl[ voice[ a0 ].volume ]);		// ????????
		pDSData[ a0 ]->SetFrequency((DWORD)voice[ a0 ].freq);			// ?????
		pDSData[ a0 ]->SetPan(panpot_tbl[ voice[ a0 ].pan ]);			// ????????
		pDSData[ a0 ]->Play(0, 0, voice[ a0 ].loop_flg);				//????
		voice[ a0 ].name = VOICE_NORMAL;
		voice_lfo( a0 );		//??????
		break;
	case VOICE_NORMAL:		//二?????
		voice_lfo( a0 );		//??????
		break;
	case VOICE_TIE_SET:		//???????
		pDSData[ a0 ]->SetVolume(volume_tbl[ voice[ a0 ].volume ]);		// ????????
		pDSData[ a0 ]->SetFrequency((DWORD)voice[ a0 ].freq);		// ?????
		pDSData[ a0 ]->SetPan(panpot_tbl[ voice[ a0 ].pan ]);			// ????????
		voice[ a0 ].name = VOICE_NORMAL;
		break;
	case VOICE_KEY_OFF:		//二????
//		if( pDSData[ a0 ]->GetStatus( &Status ) == DS_OK ){
///		pDSData[ a0 ]->GetStatus( &Status );		//??????????
///		if( Status & DSBSTATUS_PLAYING || Status & DSBSTATUS_LOOPING ){		//????
			if(voice[ a0 ].volume){		//??????????
				voice[ a0 ].release_cnt++;		//?????阪?????
				if(voice[ a0 ].release_spd != voice[ a0 ].release_cnt){
					voice_lfo( a0 );		//??????
					break;		//???????兌??
				}
				voice[ a0 ].release_cnt = 0;		//?????阪??
				voice[ a0 ].volume--;		//????????
//				if(voice[ a0 ].volume < 0){		//??????????
//					voice[ a0 ].volume=0;		//?????????
//				}
				pDSData[ a0 ]->SetVolume(volume_tbl[ voice[ a0 ].volume ]);		// ?????阪?
				voice_lfo( a0 );		//??????
				break;		//????兌??
			}
///		}
		pDSData[ a0 ]->Stop();							//????
		pDSData[ a0 ]->SetCurrentPosition(0);			//
		voice[ a0 ].name = VOICE_KEY_OFF_WAIT;			//?????┴?
//		play_use_task[ voice[ a0 ].track_no	] = -1;		//
		break;
#if !VOICE_KILL_ON
	case VOICE_KEY_OFF_FAST:		//﹩?????
		pDSData[ a0 ]->Stop();					//????
		pDSData[ a0 ]->SetCurrentPosition(0);			//
		voice[ a0 ].name = 0;					//
		break;
#endif
	}
}
//??????????  ???????????  ??????????
void set_voice_param( int task_no, long note, int delay, int lfo_flg)
{
	voice[ task_no ].volume = dataVolume[ch];			// ????????
//	voice[ task_no ].freq = freq + dataFrequency[ch];	// ?????
	// ?????
	if(play_detune[ch]>0){		//???????????
		voice[ task_no ].freq = freq_tbl[ note ] + detune_tbl[ note ]*play_detune[ch]/64 + dataFrequency[ch];
	} else if(play_detune[ch]<0){		//????????????
		voice[ task_no ].freq = freq_tbl[ note ] + detune_tbl[ note-1 ]*play_detune[ch]/64 + dataFrequency[ch];
	} else {					//?????叉?????
		voice[ task_no ].freq = freq_tbl[ note ] + dataFrequency[ch];
	}
	voice[ task_no ].note_no = note;				// ???吻????
	voice[ task_no ].pan = dataPan[ch];				// ????????
	voice[ task_no ].release_spd = wave[ch];		// ?????????
	voice[ task_no ].release_cnt = 0;				//
	if(voice[ task_no ].tone_no == 12-1){				//??????
		voice[ task_no ].loop_flg = DSBPLAY_LOOPING;	//
	} else {
		voice[ task_no ].loop_flg = 0;					//
	}
	if(delay >= 0){								//?????????????
		voice[ task_no ].delay_cnt = delay;		//
	}
	if(lfo_flg){		//??????????
		voice[ task_no ].lfo_h = play_lfo_h[ ch ];					//??????
		voice[ task_no ].lfo_m = play_lfo_m[ ch ];					//
		if((voice[ task_no ].lfo_s = play_lfo_s[ ch ]) >= 0){		//
			voice[ task_no ].lfo_s_ang = 1;							//
		} else {
			voice[ task_no ].lfo_s_ang = -1;						//
		}
		voice[ task_no ].lfo_s_cnt = 0;								//
	}
}

//??????????  ???????  ??????????
static int voice_seek_point=0;
int	set_tone( int tone, long note, int use_task_cnt, int delay)
{
	int d7,d1=0;

	for(d7=0;d7<VOICE_MAX;d7++){		//???????Π?
//		if( voice[ d7 ].name == 0){		//???
		if( voice[ voice_seek_point ].name == 0){		//???
			d1=1;										//??????
			break;
		}
		voice_seek_point++;
		voice_seek_point&=VOICE_MAX-1;
	}
	if(!d1){		//????????
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, "????", "Error", MB_OK);		//????兌??
#endif
		play_use_task[ch][use_task_cnt]=-1;		//??????
		return -1;
	}
	d7 = voice_seek_point;

	play_use_task[ch][use_task_cnt]=d7;		//????吻?卒㎏

	voice[ d7 ].name = VOICE_KEY_ON;			//二????

	if(voice[ d7 ].tone_no == tone)		//可??吻??
		goto set_tone_500;				//??????

	voice[ d7 ].tone_no = tone;		//?吻?卒㎏

	RELEASE(pDSData[d7]);

	if( pDSound->DuplicateSoundBuffer(pDSData_tone[tone],&pDSData[d7]) != DS_OK ){		//????
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, "葩秶汒秞buffer囮啖ㄐ", "Error", MB_OK);
#endif
		play_use_task[ch][use_task_cnt]=-1;		//??????
		return -1;
	}

set_tone_500:
	set_voice_param( d7, note, delay, 1 );		//???????????

	return 0;
}

//??????????  ﹩?????  ??????????
void stop_voice(void)
{
	for(int cnt=0;cnt<play_use_task_cnt[ch];cnt++){
		if(play_use_task[ch][cnt]!=-1){					//??????
#if VOICE_KILL_ON
			voice_kill_buf[voice_kill_point++]=play_use_task[ch][cnt];
#else
			voice[ play_use_task[ch][cnt] ].name = VOICE_KEY_OFF_FAST;		//????﹩?????
#endif
			play_use_task[ch][cnt]=-1;					//????
		}
	}
}

//??????????  ????????  ??????????
void gate(void)
{
	int next_command;
	DWORD	Status = 0;
	if(!play_flg[ ch ])		//于??
		return;				//????兌??

	gate_time[ch]--;		//???????
	if(!gate_time[ch]){		//??????
		if( !d_pedal[ch] ){		//????????
			if( !wave[ch] ){	//???????
				stop_voice();		//???????
			} else {
				for(int cnt=0;cnt<play_use_task_cnt[ch];cnt++){
					if(play_use_task[ch][cnt]!=-1){					//??????
						voice[ play_use_task[ch][cnt] ].name = VOICE_KEY_OFF;	//????????
					}
				}
			}
		}
	}

	step_time[ch]--;			//?????
	if(step_time[ch]>0){		//
		return;				//???????兌??
	} else {
		if(!tie_flg[ch]){		//???????
			if( !d_pedal[ch] ){		//????????
				if( !wave[ch] ){	//???????
					stop_voice();		//???????
				} else {
					for(int cnt=0;cnt<play_use_task_cnt[ch];cnt++){
						if(play_use_task[ch][cnt]!=-1){					//??????
							voice[ play_use_task[ch][cnt] ].name = VOICE_KEY_OFF;	//????????
						}
					}
				}
			}
		}

gate_100:
		next_command = step[ch][step_point[ch]++];		//??????

		if(next_command >= 0){		//???吻???
			if(play_use_task_cnt[ch]!=1 && tie_flg[ch]==1){		//??????????
				tie_flg[ch]=0;		//???
			}
			if(!tie_flg[ch]){		//???????
				stop_voice();		//??????于
				set_tone( dataToneNo[ch], next_command, 0, 0);	//Μ?????
				note_old[ch][0] = next_command;		//??????吻?卒㎏
			} else {
				for(int cnt=0;cnt<play_use_task_cnt[ch];cnt++){
					//???????
					voice[ play_use_task[ch][cnt] ].name = VOICE_TIE_SET;
					//???????????
					set_voice_param( play_use_task[ch][cnt], next_command, -1, 0 );
					note_old[ch][cnt] = next_command;		//??????吻?卒㎏
				}
			}
			step_time[ch] = step[ch][step_point[ch]++];		//???????
			gate_time[ch] = step[ch][step_point[ch]++];		//??????
			if(gate_time[ch]==-1){		//????
				tie_flg[ch]=1;
			} else {
				tie_flg[ch]=0;
			}
			play_use_task_cnt[ch] = 1;		//伶?????
			return;
		}
		if(next_command==MUSIC_WAON){		//????
			int waon_cnt,waon_delay,waon_delay_total=0;
			waon_cnt = step[ch][step_point[ch]++];		//?????
			waon_delay = step[ch][step_point[ch]++];		//????????
			if(waon_cnt != play_use_task_cnt[ch] && tie_flg[ch]==1){		//??????????
				tie_flg[ch]=0;		//???
			}
			if(!tie_flg[ch]){		//???????
				stop_voice();		//??????于
				for(int cnt=0;cnt<waon_cnt;cnt++){
					//??????
					set_tone( dataToneNo[ch], step[ch][step_point[ch]], cnt, waon_delay_total);
					note_old[ch][cnt] = step[ch][step_point[ch]++];		//??????吻?卒㎏
					waon_delay_total += waon_delay;
				}
			} else {
				for(int cnt=0;cnt<play_use_task_cnt[ch];cnt++){
					//???????
					voice[ play_use_task[ch][cnt] ].name = VOICE_TIE_SET;
					//???????????
					set_voice_param( play_use_task[ch][cnt], step[ch][step_point[ch]], -1, 0 );
					note_old[ch][cnt] = step[ch][step_point[ch]++];		//??????吻?卒㎏
				}
			}
			step_time[ch] = step[ch][step_point[ch]++];		//???????
			gate_time[ch] = step[ch][step_point[ch]++];		//??????
			if(gate_time[ch]==-1){		//????
				tie_flg[ch]=1;
			} else {
				tie_flg[ch]=0;
			}
			play_use_task_cnt[ch] = waon_cnt;		//伶?????
			return;
		}
		if(next_command==MUSIC_TEMPO){		//????
			tempo.LowPart = step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_NEIRO_NO){		//??▔??
//			change_neiro( ch, step[ch][step_point[ch]++] );
			dataToneNo[ch] = step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_LOOP){		//????
			step_point[ch] = step[ch][step_point[ch]];
			goto gate_100;
		}
		if(next_command==MUSIC_KURI_START){		//??刺???
			play_kuri_lv[ch]++;		//????????
			play_kuri_num[play_kuri_lv[ch]][ch] = step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_KURI_SKIP){		//??刺?Α?
			if(play_kuri_num[play_kuri_lv[ch]][ch] == 1){
				step_point[ch] = step[ch][step_point[ch]];
				play_kuri_lv[ch]--;		//????????
			} else {
				step_point[ch]++;
			}
			goto gate_100;
		}
		if(next_command==MUSIC_KURI_RET){		//??刺???
			play_kuri_num[play_kuri_lv[ch]][ch]--;
			if(play_kuri_num[play_kuri_lv[ch]][ch] > 0){
				step_point[ch] = step[ch][step_point[ch]];
			} else {
				step_point[ch]++;
				play_kuri_lv[ch]--;		//????????
			}
			goto gate_100;
		}
		if(next_command==MUSIC_R){		//??
			if(tie_flg[ch]){		//????
				for(int cnt=0;cnt<play_use_task_cnt[ch];cnt++){
					//???????
					voice[ play_use_task[ch][cnt] ].name = VOICE_TIE_SET;
					//???????????
					set_voice_param( play_use_task[ch][cnt], note_old[ch][cnt], -1, 0 );
				}
			}
			step_time[ch] = step[ch][step_point[ch]++];		//???????
			gate_time[ch] = step[ch][step_point[ch]++];		//??????
			if(gate_time[ch]==-1){		//????
				tie_flg[ch]=1;
			} else {
				tie_flg[ch]=0;
			}
			return;
		}
		if(next_command==MUSIC_LFO_H){		//??????
			play_lfo_h[ch]=step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_LFO_M){		//??????
			play_lfo_m[ch]=step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_LFO_S){		//??????
			play_lfo_s[ch]=step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_PEDAL){		//????????
			d_pedal[ch]=step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_WAVE){		//???????
			wave[ch]=step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_VOL){		//????
			dataVolume[ch]=step[ch][step_point[ch]++];
//			pDSData[play_use_task[ch]]->SetVolume(dataVolume[ch]);
			goto gate_100;
		}
		if(next_command==MUSIC_DETUNE){		//??????
			play_detune[ch] = step[ch][step_point[ch]++];
			goto gate_100;
		}
		if(next_command==MUSIC_FREQ){		//???
			dataFrequency[ch]=step[ch][step_point[ch]++];
//			pDSData[play_use_task[ch]]->SetFrequency((DWORD)dataFrequency[ch]);
			goto gate_100;
		}
		if(next_command==MUSIC_PAN){		//??????
			dataPan[ch]=step[ch][step_point[ch]++];
//			pDSData[play_use_task[ch]]->SetPan(dataPan[ch]);
			goto gate_100;
		}
		if(next_command==MUSIC_END){		//??
			play_flg[ ch ] = 0;
			return;
		}
	}
}

//void CALLBACK music( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime )
void music( void )
{
	int d7;
#if 1
#if THREAD_ON
	while(!thread_flg){
#if CLASS_UP
		//Sleep(1);		//??
#endif
#endif
		LARGE_INTEGER d0;
		QueryPerformanceCounter( &d0 );
		if((d_cnt.LowPart + tempo.LowPart) > d0.LowPart)
#if THREAD_ON
		{
			continue;
		}
#else
			return;
#endif
#if !DEBUG_ON
		if(abs(d0.LowPart - d_cnt.LowPart) > 3333333)
#if THREAD_ON
			continue;
#else
			return;
#endif
#endif
		d_cnt.LowPart += tempo.LowPart;
#endif
		if(!pause_flg){
#if VOICE_KILL_ON
			for( d7 = 0 ; d7 < voice_kill_point ; d7++ ){
				voice[ voice_kill_buf[ d7 ] ].name = 0;			//??????
				pDSData[ voice_kill_buf[ d7 ] ]->Stop();		//????
				pDSData[ voice_kill_buf[ d7 ] ]->SetCurrentPosition(0);	//
			}
			voice_kill_point=0;
#endif
			for( d7 = 0 ; d7 < VOICE_MAX ; d7++ ){
				if(voice[ d7 ].name){
					voice_action( d7 );
				}
			}
			for( ch = 0 ; ch < TRACK_MAX ; ch++ ){
				if(play_track_flg[ ch ])
					gate();
			}
		} else {

#if CLASS_UP
			//Sleep(8);		//????
#endif
			QueryPerformanceCounter( &d_cnt );
		}

#if 0		//--------------

#if CLASS_UP
#if MODE98
	if( tempo.LowPart > 10080*2 )		//?????????
#else
	if( tempo.LowPart > 10080 )		//?????????
#endif
		Sleep(2);		//??
	else
		Sleep(1);		//??
#endif

#endif		//--------------

#if THREAD_ON
	}
	thread_flg=2;
	ExitThread( TRUE );
#endif
	d7=0;
	return;
}

/*--------------------------------------------
	??????????
---------------------------------------------*/
int t_music_init()
{
	if( GetAsyncKeyState( 0x10 ) & 0x8000 ){		//?????????????
		dsound_flg = -1;
		return FALSE;
	}
	if(!InitDSound()){
		dsound_flg = -1;
		return FALSE;
	}
	music_init();		//????????

/*
//cary	黍秞氈
	if(!music_read()){
		play_start();
	} else {
		play_stop();
		dsound_flg = -1;
		return FALSE;
	}
	QueryPerformanceCounter(&d_cnt);
#if THREAD_ON
	static DWORD dwChildThreadId = 0;
	HANDLE hChildThred = CreateThread( NULL,
										16384,
										(LPTHREAD_START_ROUTINE)music,
//										(LPVOID)hWnd,
										NULL,
										NULL,
										&dwChildThreadId );
#if CLASS_UP
	SetThreadPriority(hChildThred,THREAD_PRIORITY_HIGHEST);
#endif
#endif
*/
	return TRUE;
}

/*--------------------------------------------
	????????
---------------------------------------------*/
void t_music_end()
{
	//?????????
	if(dsound_flg == -1)
		return;
#if THREAD_ON
//	thread_flg=1;
//	while(thread_flg!=2);
#endif
	EndApp();
}


#if 0
//??????????  ????  ??????????
//static voice_seek_point=0;
int play_se(int tone, int x, int y)
{
	int distance;
	//?????????
	if(dsound_flg == -1){
		return -1;
	}

	if(tone > TONE_MAX){		//???????
		return -1;
	}

	if(tone_tbl[ tone ].voice_place == -1){		//叉????
		return -1;
	}

	if(tone_tbl[ tone ].play_time){		//??????
		return 0;
	}

	if(voice[ voice_seek_point ].tone_no != tone){		//可??吻??????
		voice[ voice_seek_point ].tone_no = tone;		//?吻?卒㎏
		RELEASE(pDSData[voice_seek_point]);		//??
		//????
		if( pDSound->DuplicateSoundBuffer(pDSData_tone[tone_tbl[ tone ].voice_place],&pDSData[voice_seek_point]) != DS_OK ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "葩秶汒秞buffer囮啖ㄐ", "Error", MB_OK);
#endif
			return -1;
		}
	}

	if(stereo_flg == T_MUSIC_MONO){		//??????
		distance = 127 * tone_tbl[ tone ].voice_volume / 10;
		x = 64;
	} else {		//??????
		distance = abs(y - 240);		//??????火???
		y = abs(x - 320);		//??????火???
		if(distance < y)		//?????
			distance = y;		//

		distance = distance << 4;		//?????π?阪?
		distance /= 0x5a;				//

		if(distance >= 127)		//吳????
			return 0;			//?????

		x /= 5;			//?????π?阪?
		if(x < 0)		//
			x = 0;		//
		if(x > 127)		//
			x = 127;	//

		distance = (127 - distance) * tone_tbl[ tone ].voice_volume / 10;		//???????
	}

	pDSData[ voice_seek_point ]->SetVolume(volume_tbl[ distance ]);		// ????????
	pDSData[ voice_seek_point ]->SetPan(panpot_tbl[ x ]);			// ????????
	pDSData[ voice_seek_point ]->Play(0, 0, 0);						//????

	voice_seek_point = voice_seek_point++;		//??????
	voice_seek_point&=VOICE_MAX-1;				//
	tone_tbl[ tone ].play_time = 8;		//???????

	return 0;
}

#else

//???????????????  ??  ???????????????
HANDLE hEvent[3];	// ?????????
HANDLE  hThreadHandle;	// ?????????????
DWORD dwThreadID;	// ???????????
/*-------------------------------------------
	????????????????????
--------------------------------------------*/
// ???????????仔??﹨▍??
struct SoundData {
    LPDIRECTSOUNDBUFFER pDSBuffer;	// ????????????????
    HANDLE *phEvent;	// ??????二ρ?ㄅ????????????????????????
    DWORD dwEventNum;	// ??????
    DWORD dwStopEventNum;	// ?于??????????????????
    LPBYTE lpWaveData;	// ???????????
    DWORD dwWaveSize;	// ???????????????
    DWORD dwBlock_size;	// ??????????????????
} sd;

// ?????????
void Block_Copy(LPDIRECTSOUNDBUFFER lpBuffer,
    DWORD blockadd, DWORD blocksize,
    LPBYTE lpWave, LONG &waveAdd, LONG waveSize)
{
    LPBYTE lpBlockAdd1, lpBlockAdd2;
    DWORD blockSize1, blockSize2;
	LONG ws = waveSize - waveAdd;

    // ????????
    HRESULT hr = lpBuffer->Lock(blockadd, blocksize, (LPVOID*)&lpBlockAdd1, &blockSize1, (LPVOID*)&lpBlockAdd2, &blockSize2, 0);
    if(hr == DS_OK)
    {
        if(ws < (long)blockSize1)
        {
            CopyMemory(lpBlockAdd1, lpWave + waveAdd, ws);
			waveAdd = blockSize1 - ws;
            CopyMemory(lpBlockAdd1 + ws, lpWave + thread_loop_start, waveAdd);
            waveAdd += thread_loop_start;
        }
        else
        {
            CopyMemory(lpBlockAdd1, lpWave + waveAdd, blockSize1);
            waveAdd += blockSize1;
        }
        if(lpBlockAdd2)
        {
			ws = waveSize - waveAdd;
            if(ws < (long)blockSize2)
            {
                CopyMemory(lpBlockAdd1, lpWave + waveAdd, ws);
				waveAdd = blockSize2 - ws;
                CopyMemory(lpBlockAdd1 + ws, lpWave + thread_loop_start, waveAdd);
	            waveAdd += thread_loop_start;
            }
            else
            {
                CopyMemory(lpBlockAdd2, lpWave + waveAdd, blockSize2);
				waveAdd += blockSize2;
            }
        }
        lpBuffer->Unlock(lpBlockAdd1, blockSize1, lpBlockAdd2, blockSize2);
    }
}

static LONG wave_count = 0;
static LONG wave_address = 0;
static LONG buffer_address = 0;
volatile static int thread_start_flg = 0;

// ??????????
DWORD WINAPI MyThreadFunc(LPVOID param)
{
	LONG waveAdd = 0;
    // ??????????????????????
    Block_Copy(sd.pDSBuffer, 0, sd.dwBlock_size, sd.lpWaveData, waveAdd, sd.dwWaveSize);
	thread_start_flg = 1;
    while(TRUE)
    {
        DWORD i = WaitForMultipleObjects(sd.dwEventNum, sd.phEvent, FALSE, INFINITE);
		wave_count++;
        switch(i)
        {
            case WAIT_OBJECT_0 + 1:
                // ?吻???????????
				buffer_address = 0; wave_address = waveAdd;
                Block_Copy(sd.pDSBuffer, 0, sd.dwBlock_size, sd.lpWaveData, waveAdd, sd.dwWaveSize);
                break;
            case WAIT_OBJECT_0:
                // ?吻???????????
				buffer_address = sd.dwBlock_size; wave_address = waveAdd;
                Block_Copy(sd.pDSBuffer, sd.dwBlock_size, sd.dwBlock_size, sd.lpWaveData, waveAdd, sd.dwWaveSize);
                break;
            case WAIT_OBJECT_0 + 2:
                 // ?????????????
				exit_thread_flg = 1;
				ExitThread(TRUE);
            default:
                 // ?????????????
				exit_thread_flg = 1;
				ExitThread(TRUE);
        }
//		InvalidateRect(hwndApp, NULL, TRUE);
    }
	return 0L;
}

//
// ??????????台
//
// ?  ?ㄩ	pWfmtx ... ???????????
//			DataSize ... ????????
//			ppData ... 台????????????????
// ??πㄩ	TRUE  ... ?
//			FALSE ... ?
static BOOL dwSoundInit2( WAVEFORMATEX *pWfmtx, DWORD DataSize, LPDIRECTSOUNDBUFFER *ppData, HMMIO hmmio )
{
	// ?????????????????????
	WAVEFORMATEX pcmwf = *pWfmtx;
	DSBUFFERDESC  dsbufferdesc;
	HRESULT ret;

	//??????
	if(stream_flg == 1){
		RELEASE(pDSNotify);
		stream_flg = 0;
	}
	//??????????????
	memset( &dsbufferdesc , 0  , sizeof( DSBUFFERDESC ) );
	dsbufferdesc.dwSize = sizeof( DSBUFFERDESC );
	dsbufferdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2; 
//	dsbufferdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC | DSBCAPS_CTRLDEFAULT | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS; 
    dsbufferdesc.dwBufferBytes = DataSize;
    dsbufferdesc.lpwfxFormat   = pWfmtx;
    if ( pDSound->CreateSoundBuffer( &dsbufferdesc , ppData , NULL ) != DS_OK ){
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "膘蕾汒秞腔buffer囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}


	//??????????
	if( WaveDataRead( hmmio, &DataSize, pDSData_stream ) == FALSE  ){
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, "黍WAV紫囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}

	// ?????????????????????
//	WAVEFORMATEX pcmwf = *pWaveHeader;
	DSBUFFERDESC dsbdesc; 
	// DSBUFFERDESC?﹨▍?　叉
	ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
#if ACTIVE
	dsbdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS; 
#else
	dsbdesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | /*DSBCAPS_CTRLDEFAULT*/DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2; 
#endif
	// ?　叉??Wave??????????企????????
	dsbdesc.dwBufferBytes = 2 * pcmwf.nAvgBytesPerSec; 
	dsbdesc.lpwfxFormat = &pcmwf; 
	ret = pDSound->CreateSoundBuffer(&dsbdesc, &pDSData_tone[ TONE_MAX ], NULL); 
	if(ret != DS_OK) { 
	    // ????????
//		RELEASE(pDSound);
		RELEASE(pDSData_stream);
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "膘蕾汒秞腔buffer囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}

	// DirectSoundNotify??????????
	ret = pDSData_tone[ TONE_MAX ]->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&pDSNotify);
	if(ret != DS_OK)
	{
//		RELEASE(pDSound);
		RELEASE(pDSData_stream);
		RELEASE(pDSData_tone[ TONE_MAX ]);
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "膘蕾DirectSoundNotify囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
		//  ???
	}

	// ?????????????
	hEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	hEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	hEvent[2] = CreateEvent(NULL, FALSE, FALSE, NULL);

	// DirectSoundNotify???????　叉??
	// ?﹨▍????
	DSBPOSITIONNOTIFY  pn[3];
	pn[0].dwOffset = 0;	// ?企?
	pn[0].hEventNotify = hEvent[0];
	pn[1].dwOffset = 1 * pcmwf.nAvgBytesPerSec;	// ?企?
	pn[1].hEventNotify = hEvent[1];
	pn[2].dwOffset = DSBPN_OFFSETSTOP;	// ?于
	pn[2].hEventNotify = hEvent[2];	// ??????????????于??????????

	// 于?ㄅ　叉
	HRESULT hr;
	hr = pDSNotify->SetNotificationPositions(3, pn);
	if(hr != DS_OK)
	{
	    // 　叉?
		RELEASE(pDSNotify);
//		RELEASE(pDSound);
		RELEASE(pDSData_stream);
		RELEASE(pDSData_tone[ TONE_MAX ]);
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "扢隅DirectSoundNotify囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}

	stream_flg = 1;		//???????

	// ??????????????
	thread_start_flg = 0;

	// ?????????
	sd.pDSBuffer = pDSData_tone[ TONE_MAX ];	// ??????????DirectSoundBuffer??????
	sd.phEvent = hEvent;	// 4.4.????????????????
	sd.dwEventNum = 3;	// ????????????
	sd.dwStopEventNum = 2;	// ??????????????于?????
	sd.lpWaveData = pDSbuffData;	// ???Wave?????????　叉?
	sd.dwWaveSize = DataSize;	// ???Wave????????　叉?
	sd.dwBlock_size = pcmwf.nAvgBytesPerSec;	// 44.1kHz???????????

	// ??????????
	hThreadHandle = CreateThread(NULL, 0, MyThreadFunc, &sd, 0, &dwThreadID);
	if(hThreadHandle == NULL)
	{
#ifdef _STONDEBUG_
	    // ???????
#ifdef _STONDEBUG_
	    MessageBoxNew(hWnd, "膘蕾Thread囮啖ㄐ", "Error", MB_OK);
#endif
#endif
		return FALSE;
	}

	// ??????????┴?
	while(1){
		if(thread_start_flg)
			break;
	}

//	SetThreadPriority(hThreadHandle,THREAD_PRIORITY_HIGHEST);
//	SetThreadPriority(hThreadHandle,THREAD_PRIORITY_ABOVE_NORMAL);
//	pDSData_tone[ TONE_MAX ]->Play(0, 0, DSBPLAY_LOOPING);
	return TRUE;
}
//???????????????  ??  ???????????????

int t_music_bgm_no = -1;
char t_music_bgm_pitch[16]={
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

//aaa
//??????????  ????  ??????????
#define	BGM_AUTO_LOOP	1
T_MUSIC_BGM bgm_tbl[] = {
	{"data\\bgm\\sabgm_s0.wav",	110,0,0},			//00	????
	{"data\\bgm\\sabgm_s1.wav",	120,0,0},			//01	????
	{"data\\bgm\\sabgm_s1.wav",	105,2,0},			//02	???︻??代??
	{"data\\bgm\\sabgm_t1.wav",	127,2,0},			//03	?????代??
	{"data\\bgm\\sabgm_t0.wav",	95,2,0},			//04	??代??

	{"data\\bgm\\sabgm_b0.wav",	122,1,0},			//05	???·卯??！卡?
	{"data\\bgm\\sabgm_b1.wav",	122,2,0},			//06	???·卯??代??

	{"data\\bgm\\sabgm_d0.wav",	120,1,0},			//07	???叩??！卡?
	{"data\\bgm\\sabgm_d1.wav",	105,1,0},			//08	吝?叩???！卡?
	{"data\\bgm\\sabgm_d2.wav",	100,2,0},			//09	吝?叩???代??

	{"data\\bgm\\sabgm_f0.wav",	127,2,0},			//10	?????代??
	{"data\\bgm\\sabgm_f1.wav",	100,2,0},			//11	?????代??

	{"data\\bgm\\sabgm_b3.wav", 110,2,0},			//12	陔腔珨啜桵須
	{"data\\bgm\\sabgm_b2.wav", 110,2,0},			//13	陔腔boss桵須
	{"data\\bgm\\sabgm_b4.wav", 110,2,0},			//14	陔腔郔綴boss桵須

	{"data\\bgm\\sabgm_f2.wav",	110,2,0},			//15	畛菟湮翻

	{"data\\bgm\\sabgm_t2.wav",	110,2,0},			//16	畛菟埶
	{"data\\bgm\\sabgm_t3.wav",	110,2,0},			//17	華傑
	{"data\\bgm\\sabgm_t4.wav",	110,2,0},			//18	阨傑
	{"data\\bgm\\sabgm_t5.wav",	110,2,0},			//19	鳶傑
	{"data\\bgm\\sabgm_t6.wav",	110,2,0},			//20	瑞傑
	{"data\\bgm\\sabgm_t7.wav",	110,2,0},			//21	傭軯

#ifdef _NEWMUSICFILE6_0		   // Syu ADD 6.0 陔華芞秞氈
	{"data\\bgm\\sabgm_t8.wav",	110,2,0},			//22	漆菁韌貓
	{"data\\bgm\\sabgm_t9.wav",	110,2,0},			//23	漆菁埻
	{"data\\bgm\\sabgm_t10.wav", 110,2,0},			//24	漆菁桵須
#endif
	{"data\\bgm\\sabgm_t11.wav", 110,2,0},			//25
	{"data\\bgm\\sabgm_t12.wav", 110,2,0},			//26
#ifdef _NEW_MUISC_
	{"data\\bgm\\sabgm_s2.wav", 110,2,0},			//26
#endif
};

int play_bgm(int bgm_no)
{
	if (MuteFlag) return -1;



	HMMIO hmmio;			//????????
    WAVEFORMATEX	Wfmtx;			//WAVE??????????????
	DWORD			DataSize;		//?????????
	DWORD			WaveSize;		//Wave????????

//	bgm_no = 0;
	//cary 2001 7 17
	if( bgm_no < 0)
		bgm_no = 0;

	//??????????
	t_music_fade_flg = 0;

	if(dsound_flg == -1)
		return -1;

	//???????????
	if(t_music_bgm_no != -1){
		//????于?
		stop_thread();
	}

	//????????????
//	if( (hmmio = mmioOpen(fname[bgm_no], NULL, MMIO_READ | MMIO_ALLOCBUF )) == NULL){
	if( (hmmio = mmioOpen(bgm_tbl[bgm_no].fname, NULL, MMIO_READ | MMIO_ALLOCBUF )) == NULL){
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, "湖羲WAV紫囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}
	//???????????
	if( WaveFormatRead( hmmio , &Wfmtx , &WaveSize ) == FALSE ){
#ifdef _STONDEBUG_
		MessageBoxNew(hWnd, "黍汒秞跡宒囮啖ㄐ", "Error", MB_OK);
#endif
		return FALSE;
	}
	DataSize = WaveSize;
	//??????????????
	if(bgm_tbl[bgm_no].loop_flg == 2){		//代??????
		thread_stop_flg = 1;		//?????
		//???????
#if BGM_AUTO_LOOP
		FILE *fp;
		int d0,d1,d2;
		fp = fopen( bgm_tbl[bgm_no].fname, "rb" );
		if( fp == NULL ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "湖羲汒秞紫囮啖ㄐ", "Error", MB_OK);
#endif
			return FALSE;
		}
		d2 = 8;							//??????????????
		d2 += 4;						//??????????????
		d2 += 4;						//????  ?????????
		d2 += 16;						//?????????
		fseek(fp,d2,SEEK_CUR);			//??????????去
		d0 = fgetc(fp);					//?????????????
		d0 |= fgetc(fp) << 8;			//
		d2 = 2;							//????????
		d2 += 12+DataSize;				//??????????  ?  ??????????
		if(DataSize&1){					//????????
			d2++;						//?????
		}
		d2 += 12*4;						//??????????
		fseek(fp,d2,SEEK_CUR);			//??????????去

		//?????ㄅ
		d1 = fgetc(fp);					//??????????
		d1 |= fgetc(fp) << 8;			//
		d1 |= fgetc(fp) << 16;			//
		d1 |= fgetc(fp) << 24;			//
		if( d1 < 0)
			thread_loop_start = 0;
		else
			thread_loop_start = d1 * d0;	//????ㄅ???
		//?????ㄅ
		d1 = fgetc(fp);
		d1 |= fgetc(fp) << 8;
		d1 |= fgetc(fp) << 16;
		d1 |= fgetc(fp) << 24;
//		thread_loop_end = d1 * d0;

		fclose(fp);
#else
		thread_loop_start = bgm_tbl[bgm_no].loop_point << 1;
#endif
//		thread_loop_start = bgm_tbl[bgm_no].loop_start;
//		thread_loop_end = bgm_tbl[bgm_no].loop_end;
//		if( dwSoundInit2( &Wfmtx, DataSize, &pDSData_tone[TONE_MAX], hmmio ) == FALSE ){
		if( dwSoundInit2( &Wfmtx, DataSize, &pDSData_stream, hmmio ) == FALSE ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "膘蕾汒秞腔buffer囮啖ㄐ", "Error", MB_OK);
#endif
			return FALSE;
		}
#if BGM_AUTO_LOOP
		if( d1 > 0)
			sd.dwWaveSize = d1 * d0;		//?????ㄅ????????????????
#endif
	} else {
		thread_stop_flg = 0;		//?????
		if( dwSoundInit( &Wfmtx, DataSize, &pDSData_tone[TONE_MAX] ) == FALSE ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "膘蕾汒秞腔buffer囮啖ㄐ", "Error", MB_OK);
#endif
			return FALSE;
		}
		//???????
		if( WaveDataRead( hmmio, &DataSize, pDSData_tone[TONE_MAX] ) == FALSE  ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "黍WAV紫腔訧蹋囮啖ㄐ", "Error", MB_OK);
#endif
			return FALSE;
		}
	}
	mmioClose( hmmio , 0 );

	//??????
	pDSData_tone[ TONE_MAX ]->SetFrequency((DWORD)freq_tbl[36+1+t_music_bgm_pitch[bgm_no]]);
	//?????
	pDSData_tone[ TONE_MAX ]->SetVolume(volume_tbl[ bgm_tbl[bgm_no].volume * t_music_bgm_volume / 15 ]);		// ????????

	//?????
	if(bgm_tbl[bgm_no].loop_flg){		//?????
		pDSData_tone[ TONE_MAX ]->Play(0, 0, DSBPLAY_LOOPING);
	} else {
		pDSData_tone[ TONE_MAX ]->Play(0, 0, 0);
	}

	//?????吻?卒㎏
	t_music_bgm_no = bgm_no;
	if(t_music_bgm_no < 0) t_music_bgm_no=0;
	// ????????
	NowTime = TimeGetTime();

	//????????
	t_music_playing_flg = 1;

	//?????吻?卒㎏
//	map_bgm_no = t_music_bgm_no;

	return TRUE;
}

//??????????  ????????  ??????????
void bgm_volume_change(void)
{
	if (MuteFlag) return;

	if(dsound_flg == -1)
		return;

	if(t_music_bgm_volume > 15){		//吳????
		t_music_bgm_volume = 15;
	}
	if(t_music_bgm_volume < 0){		//吳????
		t_music_bgm_volume = 0;
	}
	//?????
	if (pDSData_tone[TONE_MAX])
	{
		pDSData_tone[TONE_MAX]->SetVolume(volume_tbl[bgm_tbl[t_music_bgm_no].volume * t_music_bgm_volume / 15]);		// ????????
	}
}

//??????????  ?????????  ??????????
void set_gbm_pitch(void)
{
	if(dsound_flg == -1)
		return;

	//??????
	pDSData_tone[ TONE_MAX ]->SetFrequency((DWORD)freq_tbl[36+1+t_music_bgm_pitch[t_music_bgm_no]]);
}

int t_music_se_no = -1;
//??????????  ????  ??????????
//static voice_seek_point=0;
int play_se(int tone, int x, int y)
{
	if (MuteFlag) return -1;


	int d0;
	int total_level = 127;
	t_music_se_no = tone;

	if(dsound_flg == -1)
		return -1;

	if(tone > TONE_MAX){		//???????
		return -1;
	}

	if(tone_tbl[ tone ].voice_place == -1){		//叉????
		return -1;
	}

	if(tone_tbl[ tone ].play_time){		//??????
		return 0;
	}

	//〈???吻????
	d0 = voice[ voice_seek_point ].tone_no;
	if(voice[ voice_seek_point ].tone_no != tone){		//可??吻??????
		voice[ voice_seek_point ].tone_no = tone;		//?吻?卒㎏
		//???????????佃??????
		if(d0 != -2){
			RELEASE(pDSData[voice_seek_point]);		//??
		}
		//????
		if( pDSound->DuplicateSoundBuffer(pDSData_tone[tone_tbl[ tone ].voice_place],&pDSData[voice_seek_point]) != DS_OK ){
#ifdef _STONDEBUG_
			MessageBoxNew(hWnd, "葩秶汒秞buffer囮啖ㄐ", "Error", MB_OK);
#endif
			//???????????佃???????
			voice[ voice_seek_point ].tone_no = -2;
			return -1;
		}
	} else {
		pDSData[voice_seek_point]->SetCurrentPosition(0);	//?????????
	}
#if 0
	total_level = abs(y - 240);		//??????火???
	y = abs(x - 320) >> 1;		//??????火???
	if(total_level < y)		//?????
		total_level = y;		//

	total_level = total_level << 4;		//?????π?阪?
	total_level /= 0x5a;				//

	if(total_level >= 127)		//吳????
		return 0;			//?????

	total_level = 127 - total_level;				//

#endif

	if(stereo_flg == T_MUSIC_MONO){		//??????
		x = 64;
	} else {
		x /= 5;			//?????π?阪?
		if(x < 0)		//
			x = 0;		//
		if(x > 127)		//
			x = 127;	//
	}

	//??????
	total_level = 127 * tone_tbl[ tone ].voice_volume / 127;
//	total_level = total_level * tone_tbl[ tone ].voice_volume / 127;

	//?????????
	total_level = total_level * t_music_se_volume / 15;

	pDSData[ voice_seek_point ]->SetVolume(volume_tbl[ total_level ]);		// ????????
	pDSData[ voice_seek_point ]->SetPan(panpot_tbl[ x ]);			// ????????
	if(tone_tbl[ tone ].voice_note){		//???吻?叉??
		pDSData[ voice_seek_point ]->SetFrequency(
			(DWORD)freq_tbl[tone_tbl[ tone ].voice_note + tone_tbl[ tone ].voice_rate]);
	}

#if 0
	if(tone_tbl[ tone ].voice_loop == 0 ){		//???????
		pDSData[ voice_seek_point ]->Play(0, 0, 0);			//????
	} else {
		voice[ voice_seek_point ].loop_flg = 1;		//????
		pDSData[ voice_seek_point ]->Play(0, 0, DSBPLAY_LOOPING);		//????
	}
#else
	pDSData[ voice_seek_point ]->Play(0, 0, 0);			//????
#endif
	while(1){
		voice_seek_point++;		//??????
		voice_seek_point&=VOICE_MAX-1;				//
		if(voice[ voice_seek_point ].loop_flg == 0){		//??????????
			break;
		}
	}
	tone_tbl[ tone ].play_time = 5;		//???????

	return 0;
}
#endif

//??????????  ????  ??????????
//int play_environment_tone(int tone, int distance, int voice_address, int panpot, int new_flg)
int play_environment_tone(int tbl_no, int new_flg)
{
	int d0;
//	int total_level;

//	t_music_se_no = tone;

	if (MuteFlag) return -1;

	if(dsound_flg == -1)
		return -1;

	if(env_tbl[tbl_no].tone > TONE_MAX){		//???????
		return -1;
	}

	if(tone_tbl[ env_tbl[tbl_no].tone ].voice_place == -1){		//叉????
		return -1;
	}

//	if(tone_tbl[ env_tbl[tbl_no].tone ].play_time){		//??????
//		return 0;
//	}

	env_tbl[tbl_no].distance = env_tbl[tbl_no].distance << 4;		//?????π?阪?
	env_tbl[tbl_no].distance /= 0x6a;				//

//	if(env_tbl[tbl_no].distance >= 127)				//吳????
//		return 0;					//?????

	if(env_tbl[tbl_no].distance >= 127)				//吳????
		env_tbl[tbl_no].distance = 127;					//?????

	if(new_flg){		//???
		//〈???吻????
		d0 = voice[ env_tbl[tbl_no].voice_address ].tone_no;
		if(voice[ env_tbl[tbl_no].voice_address ].tone_no != env_tbl[tbl_no].tone){		//可??吻??????
			voice[ env_tbl[tbl_no].voice_address ].tone_no = env_tbl[tbl_no].tone;		//?吻?卒㎏
			//???????????佃??????
			if(d0 != -2){
				RELEASE(pDSData[env_tbl[tbl_no].voice_address]);		//??
			}
			//????
			if( pDSound->DuplicateSoundBuffer(pDSData_tone[tone_tbl[ env_tbl[tbl_no].tone ].voice_place],&pDSData[env_tbl[tbl_no].voice_address]) != DS_OK ){
#ifdef _STONDEBUG_
				MessageBoxNew(hWnd, "葩秶汒秞buffer囮啖ㄐ", "Error", MB_OK);
#endif
				//???????????佃???????
				voice[ env_tbl[tbl_no].voice_address ].tone_no = -2;
				return -1;
			}
		}
	}

	//??????
//	total_level = (127 - env_tbl[tbl_no].volume) * tone_tbl[ env_tbl[tbl_no].tone ].voice_volume / 127;

	//?????????
//	total_level = total_level * t_music_se_volume / 15;

	//??????阪?????
	if(env_tbl[tbl_no].volume != env_tbl[tbl_no].volume_old){
		// ????????
		pDSData[ env_tbl[tbl_no].voice_address ]->SetVolume(volume_tbl[ env_tbl[tbl_no].volume ]);
		env_tbl[tbl_no].volume_old = env_tbl[tbl_no].volume;
	}

	//??????阪?????
	if(env_tbl[tbl_no].panpot != env_tbl[tbl_no].panpot_old){
		// ????????
		pDSData[ env_tbl[tbl_no].voice_address ]->SetPan(panpot_tbl[ env_tbl[tbl_no].panpot ]);
		env_tbl[tbl_no].panpot_old = env_tbl[tbl_no].panpot;
	}

	//???吻?叉??
	if(tone_tbl[ env_tbl[tbl_no].tone ].voice_note){
		pDSData[ env_tbl[tbl_no].voice_address ]->SetFrequency(
			(DWORD)freq_tbl[tone_tbl[ env_tbl[tbl_no].tone ].voice_note + tone_tbl[ env_tbl[tbl_no].tone ].voice_rate]);
	}

	if(new_flg){		//???
		pDSData[ env_tbl[tbl_no].voice_address ]->SetCurrentPosition(0);	//
		if(tone_tbl[ env_tbl[tbl_no].tone ].voice_loop == 0 ){		//???????
			pDSData[ env_tbl[tbl_no].voice_address ]->Play(0, 0, 0);			//????
		} else {
			voice[ env_tbl[tbl_no].voice_address ].loop_flg = 1;		//????
			pDSData[ env_tbl[tbl_no].voice_address ]->Play(0, 0, DSBPLAY_LOOPING);		//????
		}

		while(1){
			voice_seek_point++;		//??????
			voice_seek_point&=VOICE_MAX-1;				//
			if(voice[ voice_seek_point ].loop_flg == 0){		//??????????
				break;
			}
		}
	}
//	tone_tbl[ env_tbl[tbl_no].tone ].play_time = 8;		//???????

	return 0;
}

//??????????  ??????????  ??????????
void play_map_bgm_check(void)
{
	switch(map_bgm_vct_no){
	case 0:		//???阪?????
		if(draw_map_bgm_flg){
			if(t_music_bgm_no != map_bgm_no){		//???阪???
				map_bgm_vct_no++;		//?????????
			}
		}
		draw_map_bgm_flg = 0;
		break;

	case 1:		//???????????
		fade_out_bgm();
		map_bgm_vct_no++;		//????????┴??
		break;

	case 2:		//???????????┴?
		if(t_music_fade_flg == 0){		//??????????
			map_bgm_vct_no++;		//?????
		}
		break;

	case 3:		//????
		play_bgm(map_bgm_no);		//????
		map_bgm_vct_no = 0;		//???阪??????
		break;
	}
}

//??????????  ???????  ??????????

int BakBgm = 0;
int play_map_bgm(int tone)
{
	// shan 2002/01/18
#ifdef _NEWMUSICFILE6_0		   // Syu ADD 6.0 陔華芞秞氈
	if( BakBgm != tone && tone >= 40 && tone <= 55 )
#else
	if( BakBgm != tone && tone >= 40 && tone <= 53 )
#endif
	{
		FILE *fp;
		char filename[255];
		char vTemp[32];
		BakBgm = tone;

		sprintf_s( filename, "map\\bgm%d.dat", sCharSide);
		sprintf_s( vTemp, "%d", tone);
		if( (fp = fopen( filename, "rb+" ))==NULL )
		{
			_mkdir( "map" );
			fp = fopen( filename, "wb" );
			fclose( fp );
			if( (fp = fopen( filename, "rb+" ))==NULL )
				return FALSE;
		}
		fwrite( vTemp, sizeof(char), 2, fp );
		fclose (fp);
	}
	switch(tone){
	//?????
	case 40:	//???
		map_bgm_no = 4;
		break;
	case 41:	//??
		map_bgm_no = 3;
		break;
	case 42:	//叩??
		map_bgm_no = 7;
		break;
	case 43:	//叩??
		map_bgm_no = 8;
		break;
	case 44:	//叩??
		map_bgm_no = 9;
		break;
	case 45:	//?????
		map_bgm_no = 10;
		break;
	case 46:	//????????????
		map_bgm_no = 11;
		break;
	case 47:	//畛菟湮翻
		map_bgm_no = 15;
		break;
	case 48:	//畛菟埶
		map_bgm_no = 16;
		break;
	case 49:	//傭部
		map_bgm_no = 21;
		break;
	case 50:	//華傑
		map_bgm_no = 17;
		break;
	case 51:	//阨傑
		map_bgm_no = 18;
		break;
	case 52:	//鳶傑
		map_bgm_no = 19;
		break;
	case 53:	//瑞傑
		map_bgm_no = 20;
		break;

#ifdef _NEWMUSICFILE6_0		   // Syu ADD 6.0 陔華芞秞氈
	case 54:
		map_bgm_no = 22;	   // 漆菁韌貓
		break;
	case 55:
		map_bgm_no = 23;	   // 漆菁埻
		break;
#endif
	}
	return 0;
}

//??????????  ????  ??????????
int play_environment(int tone, int x, int y)
{
	int d0,d1,distance,volume;
	d0 = d1 = 0;
	float dx, dy;
	if (MuteFlag) return 0;
//	tone = 157;

	if(tone < 20 || tone > 37){		//阪??π??
		return 0;					//?????
	}

	switch(tone){
	case 28:		//???
		if(SaTimeZoneNo != LS_MORNING){		//??????
			return 0;						//?????
		}
		break;
	case 29:		//?
		if(SaTimeZoneNo != LS_NIGHT){		//??????
			return 0;					//?????
		}
		break;
	case 37:		//??
		if(SaTimeZoneNo == LS_MORNING){		//???
			return 0;						//?????
		}
		if(SaTimeZoneNo == LS_NIGHT){		//???
			tone = 29;					//??阪?
			break;
		}
		break;
	}






	dx = (float)(x - 320);
	dy = (float)(y - 240);

	distance = (int)sqrt( (double)(dx*dx+dy*dy) );

	volume = distance << 4;		//?????π?阪?
	volume /= 0x6a;				//

	if(volume >= 127)				//吳????
		return 0;					//?????

	d0 = tone - 20;
	tone = 151 + d0;
	if(env_tbl[d0].count == 0){		//???
		//???向
		env_tbl[d0].distance = distance;
		volume = (127 - volume) * tone_tbl[ tone ].voice_volume / 127;
		env_tbl[d0].volume = volume * t_music_se_volume / 15;
		d1 = 1;		//????????
		if(env_tbl[d0].voice_address == -1){		//???
			env_tbl[d0].tone = tone;
		}
	} else {
		//???????向
		if(env_tbl[d0].distance > distance){
			env_tbl[d0].distance = distance;
			volume = (127 - volume) * tone_tbl[ tone ].voice_volume / 127;
			env_tbl[d0].volume = volume * t_music_se_volume / 15;
			d1 = 1;		//????????
		}
	}
	//????
	if(d1){
		env_tbl[d0].side = x;
	}
	env_tbl[d0].count++;
	return 0;
}

//??????????  ???????  ??????????
void play_environment_check(void)
{
	if(t_music_bgm_volume==1) return;
	int d0,d1,d3,d7;
	for(d7=0; d7<MAX_ENV; d7++){
		if(env_tbl[d7].count == 0){		//?????
			if(env_tbl[d7].voice_address != -1){		//???
				pDSData[ env_tbl[d7].voice_address ]->Stop();		//????
				voice[ env_tbl[d7].voice_address ].loop_flg = 0;		//?????
				env_tbl[d7].voice_address = -1;
			}
			continue;
		}
		if(env_tbl[d7].voice_address == -1){		//???
			env_tbl[d7].voice_address = voice_seek_point;
			env_tbl[d7].volume_old = -1;
			env_tbl[d7].panpot_old = -1;
			d0 = 1;
		} else {
			d0 = 0;
		}

		if(stereo_flg == T_MUSIC_MONO){		//??????
			env_tbl[d7].panpot = d1 = 64;
		} else {
			d1 = env_tbl[d7].side / 5;			//?????π?阪?
			if(d1 < 0)			//
				d1 = 0;			//
			if(d1 > 127)		//
				d1 = 127;		//
		}

		if(d0){		//???
			env_tbl[d7].panpot = d1;
		} else {
			d3 = d1 - env_tbl[d7].panpot;
			if(d3 > 2)
				d3 = 2;
			if(d3 < -2)
				d3 = -2;
			//?????　叉
			env_tbl[d7].panpot += d3;
		}
//		play_environment_tone(env_tbl[d7].tone, env_tbl[d7].distance, env_tbl[d7].voice_address, env_tbl[d7].panpot, d0);
		play_environment_tone(d7, d0);

		env_tbl[d7].count = 0;		//????????
	}
}

//??????????  ???????????  ??????????
void bgm_fade_check(void)
{
	//???????
	if(t_music_playing_flg == 0){
		t_music_fade_flg = 0;
		return;
	}
	//????????????
	if(!t_music_fade_flg){
		return;
	}
	
	if(!t_music_fade_vol || (t_music_bgm_volume==1 && t_music_se_volume == 1)){
		stop_bgm();
		t_music_bgm_no = -1;
	} else {
		// ????????
		pDSData_tone[ TONE_MAX ]->SetVolume(volume_tbl[ --t_music_fade_vol ]);
	}
}

//??????????  ???????  ??????????
void check_se_loop(void){
	int d7;

	if(dsound_flg == -1)
		return;
	for(d7=0; d7<TONE_MAX ;d7++){
		if(tone_tbl[ d7 ].play_time){		//????
			tone_tbl[ d7 ].play_time--;
		}
	}
	//???????
	play_environment_check();
	//??????????
	play_map_bgm_check();
	//???????????????
	bgm_fade_check();
}

















int cdda_no = -1;
int stereo_flg = T_MUSIC_STEREO;
int t_music_se_volume = 15;
int t_music_bgm_volume = 15;
static MCI_OPEN_PARMS open  = {0};
static MCIERROR       dwRes = {0};
static int cdda_flg;
static int cdda_check_cnt = -1;
/////////////////////////////////////////////////////////////////////
//  CD-DA???? n?????
//     ???? true?刺??
//     ???? ???????????? false?刺??
bool cdda_open(int n)
{
	cdda_flg = 0;
// ?????? 2???????
    _ASSERT( n > 1);

//    MCI_OPEN_PARMS open  = {0};
//  MCIERROR       dwRes = {0};

// ???????
    open.lpstrDeviceType  = "cdaudio";
    dwRes = mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE, (DWORD)&open);
    if ( dwRes)
    {
#ifdef _STONDEBUG_
//		MessageBoxNew(hWnd, "湖羲CD秞寢囮啖ㄐ", "隅", MB_OK);
#endif
		cdda_flg = 1;
        return FALSE;
    }

// ?????????????叉???
    MCI_SET_PARMS set;
    set.dwTimeFormat = MCI_FORMAT_TMSF;
    dwRes = mciSendCommand( open.wDeviceID, MCI_SET, 
                            MCI_SET_TIME_FORMAT, (DWORD)&set); 
    if ( dwRes)
    {
#ifdef _STONDEBUG_
//		MessageBoxNew(hWnd, "湖羲CD秞寢囮啖ㄐ", "隅", MB_OK);
#endif
		cdda_flg = 1;
        return FALSE;
    }
	return TRUE;
}
/*--------------------------------------------
			?????????
---------------------------------------------*/
bool cdda_start(int n)
{
// ???
    // ???? n???
    MCI_PLAY_PARMS play;
    play.dwFrom = MCI_MAKE_TMSF( n,0,0,0);
    play.dwTo   = MCI_MAKE_TMSF( n+1,0,0,0);
    dwRes = mciSendCommand( open.wDeviceID, 
                            MCI_PLAY, MCI_FROM | MCI_TO,
                            (DWORD)&play);
    if ( dwRes)
    {
#ifdef _STONDEBUG_
//		MessageBoxNew(hWnd, "??????????????", "隅", MB_OK);
#endif
		cdda_flg = 2;		//?????????
        return FALSE;
    }
	return TRUE;
}
/*--------------------------------------------
			?????于
---------------------------------------------*/
bool cdda_stop(void)
{
	if(cdda_flg == 1)		//??????????
		return TRUE;

	mciSendCommand(open.wDeviceID, MCI_STOP, 0, NULL);

// ????????
    dwRes = mciSendCommand( open.wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
    if ( dwRes)
    {
#ifdef _STONDEBUG_
//	    MessageBoxNew(hWnd, "??????????????", "隅", MB_OK);
#endif
        return FALSE;
    }
	return TRUE;
}
/*--------------------------------------------
			??????
---------------------------------------------*/
extern	DWORD NowTime;
//extern	double NowTime;

bool cdda_play(int n)
{
	check_se_loop();		//????????

	cdda_flg = 1;		//??????????
	return TRUE;

	if(cdda_flg == 1)		//??????????
		return TRUE;

	if(cdda_no != n ){		//??阪????
		cdda_flg = 0;		//??????
		cdda_check_cnt = -1;
	}

	if(cdda_flg)		//??????
		return TRUE;

	cdda_check_cnt++;
	cdda_check_cnt&=63;
	if(cdda_check_cnt)
		return TRUE;

	if(cdda_no == n ){		//????
		MCI_STATUS_PARMS mcisp;	mcisp.dwItem = MCI_STATUS_MODE;
		if(mciSendCommand(open.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcisp)){		//???
			cdda_flg = 3;		//??????????
			return FALSE;
		}
		if(mcisp.dwReturn == MCI_MODE_PLAY){		//????
			return TRUE;
		} else {		//?????
			cdda_no = Rnd( 11, 34 );
			cdda_start( cdda_no );
			// ???ㄈ?釤????
			NowTime = TimeGetTime();
			return TRUE;
		}
	} else {
		cdda_no = n;		//???
		cdda_stop();
		cdda_open( n );
		cdda_start( n );
		// ???ㄈ?釤????
		NowTime = TimeGetTime();
	}
	return TRUE;
}
