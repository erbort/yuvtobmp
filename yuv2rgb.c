#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"
#include "assert.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sample_comm.h"


#include "yuv2rgb.h"

//typedef unsigned short WORD;//HI_U16
//typedef unsigned long DWORD;//HI_U32
//typedef long LONG;//HI_S32
//typedef unsigned char BYTE;//HI_U8
/*
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
*/


int saveBmpFileHeader(FILE *fp,BITMAPFILEHEADER *bmfh)
{
    	if(fwrite(&bmfh->bfType,sizeof(HI_U16),1,fp)!=1)
    	{
        	printf("Error(saveBmpFileHeader): can not write bfType of the File Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmfh->bfSize,sizeof(HI_U32),1,fp)!=1)
    	{
        	printf("Error(saveBmpFileHeader): can not write bfSize of the File Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmfh->bfReserved1,sizeof(HI_U16),1,fp)!=1)
    	{
        	printf("Error(saveBmpFileHeader): can not write bfReserved1 of the File Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmfh->bfReserved2,sizeof(HI_U16),1,fp)!=1)
    	{
        	printf("Error(saveBmpFileHeader): can not write bfReserved2 of the File Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmfh->bfOffBits,sizeof(HI_U32),1,fp)!=1)
    	{
        	printf("Error(saveBmpFileHeader): can not write bfOffBits of the File Header.\n");
        	return -1;
    	}

    	return 0;
}




int saveBmpInfoHeader(FILE *fp,BITMAPINFOHEADER *bmih)
{
    	if(fwrite(&bmih->biSize,sizeof(HI_U32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biSize of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biWidth,sizeof(HI_S32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biWidth of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biHeight,sizeof(HI_S32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biHeight of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biPlanes,sizeof(HI_U16),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biPlanes of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biBitCount,sizeof(HI_U16),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biBitCount of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biCompression,sizeof(HI_U32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biCompression of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biSizeImage,sizeof(HI_U32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biSizeImage of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biXPelsPerMeter,sizeof(HI_S32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biXPelsPerMeter of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biYPelsPerMeter,sizeof(HI_S32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biYPelsPerMeter of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biClrUsed,sizeof(HI_U32),1,fp)!=1)
    	{
        	printf("Error(saveBmpInfoHeader): can not write biClrUsed of the Info Header.\n");
        	return -1;
    	}

    	if(fwrite(&bmih->biClrImportant,sizeof(HI_U32),1,fp)!=1)
    	{
    	    	printf("Error(saveBmpInfoHeader): can not write biClrImportant of the Info Header.\n");
        	return -1;
    	}

    	return 0;
}

/**************************************************************************************
function :	yuv422 -> rgb888


**************************************************************************************/
HI_U8 *YUV420SP_to_RGB24(VIDEO_FRAME_INFO_S *pVBuf, HI_U8 *rgb24)
{
	HI_U32 width = pVBuf->stVFrame.u32Width;		//picture width
    	HI_U32 height = pVBuf->stVFrame.u32Height;		//picture height
	
   	// HI_S32 s32Ret = HI_SUCCESS;
    	HI_U8 *pVBufVirt_Y;
    	HI_U8 *pVBufVirt_C;
	HI_U32 stride;
 
    	HI_U32 phy_addr[2],size;
    	HI_U8 *pUserPageAddr[2];
    	PIXEL_FORMAT_E  enPixelFormat = pVBuf->stVFrame.enPixelFormat; //get video format
 
   	HI_U32 u32UvHeight;
	
    	if (PIXEL_FORMAT_YUV_SEMIPLANAR_420 == enPixelFormat) //yuv
    	{
        	size = (pVBuf->stVFrame.u32Stride[0])*(pVBuf->stVFrame.u32Height)*3/2;
        	u32UvHeight = pVBuf->stVFrame.u32Height/2;
    	}else{
        	size = (pVBuf->stVFrame.u32Stride[0])*(pVBuf->stVFrame.u32Height);
        	u32UvHeight = pVBuf->stVFrame.u32Height;
    	}

	phy_addr[0] = pVBuf->stVFrame.u32PhyAddr[0];//Y 
    	phy_addr[1] = pVBuf->stVFrame.u32PhyAddr[1];//UV
    	//printf("phy_addr:%x, size:%d\n", phy_addr, size);

	
    	pUserPageAddr[0] = (HI_U8 *) HI_MPI_SYS_Mmap(phy_addr[0], size);
    	if (NULL == pUserPageAddr[0])
    		return HI_FAILURE;
    
    	pUserPageAddr[1] = (HI_U8 *) HI_MPI_SYS_Mmap(phy_addr[1], size);
    	if (NULL == pUserPageAddr[0])
        	return HI_FAILURE;

    	printf("stride: %d,%d\n",pVBuf->stVFrame.u32Stride[0],pVBuf->stVFrame.u32Stride[1] );
    

        stride = pVBuf->stVFrame.u32Stride[0] - pVBuf->stVFrame.u32Width;

	pVBufVirt_Y = pUserPageAddr[0];
    	pVBufVirt_C = pUserPageAddr[1];


	HI_U8 *yuv420sp[3];
    	yuv420sp[0]=pVBufVirt_Y;
    	yuv420sp[1]=pVBufVirt_C;
    	yuv420sp[2]=pVBufVirt_C;
/*****************************************************************************/
    //	SaveBmpInfo(pfd, width, height);
    	//rgb24 = (HI_U8 *)malloc(Linebytes*height);
	//memset(rgb24, 0 , Linebytes*height);
    	int R,G,B;
	int x,y;
    	HI_U8 Y,U,V;
	printf("yuv start convert to rgb!\n");	
    	for(y=0;y<height;y++)
    	{
        	for(x=0;x<width;x++)
  		{
   	    		Y = *(yuv420sp[0] + y*width + x + y*stride);

	    		if(x==0)					//the first byte 
	    		{
				U = *(yuv420sp[1] + (int)(y/2)*width + 0 + (int)(y/2)*stride);
   	    			V = *(yuv420sp[2] + (int)(y/2)*width + 1 + (int)(y/2)*stride);
	    		}else if((x>0)&&(x%2==0)){
   	    			U = *(yuv420sp[1] + (int)(y/2)*width + x + (int)(y/2)*stride);
   	    			V = *(yuv420sp[2] + (int)(y/2)*width + x+1 +(int)(y/2)*stride);
	    		}else{					
				U = *(yuv420sp[1] + (int)(y/2)*width + x-1 +(int)(y/2)*stride);
   	    			V = *(yuv420sp[2] + (int)(y/2)*width + x +(int)(y/2)*stride);
	    		}						//the above code is get the Y U V data

			
   	    		R = Y + 1.402*(V-128);//method 1
   	    		G = Y - 0.34414*(U-128) - 0.71414*(V-128);
  	    		B = Y + 1.772*(U-128);
			
			/*
	  		R=1.164*(Y-16)+1.596*(V-128);//method 2
	  		G=1.164*(Y-16)+0.813*(V-128)-0.391*(U-128);
	  		B=1.164*(Y-16)+2.018*(U-128);
	  		*/
			/*
			R = Y + 1.4075*(V-128);//method 3
			G = Y - 0.3445*(U-128) - 0.7169*(V-128);
			B = Y + 1.779*(U-128);
			*/
			/*
			R = 1.164*(Y-16) + 1.596*(V-128);//method 4
			G = 1.164*(Y-16) - 0.392*(U-128) - 0.813*(V-128);
			B = 1.164*(Y-16) + 2.017*(U-128);			
			*/

	 		/*		correct the RGB data		*/ 
	   		if(R>255)	R=255;
	   		if(R<0)		R=0;
			
	   		if(G>255)	G=255;
	   		if(G<0)		G=0;
			
	   		if(B>255)	B=255;
	   		if(B<0)		B=0;
				
  			rgb24[((height-y-1)*width + x)*3 + 0] = (HI_U8)R;
	   		rgb24[((height-y-1)*width + x)*3 + 1] = (HI_U8)G;
	   		rgb24[((height-y-1)*width + x)*3 + 2] = (HI_U8)B;
  		}
    	}
	//SaveBmpData(pfd, width, height, rgb24);
    	//fwrite(rgb24, Linebytes*height, 1, pfd);
    	//fclose(pfd);
    	//free(rgb24);
    	printf("YUV2BMP success ok\n");
    	HI_MPI_SYS_Munmap(pUserPageAddr[0], size);
    	HI_MPI_SYS_Munmap(pUserPageAddr[1], size);
    	return rgb24;
}



void SaveBmpInfo(FILE *fd, int picwidth, int picheight)
{
	HI_U32 width,height;
	width = picwidth;
	height = picheight;

	BITMAPFILEHEADER bmfh;
        BITMAPINFOHEADER bmih;

        HI_U64 Linebytes = width*3;
        HI_U64 SizeImage = Linebytes*height;

        /*file header*/
        bmfh.bfType=0x4D42;
        bmfh.bfSize=SizeImage+54;
        bmfh.bfReserved1 = 0;
        bmfh.bfReserved2 = 0;
        bmfh.bfOffBits = 54;

        /*info header*/
        bmih.biSize = 40;
        bmih.biWidth = width;
        bmih.biHeight = height;
        bmih.biPlanes = 1;
        bmih.biBitCount = 24;
        bmih.biCompression = 0;
        bmih.biSizeImage =width*height*3;
        bmih.biXPelsPerMeter = 0;
        bmih.biYPelsPerMeter = 0;
        bmih.biClrUsed = 0;
        bmih.biClrImportant = 0;
	
	saveBmpFileHeader(fd, &bmfh);
        saveBmpInfoHeader(fd, &bmih);
}


void SaveBmpData(FILE *pfd, HI_U32 picwidth, HI_U32 picheight, HI_U8 *rgb)
{
	HI_U8 *rgb24;
	HI_U32 width,height;
	HI_U64 Size;
	FILE *fd;


	fd = pfd;
	rgb24 = rgb;
	width = picwidth;
	height = picheight;
	Size = 3*width*height;

	fwrite(rgb24, Size, 1, pfd);
}





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
