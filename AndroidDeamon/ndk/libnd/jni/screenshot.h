#pragma once

#include <sys/stat.h>
#include <sys/mman.h>
#include <buffer.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <errno.h>

#include "log.h"

#define FB_PATH "/dev/graphics/fb0"

#define FBWidth(pfb) ((pfb)->var_info.xres) //��
#define FBHeight(pfb) ((pfb)->var_info.yres) //��
#define FBBpp(pfb) ((pfb)->var_info.bits_per_pixel) //ÿ�����ص���ռ������λ��
#define FBBytespp(pfb) (FBBpp(pfb) >> 3) //ÿ�����ص���ռ�ֽ���
#define FBVarLineLen(pfb) (FBBytespp(pfb)*(pfb)->var_info.xres)
#define FBVarDataSize(pfb) ((pfb)->var_info.xres * (pfb)->var_info.yres * FBBytespp(pfb)) //varinfo���ݴ�С
#define FBFixDataSize(pfb) ((pfb)->fix_info.line_length * (pfb)->var_info.yres) //fixinfo���ݴ�С
#define FBOffSet(pfb) ((pfb)->var_info.xoffset * FBBytespp(pfb) + pfb->var_info.yoffset * FBVarLineLen(pfb)) //��ͼ��ʼλ��
#define SCREEN_BUFF_SIZE 1024 * 64
#define FBSize(fb) ((fb)->var_info.xres * (fb)->var_info.yres * (FBBpp(fb) >> 3))

struct _FBInfo
{
	int fd;
	char* bits;
	struct fb_fix_screeninfo fix_info;
	struct fb_var_screeninfo var_info;
};

typedef struct _FBInfo FBInfo;

int FBOpen(FBInfo* fb, const char* fbPath, BOOLEAN toBitMap565);
void FBClose(FBInfo* fb);
void GetScreenDataNormal(FBInfo* pfb);
void GetScreenDataRGB565(FBInfo* pfb);
struct nd_data Screenshot(struct nd_data* data_receive);
bool Screencat(FBInfo& fb);
bool SetFbInfo(FBInfo& fb, int format, int w, int h);
void Write_var_screeninfo(FBInfo* fb, struct nd_data* data);
void bmptojpg(unsigned char *sourcedata,char *out,const int width,const int height,int *pSzie,int quality);
struct nd_data ScreenshotJpeg(struct nd_data* data_receive );
struct nd_data ScreenSize(struct nd_data* data_receive);