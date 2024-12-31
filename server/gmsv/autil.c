// Arminius' protocol utilities ver 0.1
//
// Any questions and bugs, mailto: arminius@mail.hwaei.com.tw

// -------------------------------------------------------------------
// The following definitions is to define game-dependent codes.
// Before compiling, remove the "//".
#define __STONEAGE
#include "version.h"
#include <stdio.h>
#include <stdlib.h>
#include "autil.h"
#include "char.h"
#ifdef __STONEAGE
#include "lssproto_util.h"
#include "common.h"
#endif

// Nuke 0701 fix
char *MesgSlice[SLICE_MAX];
int SliceCount;

char PersonalKey[4096];

// -------------------------------------------------------------------
// Initialize utilities
//
BOOL util_Init( void)
{
  int i;

  for (i=0; i<SLICE_MAX; i++){
    MesgSlice[i] = (char *) calloc( 1,SLICE_SIZE);
    if(MesgSlice[i]==NULL) return FALSE;
  }

  SliceCount = 0;
  strcpy(PersonalKey, _DEFAULT_PKEY);

	return TRUE;
}

// -------------------------------------------------------------------
// Split up a message into slices by spearator.  Store those slices
// into a global buffer "char **MesgSlice"
//
// arg: source=message string;  separator=message separator (1 byte)
// ret: (none)

// WON ADD
//void util_SplitMessage(char *source, char *separator)
BOOL util_SplitMessage(char *source, char *separator)
{
#ifdef _NEW_FUNC_DECRYPT
	if(source[0]!= '('){
		return FALSE;
	}
	source++;
#endif

  if (source && separator) {	// NULL input is invalid.
    char *ptr;
    char *head = source;
    
    // Nuke 1006 : Bug fix
    while ((ptr = (char *) strstr(head, separator)) && (SliceCount<SLICE_MAX) && (SliceCount>=0)) {
      ptr[0] = '\0';
      if (strlen(head)<SLICE_SIZE) {	// discard slices too large

		// Nuke 0701
//		if (*MesgSlice != *dumb) {
				//print("Warning! Mem may be broken\n");
		//}
/*
        if (MesgSlice[SliceCount]==0xffffffff) {
                print("MesgSlice[%d] broken\n",SliceCount);
                return FALSE;
        } else {
*/
                strcpy(MesgSlice[SliceCount], head);
                SliceCount++;
        //}

      }
      head = ptr+1;
    }
    strcpy(source, head);	// remove splited slices	
  }
  return TRUE;
}

// -------------------------------------------------------------------
// Encode the message
//
// arg: dst=output  src=input
// ret: (none)
void util_EncodeMessage(char *dst, char *src)
{
//  strcpy(dst, src);
//  util_xorstring(dst, src);

  int rn = rand() % 99;
  int t1, t2;
  char t3[65500], tz[65500];
  util_swapint(&t1, &rn, "2413");
  t2 = t1 ^ 0xffffffff;
  util_256to64(tz, (char *) &t2, sizeof(int), DEFAULTTABLE);

  util_shlstring(t3, src, rn);
  strcat(tz, t3);
  util_xorstring(dst, tz);
}

// -------------------------------------------------------------------
// Decode the message
//
// arg: dst=output  src=input
// ret: (none)
#ifdef _NEW_FUNC_DECRYPT
void hexstrtostring(char* dst,char* src)
{
	int len;
	int b, tmp;
	len = strlen(src);
	int i;
	for ( i= 0; i < len; i++)
	{
		if (src[i] >= 65) { b = (src[i] - 65 + 10); }
		else { b = src[i] - 48; }
		if (i % 2)
		{
			tmp = tmp + b;
			dst[i / 2] = tmp;
		}
		else
		{
			tmp = b <<4;
		}
	}
	dst[strlen(dst)]=0;
}

