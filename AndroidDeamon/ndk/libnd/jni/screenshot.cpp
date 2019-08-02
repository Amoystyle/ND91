#include "screenshot.h"
#include "stdafx.h"
#include "memory.h"
#include "string.h"
#include "libjpeg/jpeglib.h"
#include <stdio.h>
#include "ndstream.h"
#include <sys/types.h>
#include <sys/wait.h>

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long				LONG;
char* g_frameBuffer = (char*) MAP_FAILED;

int FBOpen(FBInfo* pfb, const char* fbPath, BOOLEAN toBitMap565)
{
	pfb->fd = open(fbPath, O_RDONLY);
	if (pfb->fd < 0)
	{
		ErrorNdsh("open(\"%s\", O_RDONLY) fail !    errno: %d\n", fbPath, errno);		
		return -1;
	}

	if (ioctl(pfb->fd, FBIOGET_VSCREENINFO, &pfb->var_info) < 0)
	{
		ErrorNdsh("ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->var_info)\n\"%s\" is not a framebuffer\n", fbPath);
		return -1;
	}

	if (ioctl(pfb->fd, FBIOGET_FSCREENINFO, &pfb->fix_info) < 0)
	{
		ErrorNdsh("ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->var_info) error\n", fbPath);
		return -1;
	}



	//DebugNdsh("xres: %d, yres: %d\n", fb->var_info.xres, fb->var_info.yres);
	//DebugNdsh("xres_virtual: %d, yres_virtual: %d\n", fb->var_info.xres_virtual, fb->var_info.yres_virtual);
	//DebugNdsh("xoffset: %d, yoffset: %d", fb->var_info.xoffset, fb->var_info.yoffset);
	//DebugNdsh("line_length: %d, smem_len: %d, sizeVirtual: %d\n", fb->fix_info.line_length, fb->fix_info.smem_len, sizeVirtual);

	

	//处理截屏偏红问题（颜色起始位置错误）
	if(pfb->var_info.bits_per_pixel==32 && pfb->var_info.red.offset == 24 && pfb->var_info.green.offset==16 && pfb->var_info.blue.offset == 8) 
	{
		pfb->var_info.red.offset = 0;
		pfb->var_info.green.offset = 8;
		pfb->var_info.blue.offset = 16;
	}	

	//内存映射
	if(g_frameBuffer == MAP_FAILED)
	{
		g_frameBuffer = (char*) mmap(0, pfb->fix_info.smem_len, PROT_READ, MAP_SHARED, pfb->fd, 0);
	}	

	if (g_frameBuffer == MAP_FAILED)
	{
		ErrorNdsh("mmap(0, fb->fix_info.line_length * fb->var_info.yres, PROT_READ | PROT_WRITE, MAP_SHARED, fb->fd, 0) error\n");
		return -1;
	}


	//32位转16位
	if(toBitMap565 && pfb->var_info.bits_per_pixel == 32)
	{
		GetScreenDataRGB565(pfb);
	}
	else
	{
		GetScreenDataNormal(pfb);
	}
	
	return 0;
}

void FBClose(FBInfo* fb)
{
	//关闭fd
	close(fb->fd);

	//释放内存
	if(fb->bits)
		free(fb->bits);
}

void Write_var_screeninfo(FBInfo* fb, struct nd_data* data)
{
	WriteIntToBuffer(data, FBBpp(fb));
	WriteIntToBuffer(data, FBWidth(fb));
	WriteIntToBuffer(data, FBHeight(fb));
	
	WriteIntToBuffer(data, fb->var_info.red.offset);
	WriteIntToBuffer(data, fb->var_info.red.length);

	WriteIntToBuffer(data, fb->var_info.blue.offset);
	WriteIntToBuffer(data, fb->var_info.blue.length);

	WriteIntToBuffer(data, fb->var_info.green.offset);
	WriteIntToBuffer(data, fb->var_info.green.length);

	WriteIntToBuffer(data, fb->var_info.transp.offset);
	WriteIntToBuffer(data, fb->var_info.transp.length);
}

struct nd_data Screenshot(struct nd_data* data_receive)
{
	//DebugNdsh("Screenshot()\n");

	struct nd_data data_result = {0, 0, NULL};

	FBInfo fb;
	memset(&fb, '\0', sizeof(FBInfo));
	
