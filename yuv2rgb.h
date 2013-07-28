#ifndef __YUV420SP_TO_RGB24_H__
#define __YUV420SP_TO_RGB24_H__




#include "hi_type.h"
#include "hi_math.h"
#include "hi_defines.h"


#ifdef  __cplusplus
#if  __cplusplus
extern  "C"{
#endif
#endif

//typedef unsigned short WORD;//HI_U16
//typedef unsigned long DWORD;//HI_U32
//typedef long LONG;//HI_S32
//typedef unsigned char BYTE;//HI_U8


typedef struct tagBITMAPFILEHEADER
{ // bmfh
        HI_U16 bfType;
        HI_U32 bfSize;
        HI_U16 bfReserved1;
        HI_U16 bfReserved2;
        HI_U32 bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{ // bmih
        HI_U32 biSize;
        HI_S32 biWidth;
        HI_S32 biHeight;
        HI_U16 biPlanes;
        HI_U16 biBitCount;
        HI_U32 biCompression;
        HI_U32 biSizeImage;
        HI_S32 biXPelsPerMeter;
        HI_S32 biYPelsPerMeter;
        HI_U32 biClrUsed;
        HI_U32 biClrImportant;
}BITMAPINFOHEADER;



int saveBmpFileHeader(FILE *fp, BITMAPFILEHEADER *bmfh);

int saveBmpInfoHeader(FILE *fp, BITMAPINFOHEADER *bmih);


HI_U8 *YUV420SP_to_RGB24(VIDEO_FRAME_INFO_S *pVBuf, HI_U8 *rgb24);

void SaveBmpInfo(FILE *fd, int picwidth, int picheight);

void SaveBmpData(FILE *fd, HI_U32 picwidth, HI_U32 picheight, HI_U8 *rgb);



#ifdef  __cplusplus
#if  __cplusplus
}
#endif
#endif


#endif