unsigned long DELTA=0x9e3779b9;
#define MX (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);
long DecodeTea(long* v, long n, long* k)
{
	unsigned long z,y,sum,e;
	z=v[n-1];
	y=v[0];
	sum=0;
	long p, q;
	q = 6 + 52/n;
	sum = q*DELTA;
	while(sum != 0)
	{
		e = (sum >> 2) & 3;
		for (p=n-1; p>0; p--)
			z = v[p-1], y = v[p] -= MX;
		z = v[n-1]; y = v[0] -= MX;
		sum -= DELTA;
	}
	return 0;
}
BOOL util_DecodeMessageTea(char *dst,char *sac)
{
	int len = strlen(sac)/2;
	if (sac[0]=='\0') return FALSE;
	if(len/4 <= 0 ) return FALSE;

	hexstrtostring(dst,sac);
	DecodeTea((long*)dst,len/4,(long *)DENGLUKEY1);
	return TRUE;
}
#endif

void util_DecodeMessage(char *dst, char *src)
{
//  strcpy(dst, src);
//  util_xorstring(dst, src);

#define INTCODESIZE	(sizeof(int)*8+5)/6

  int rn;
  int *t1, t2;
  char t3[4096], t4[4096];	// This buffer is enough for an integer.
  char tz[65500];
  if (src[strlen(src)-1]=='\n') src[strlen(src)-1]='\0';
  util_xorstring(tz, src);

  // get seed
  strncpy(t4, tz, INTCODESIZE);
  t4[INTCODESIZE] = '\0';
  util_64to256(t3, t4, DEFAULTTABLE);
  t1 = (int *) t3;
  t2 = *t1 ^ 0xffffffff;
  util_swapint(&rn, &t2, "3142");

  util_shrstring(dst, tz + INTCODESIZE, rn);
}

// -------------------------------------------------------------------
// Get a function information from MesgSlice.  A function is a complete
// and identifiable message received, beginned at DEFAULTFUNCBEGIN and
// ended at DEFAULTFUNCEND.  This routine will return the function ID
// (Action ID) and how many fields this function have.
//
// arg: func=return function ID    fieldcount=return fields of the function
// ret: 1=success  0=failed (function not complete)
int util_GetFunctionFromSlice(int *func, int *fieldcount)
{
  char t1[16384];
  int i;

//  if (strcmp(MesgSlice[0], DEFAULTFUNCBEGIN)!=0) util_DiscardMessage();
  
  strcpy(t1, MesgSlice[1]);
  // Robin adjust
#ifdef _VERSION_NEW
  *func=atoi(t1) - 13;
#else
#ifdef _VERSION_80
  *func=atoi(t1) - 13;
#else
  *func=atoi(t1);
#endif
#endif
  for (i=0; i<SLICE_MAX; i++)
    if (strcmp(MesgSlice[i], DEFAULTFUNCEND)==0) {
      *fieldcount=i-2;	// - "&" - "#" - "func" 3 fields
      return 1;
    }

  return 0;	// failed: message not complete
}

void util_DiscardMessage(void)
{
  SliceCount=0;
}
extern int clisendfunc;
#ifdef _ABSOLUTE_DEBUG
extern int lastfunctime;
#endif
void _util_SendMesg(char *file, int line, int fd, int func, char *buffer)
{
//  char t1[16384], t2[16384];
	char t1[1024*64], t2[1024*64];
	clisendfunc=func;
#ifdef _ABSOLUTE_DEBUG
	lastfunctime=4;
#endif
  // WON ADD
  if( fd < 0 ){
	print("\n SendMesg fd err %s:%d!! ==> func(%d)\n", file, line, func);
	return;
  }
#ifdef _VERSION_NEW
  sprintf(t1, "&;%d%s;#;", func+23, buffer);
#else
#ifdef _VERSION_80
  sprintf(t1, "&;%d%s;#;", func+23, buffer);
#else
  sprintf(t1, "&;%d%s;#;", func, buffer);
#endif
#endif
  util_EncodeMessage(t2, t1);
#ifdef __STONEAGE
  lssproto_Send(fd, t2);
#endif

}