	BOOLEAN toBitMap565 = ReadBooleanFromBuffer(data_receive);
	
	if(!Screencat(fb))
	{
		if (FBOpen(&fb, FB_PATH, toBitMap565) == -1)
		{
			ErrorNdsh("FBOpen(&fb, FB_PATH) fail!    errno: %d\n", errno);
			WriteIntToBuffer(&data_result, UNKNOW_ERROR);

			return data_result;
		}
	}	

	WriteIntToBuffer(&data_result, UNKNOW_ERROR);
	Write_var_screeninfo(&fb, &data_result);
	int size = FBVarDataSize(&fb);
	WriteStringToBuffer(&data_result, fb.bits, size);

	FBClose(&fb);
	WriteIntToBufferInPos(&data_result, 0, SUCCESS);
	return data_result;
}

bool Screencat(FBInfo& fb)
{
	DebugNdsh("Screencat begin");
	bool result = false;
    unsigned int i;
    int fd_screencap;
    int w, h, f;
    int fds[2];
	pid_t pid = -1;
	int size = 0;
	int remainSize = 0;
	int bufSize = 1024;

    if (pipe(fds) < 0)
		goto done;
		
	DebugNdsh("Screencat fork");

    pid = fork();
    if (pid < 0)
	{
	DebugNdsh("Screencat pid < 0");
		goto done;
	}

    if (pid == 0) {
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
        const char* command = "screencap";
        const char *args[2] = {command, NULL};
        execvp(command, (char**)args);
		printf("3");
        exit(1);
    }

    fd_screencap = fds[0];

    /* read w, h & format */
	
    if(read(fd_screencap, &w, 4) != 4) goto done;
	DebugNdsh("Screencat w:%d", w);
    if(read(fd_screencap, &h, 4) != 4) goto done;
	DebugNdsh("Screencat h:%d", h);
    if(read(fd_screencap, &f, 4) != 4) goto done;
	DebugNdsh("Screencat f:%d", f);
    if(!SetFbInfo(fb, f, w, h)) goto done;
   

    /* write data */	
	size = FBSize(&fb);
	remainSize = size;
	fb.bits = (char*) malloc(size);	
	while(remainSize > 0)
	{
		int toRead = bufSize < remainSize ? bufSize : remainSize;
		int onceRead = read(fd_screencap, fb.bits + size - remainSize, toRead);
		if(onceRead < 0) goto done;
		
		if(onceRead > 0)
		{	
			remainSize -= onceRead;
		}		
	}
	
	result = remainSize==0 && size>0;
	
done:
    TEMP_FAILURE_RETRY(waitpid(pid, NULL, 0));

    close(fds[0]);
    close(fds[1]);
	return result;
}

bool SetFbInfo(FBInfo& fb, int format, int w, int h)
{
 switch (format) {
        case 1: /* RGBA_8888 */
            fb.var_info.bits_per_pixel = 32;
            fb.var_info.xres = w;
            fb.var_info.yres = h;
            fb.var_info.red.offset = 0;
            fb.var_info.red.length = 8;
            fb.var_info.green.offset = 8;
            fb.var_info.green.length = 8;
            fb.var_info.blue.offset = 16;
            fb.var_info.blue.length = 8;
            fb.var_info.transp.offset = 24;
            fb.var_info.transp.length = 8;
            return true;
        case 2: /* RGBX_8888 */
            fb.var_info.bits_per_pixel = 32;
            fb.var_info.xres = w;
            fb.var_info.yres = h;
            fb.var_info.red.offset = 0;
            fb.var_info.red.length = 8;
            fb.var_info.green.offset = 8;
            fb.var_info.green.length = 8;
            fb.var_info.blue.offset = 16;
            fb.var_info.blue.length = 8;
            fb.var_info.transp.offset = 24;
            fb.var_info.transp.length = 0;
            return true;
        case 3: /* RGB_888 */
            fb.var_info.bits_per_pixel = 24;
            fb.var_info.xres = w;
            fb.var_info.yres = h;
            fb.var_info.red.offset = 0;
            fb.var_info.red.length = 8;
            fb.var_info.green.offset = 8;
            fb.var_info.green.length = 8;
            fb.var_info.blue.offset = 16;
            fb.var_info.blue.length = 8;
            fb.var_info.transp.offset = 24;
            fb.var_info.transp.length = 0;
            return true;
        case 4: /* RGB_565 */
            fb.var_info.bits_per_pixel = 16;
            fb.var_info.xres = w;
            fb.var_info.yres = h;
            fb.var_info.red.offset = 11;
            fb.var_info.red.length = 5;
            fb.var_info.green.offset = 5;
            fb.var_info.green.length = 6;
            fb.var_info.blue.offset = 0;
            fb.var_info.blue.length = 5;
            fb.var_info.transp.offset = 0;
            fb.var_info.transp.length = 0;
            return true;
        case 5: /* BGRA_8888 */
            fb.var_info.bits_per_pixel = 32;
            fb.var_info.xres = w;
            fb.var_info.yres = h;
            fb.var_info.red.offset = 16;
            fb.var_info.red.length = 8;
            fb.var_info.green.offset = 8;
            fb.var_info.green.length = 8;
            fb.var_info.blue.offset = 0;
            fb.var_info.blue.length = 8;
            fb.var_info.transp.offset = 24;
            fb.var_info.transp.length = 8;
           return true;
        default:
			return false;
    }   
	
	return false;
}

