#pragma once

#include <string>

using namespace std;


namespace ND91Assistant
{
	class IiPodMediaParser
	{ 
	public:
		// 获取媒体文件播放时长
		virtual bool IsVideo()=0;
		virtual void OnDestroy() = 0;
		virtual long GetMediaDuration() = 0;
		virtual bool IsValid() = 0;		//可识别文件
		virtual std::wstring GetDevFileName() = 0;			//设备上的文件名
		virtual std::wstring GetLocalFileName() = 0;		//本地文件名

		//只读属性
		virtual /*UINT*/unsigned int GetFileSize() = 0;						 //文件大小
		virtual /*UINT*/unsigned int GetPlayTime() = 0;						 //播放时长
		virtual /*UINT*/unsigned int GetiBitSpeed() = 0;					 //码率
		virtual /*UINT*/unsigned int GetSampleRate() = 0;					 //采样率
		virtual /*UINT*/unsigned int GetVolumeAdjustment() = 0;				 //音量调整
		virtual bool GetSelectWorks() = 0;					 // 入选作品
		virtual std::wstring GetID3Ver() = 0;				 //ID3版本
		virtual std::wstring GetChannel() = 0;				 //通道数
		virtual std::wstring GetFormat() = 0;				 //音频格式

		//读写信息属性
		virtual std::wstring PropertyGetMusicName() = 0;				//获取音乐标题
		virtual bool PropertySetMusicName(std::wstring val)  = 0;		//设置音乐标题
		virtual std::wstring PropertyGetMusicPerformer()  = 0;			//获取表演者
		virtual bool PropertySetMusicPerformer(std::wstring val)  = 0;	//设置表演者
		virtual std::wstring PropertyGetAlbumPerformer()  = 0;			//获取专辑表演者
		virtual bool PropertySetAlbumPerformer(std::wstring val)  = 0;	//设置专辑表演者
		virtual std::wstring PropertyGetComposer() = 0;			//获取作曲者
		virtual bool PropertySetComposer(std::wstring val)  = 0;		//设置作曲者
		virtual std::wstring PropertyGetAlbumName()  = 0;				//取专辑名
		virtual bool PropertySetAlbumName(std::wstring val)  = 0;		//设置专辑名
		virtual std::wstring PropertyGetLyrics() = 0;				//歌词
		virtual std::wstring PropertyGetTrackNumber()  = 0;				//轨道号
		virtual std::wstring PropertyGetCDNumber() = 0;					//cd号
		virtual std::wstring PropertyGetYear()  = 0;					//年份		
		virtual std::wstring PropertyGetNotes()  = 0;					//注释	
		virtual std::wstring PropertyGetClassify()  = 0;				//分类
		virtual std::wstring PropertyGetStyle()  = 0;					//风格
		virtual std::wstring PropertyGetDescription() = 0;				//描述
		virtual std::wstring PropertyGetBPM()  = 0;						//BMP
		virtual std::wstring PropertyGetNameSort()  = 0;				//名称排序
		virtual std::wstring PropertyGetAlbumSort()  = 0;				//专辑名排序
		virtual std::wstring PropertyGetPerformerSort() = 0;			//表演者排序
		virtual std::wstring PropertyGetAlbumPerformerSort()  = 0;		//专辑表演者排序
		virtual std::wstring PropertyGetComposerSort()  = 0;			//作曲者排序
		virtual std::wstring PropertyGetMediaTypes_Prodcast()  = 0;		//Prodcast 的文件类型
		virtual std::wstring PropertyGetMediaTypes_iTunesU() = 0;		//iTunesU的文件类型
		virtual /*UINT*/unsigned int PropertyGetVolumeAdjustment() = 0;					//音量调整
		virtual bool PropertyGetSelectWorks() = 0;						//是否入选专辑
		virtual bool PropertySetSelectWorks(bool val) = 0;				//设置是否入选专辑
		virtual std::string PropertyGetArtWork() = 0;					//取封面数据
		virtual bool PropertySetArtWork(std::string val) = 0;			//设置封面数据
	};
}