int util_256to64(char *dst, char *src, int len, char *table)
{
  unsigned int dw,dwcounter,i;

  if (!dst || !src || !table) return 0;
  dw=0;
  dwcounter=0;
  for (i=0; i<len; i++) {
    dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
    dst[ dwcounter++ ] = table[ dw & 0x3f ];
    dw = ( dw >> 6 );
    if (i%3==2) {
      dst[ dwcounter++ ] = table[ dw & 0x3f ];
      dw = 0;
    }
  }
  if (dw) dst[ dwcounter++ ] = table[ dw ];
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Convert 6-bit strings into 8-bit strings, buffers that store these strings
// must have enough space.
//
// arg: dst=6-bit string;  src=8-bit string;  table=mapping table
// ret: 0=failed  >0=bytes converted
int util_64to256(char *dst, char *src, char *table)
{
  unsigned int dw,dwcounter,i;
  char *ptr = NULL;

  dw=0;
  dwcounter=0;
  if (!dst || !src || !table) return 0;
  for (i=0; i<strlen(src); i++) {
    ptr = (char *) index(table, src[i]);
    if (!ptr) return 0;
    if (i%4) {
      dw = ((unsigned int)(ptr-table) & 0x3f) << ((4-(i%4))*2) | dw;
      dst[ dwcounter++ ] = dw & 0xff;
      dw = dw >> 8;
    } else {
      dw = (unsigned int)(ptr-table) & 0x3f;
    }
  }
  if (dw) dst[ dwcounter++ ] = dw & 0xff;
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_256to64_shr(char *dst, char *src, int len, char *table, char *key)
{
  unsigned int dw,dwcounter,i,j;

  if (!dst || !src || !table || !key) return 0;
  if (strlen(key)<1) return 0;	// key can't be empty.
  dw=0;
  dwcounter=0;
  j=0;
  for (i=0; i<len; i++) {
    dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
    dst[ dwcounter++ ] = table[ ((dw & 0x3f) + key[j]) % 64 ];	// check!
    j++;  if (!key[j]) j=0;
    dw = ( dw >> 6 );
    if (i%3==2) {
      dst[ dwcounter++ ] = table[ ((dw & 0x3f) + key[j]) % 64 ];// check!
      j++;  if (!key[j]) j=0;
      dw = 0;
    }
  }
  if (dw) dst[ dwcounter++ ] = table[ (dw + key[j]) % 64 ];	// check!
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shr.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_shl_64to256(char *dst, char *src, char *table, char *key)
{
  unsigned int dw,dwcounter,i,j;
  char *ptr = NULL;

  if (!key || (strlen(key)<1)) return 0;	// must have key

  dw=0;
  dwcounter=0;
  j=0;
  if (!dst || !src || !table) return 0;
  for (i=0; i<strlen(src); i++) {
    ptr = (char *) index(table, src[i]);
    if (!ptr) return 0;
    if (i%4) {
      // check!
      dw = ((((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64)
           << ((4-(i%4))*2) | dw;
      j++;  if (!key[j]) j=0;
      dst[ dwcounter++ ] = dw & 0xff;
      dw = dw >> 8;
    } else {
      // check!
      dw = (((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64;
      j++;  if (!key[j]) j=0;
    }
  }
  if (dw) dst[ dwcounter++ ] = dw & 0xff;
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_256to64_shl(char *dst, char *src, int len, char *table, char *key)
{
  unsigned int dw,dwcounter,i,j;

  if (!dst || !src || !table || !key) return 0;
  if (strlen(key)<1) return 0;	// key can't be empty.
  dw=0;
  dwcounter=0;
  j=0;
  for (i=0; i<len; i++) {
    dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)*2) ) | dw;
    dst[ dwcounter++ ] = table[ ((dw & 0x3f) + 64 - key[j]) % 64 ];	// check!
    j++;  if (!key[j]) j=0;
    dw = ( dw >> 6 );
    if (i%3==2) {
      dst[ dwcounter++ ] = table[ ((dw & 0x3f) + 64 - key[j]) % 64 ];	// check!
      j++;  if (!key[j]) j=0;
      dw = 0;
    }
  }
  if (dw) dst[ dwcounter++ ] = table[ (dw + 64 - key[j]) % 64 ];	// check!
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shl.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_shr_64to256(char *dst, char *src, char *table, char *key)
{
  unsigned int dw,dwcounter,i,j;
  char *ptr = NULL;

  if (!key || (strlen(key)<1)) return 0;	// must have key

  dw=0;
  dwcounter=0;
  j=0;
  if (!dst || !src || !table) return 0;
  for (i=0; i<strlen(src); i++) {
    ptr = (char *) index(table, src[i]);
    if (!ptr) return 0;
    if (i%4) {
      // check!
      dw = ((((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64)
           << ((4-(i%4))*2) | dw;
      j++;  if (!key[j]) j=0;
      dst[ dwcounter++ ] = dw & 0xff;
      dw = dw >> 8;
    } else {
      // check!
      dw = (((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64;
      j++;  if (!key[j]) j=0;
    }
  }
  if (dw) dst[ dwcounter++ ] = dw & 0xff;
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Swap a integer (4 byte).
// The value "rule" indicates the swaping rule.  It's a 4 byte string
// such as "1324" or "2431".
//
void util_swapint(int *dst, int *src, char *rule)
{
  char *ptr, *qtr;
  int i;

  ptr = (char *) src;
  qtr = (char *) dst;
  for (i=0; i<4; i++) qtr[rule[i]-'1']=ptr[i];
}

// -------------------------------------------------------------------
// Xor a string.  Be careful that your string contains '0xff'.  Your
// data may lose.
//
void util_xorstring(char *dst, char *src)
{
	int i;
	if (strlen(src)>65500) return;

	for (i=0; i<strlen(src); i++){
	  dst[i]=src[i]^255;
	}
	dst[i]='\0';
}

// -------------------------------------------------------------------
// Shift the string right.
//
void util_shrstring(char *dst, char *src, int offs)
{
  char *ptr;
  if (!dst || !src || (strlen(src)<1)) return;
  
  offs = strlen(src) - (offs % strlen(src));
  ptr = src+offs;
  strcpy(dst, ptr);
  strncat(dst, src, offs);
  dst[strlen(src)]='\0';
}

// -------------------------------------------------------------------
// Shift the string left.
//
void util_shlstring(char *dst, char *src, int offs)
{
  char *ptr;
  if (!dst || !src || (strlen(src)<1)) return;
  
  offs = offs % strlen(src);
  ptr = src+offs;
  strcpy(dst, ptr);
  strncat(dst, src, offs);
  dst[strlen(src)]='\0';
}

// -------------------------------------------------------------------
// Convert a message slice into integer.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkint
int util_deint(int sliceno, int *value)
{
  int *t1, t2;
  char t3[4096];	// This buffer is enough for an integer.

  if (strlen(PersonalKey)==0) strcpy(PersonalKey, _DEFAULT_PKEY);

  util_shl_64to256(t3, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
  t1 = (int *) t3;
  t2 = *t1 ^ 0xffffffff;
  util_swapint(value, &t2, "2413");

  return *value;
}

int util_mkint(char *buffer, int value)
{
  int t1, t2; 
  char t3[4096];

  if (strlen(PersonalKey)==0) strcpy(PersonalKey, _DEFAULT_PKEY);
  util_swapint(&t1, &value, "3142");
  t2 = t1 ^ 0xffffffff;
  util_256to64_shr(t3, (char *) &t2, sizeof(int), DEFAULTTABLE, PersonalKey);
  strcat(buffer, ";");
  strcat(buffer, t3);

  return value;
}

// -------------------------------------------------------------------
// Convert a message slice into string.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkstring
int util_destring(int sliceno, char *value)
{

  if (strlen(PersonalKey)==0) strcpy(PersonalKey, _DEFAULT_PKEY);

  util_shr_64to256(value, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
  
  return strlen(value);
}

// -------------------------------------------------------------------
// Convert a string into buffer (a string).  Return a checksum.
//
// arg: buffer=output   value=data to pack
// ret: checksum, this value must match the one generated by util_destring
int util_mkstring(char *buffer, char *value)
{
  char t1[SLICE_SIZE];

  if (strlen(PersonalKey)==0) strcpy(PersonalKey, _DEFAULT_PKEY);

  util_256to64_shl(t1, value, strlen(value), DEFAULTTABLE, PersonalKey);
  strcat(buffer, ";");	// It's important to append a SEPARATOR between fields
  strcat(buffer, t1);

  return strlen(value);
}