void GetScreenDataRGB565( FBInfo* pfb )
{
	pfb->bits = (char*) malloc(FBVarDataSize(pfb) / 2);	
 
	int offset = 0;/*FBOffSet(pfb);
	if((offset + FBVarDataSize(pfb)) > pfb->fix_info.smem_len)
		offset = 0;*/


	int redIndex = pfb->var_info.red.offset >> 3;
	int greenIndex = pfb->var_info.green.offset >> 3;
	int blueIndex = pfb->var_info.blue.offset >> 3;		
	int x = 0 ,y = 0;
	int sourceIndex = 0, targetIndex = 0;

	for(y = 0; y < FBHeight(pfb); y++)
	{
		for(x = 0; x < FBWidth(pfb); x++)
		{
			sourceIndex = offset + y * pfb->fix_info.line_length + x * 4;
			targetIndex = y * FBWidth(pfb) * 2 + x * 2;

			pfb->bits[targetIndex + 1] = g_frameBuffer[sourceIndex + redIndex] >> 3 << 3;
			pfb->bits[targetIndex + 1] |= g_frameBuffer[sourceIndex + greenIndex] >> 5;
			pfb->bits[targetIndex] = (g_frameBuffer[sourceIndex + greenIndex] & 0x1C) << 3;
			pfb->bits[targetIndex] |= g_frameBuffer[sourceIndex + blueIndex] >> 3;
		}			
	}

	pfb->var_info.bits_per_pixel = 16;
	pfb->var_info.red.offset = 0;
	pfb->var_info.red.length = 5;
	pfb->var_info.green.offset = 5;
	pfb->var_info.green.length = 6;
	pfb->var_info.blue.offset = 11;
	pfb->var_info.blue.length = 5;
}

void GetScreenDataNormal( FBInfo* pfb )
{
	pfb->bits = (char*) malloc(FBVarDataSize(pfb));	

	int offset = 0;/*FBOffSet(pfb);
	if((offset + FBVarDataSize(pfb)) > pfb->fix_info.smem_len)
		offset = 0;*/

	int var_line_length = pfb->var_info.xres * FBBytespp(pfb);
	if (var_line_length < pfb->fix_info.line_length)//去除屏幕右边剩余的虚拟宽度（G14，L710等手机屏幕实际像素小于虚拟宽度，会导致右边出现黑线）
	{
		int y;
		int srcOffset, desOffset;		

		for (y = 0; y < pfb->var_info.yres; y++)
		{
			desOffset = y * pfb->var_info.xres * FBBytespp(pfb);
			srcOffset = offset + y * pfb->fix_info.line_length;
			memcpy(pfb->bits + desOffset, g_frameBuffer + srcOffset, var_line_length);
		}
	}
	else
	{
		memcpy(pfb->bits, g_frameBuffer, FBVarDataSize(pfb));
	}
}



struct nd_data ScreenshotJpeg(struct nd_data* data_receive)
{
	struct nd_data data_result = {0, 0, NULL};
	FBInfo fb;
	memset(&fb, '\0', sizeof(FBInfo));
	int quality = ReadIntFromBuffer(data_receive);
	if (FBOpen(&fb, FB_PATH, FALSE) == -1)
	{
		ErrorNdsh("FBOpen(&fb, FB_PATH) fail!    errno: %d\n", errno);
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);

		return data_result;
	}
	int pSzie=0;
	//char out[1000000];
	BYTE* pDesBuffer =  NULL;
	int width=fb.var_info.xres,height=fb.var_info.yres;
	if(fb.var_info.bits_per_pixel == 32)
	{
		pDesBuffer= (BYTE*) malloc(width * height*3);
		int i=0,j=0;
		int redIndex = fb.var_info.red.offset >> 3;
		int greenIndex = fb.var_info.green.offset >> 3;
		int blueIndex = fb.var_info.blue.offset >> 3;	
        for (;j< width*height*4 ;i+=3,j+=4)  
        {  
                *(pDesBuffer+i)=*(fb.bits+j+redIndex);  
                *(pDesBuffer+i+1)=*(fb.bits+j+greenIndex);  
                *(pDesBuffer+i+2)=*(fb.bits+j+blueIndex);  
       }  
	}
	DebugNdsh("bmptojpg\n");
    bmptojpg(pDesBuffer,fb.bits,width ,height,&pSzie,quality);
	if(pDesBuffer)
	   free(pDesBuffer);
	//fb.bits=out;
	WriteIntToBuffer(&data_result, UNKNOW_ERROR);
	Write_var_screeninfo(&fb, &data_result);
	WriteStringToBuffer(&data_result, fb.bits, pSzie);
	//FBClose(&fb);
	close(fb.fd);
	//释放内存
	WriteIntToBufferInPos(&data_result, 0, SUCCESS);
	return data_result;
}



struct nd_data ScreenSize(struct nd_data* data_receive)
{
	//DebugNdsh("Screenshot()\n");
	struct nd_data data_result = {0, 0, NULL};

	FBInfo fb;
	memset(&fb, '\0', sizeof(FBInfo));
	if (FBOpen(&fb, FB_PATH, FALSE) == -1)
	{
		ErrorNdsh("FBOpen(&fb, FB_PATH) fail!    errno: %d\n", errno);
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);
		return data_result;
	}                         
    WriteIntToBuffer(&data_result, UNKNOW_ERROR);	
	WriteIntToBuffer(&data_result, fb.var_info.xres);
	WriteIntToBuffer(&data_result, fb.var_info.yres);
	FBClose(&fb);
	WriteIntToBufferInPos(&data_result, 0, SUCCESS);
	return data_result;
}


void bmptojpg(unsigned char *sourcedata,char *out,const int width,const int height,int *pSzie,int quality)
{
	int nAdjust;		// 用于字节对齐
	// 以下代码用于压缩，从本行开始
	struct jpeg_compress_struct jcs;
	struct jpeg_error_mgr jem;
	jcs.err = jpeg_std_error(&jem);
    //DebugNdsh("jpeg_create_compress\n");
	jpeg_create_compress(&jcs);
    //DebugNdsh("jpeg_stdio_dest\n");
	jpeg_stdio_dest(&jcs, out,pSzie);
	//jpeg_stdio_dest(&jcs,f);
	jcs.image_width = width; 			// 为图的宽和高，单位为像素 
	jcs.image_height = height;
	jcs.input_components = 3;		
	jcs.in_color_space = JCS_RGB;//JCS_YCCK,JCS_CMYK,JCS_RGB
	jpeg_set_defaults(&jcs);	
	jpeg_set_quality (&jcs, quality, TRUE);
    //DebugNdsh("jpeg_start_compress\n");
	jpeg_start_compress(&jcs, TRUE);
    //DebugNdsh("jpeg_OVER_compress\n");
	JSAMPROW row_pointer[1];			// 一行位图
	int row_stride= jcs.image_width*3;	// 每一行的字节数 
	// 对每一行进行压缩
	//DebugNdsh("jcs.image_height : %d\n", jcs.image_height);
	while (jcs.next_scanline < jcs.image_height) {
	    row_pointer[0] = (& sourcedata[(jcs.next_scanline) * (row_stride)]);
	    jpeg_write_scanlines(&jcs, row_pointer, 1);
	}
	
	jpeg_finish_compress(&jcs);
	jpeg_destroy_compress(&jcs);
}