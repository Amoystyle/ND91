#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "IiPodMediaParser.h"
#include "Core/MP3Data.h"

using namespace std;

namespace ND91Assistant
{
	//mp3类型的ipod媒体数据操作类
	class ciPodMP3Paeser : public IiPodMediaParser//,public MP3Data
	{ 
		public:
			ciPodMP3Paeser(wstring path);
			virtual void OnDestroy();
			virtual bool IsVideo(){return false;}
			virtual long GetMediaDuration(){return _Mp3Data->GetMediaDuration();}
			virtual bool IsValid(){return _Mp3Data->IsValid();}	//可识别文件
			virtual std::wstring GetDevFileName() {return _Mp3Data->GetDevFileName();}			//设备上的文件名
			virtual std::wstring GetLocalFileName(){return _Mp3Data->GetLocalFileName();}		//本地文件名

			//只读属性
			virtual UINT GetFileSize(){return _Mp3Data->GetFileSize();}					 //文件大小
			virtual UINT GetPlayTime() {return _Mp3Data->GetPlayTime();}						 //播放时长
			virtual UINT GetiBitSpeed() {return _Mp3Data->GetiBitSpeed();}					 //码率
			virtual UINT GetSampleRate(){return _Mp3Data->GetSampleRate();}						 //采样率
			virtual UINT GetVolumeAdjustment(){return _Mp3Data->GetVolumeAdjustment();}					 //音量调整
			virtual bool GetSelectWorks(){return _Mp3Data->GetSelectWorks();}						 // 入选作品
			virtual std::wstring GetID3Ver(){return _Mp3Data->GetID3Ver();}					 //ID3版本
			virtual std::wstring GetChannel(){return _Mp3Data->GetChannel();}					 //通道数
			virtual std::wstring GetFormat(){return _Mp3Data->GetFormat();}					 //音频格式

			//读写信息属性
			virtual std::wstring PropertyGetMusicName(){return _Mp3Data->PropertyGetMusicName();}					//获取音乐标题
			virtual bool PropertySetMusicName(std::wstring val){return _Mp3Data->PropertySetMusicName(val);}			//设置音乐标题
			virtual std::wstring PropertyGetMusicPerformer(){return _Mp3Data->PropertyGetMusicPerformer();}				//获取表演者
			virtual bool PropertySetMusicPerformer(std::wstring val){return _Mp3Data->PropertySetMusicPerformer(val);}		//设置表演者
			virtual std::wstring PropertyGetAlbumPerformer(){return _Mp3Data->PropertyGetAlbumPerformer();}				//获取专辑表演者
			virtual bool PropertySetAlbumPerformer(std::wstring val){return _Mp3Data->PropertySetAlbumPerformer(val);}		//设置专辑表演者
			virtual std::wstring PropertyGetComposer(){return _Mp3Data->PropertyGetComposer();}				//获取作曲者
			virtual bool PropertySetComposer(std::wstring val){return _Mp3Data->PropertySetComposer(val);}			//设置作曲者
			virtual std::wstring PropertyGetAlbumName(){return _Mp3Data->PropertyGetAlbumName();}					//取专辑名
			virtual bool PropertySetAlbumName(std::wstring val){return _Mp3Data->PropertySetAlbumName(val);}			//设置专辑名
			virtual std::wstring PropertyGetLyrics(){return _Mp3Data->PropertyGetLyrics();}					//歌词
			virtual std::wstring PropertyGetTrackNumber(){return _Mp3Data->PropertyGetTrackNumber();}					//轨道号
			virtual std::wstring PropertyGetCDNumber() {return _Mp3Data->PropertyGetCDNumber();}						//cd号
			virtual std::wstring PropertyGetYear(){return _Mp3Data->PropertyGetYear();}						//年份		
			virtual std::wstring PropertyGetNotes(){return _Mp3Data->PropertyGetNotes();}						//注释	
			virtual std::wstring PropertyGetClassify(){return _Mp3Data->PropertyGetClassify();}					//分类
			virtual std::wstring PropertyGetStyle(){return _Mp3Data->PropertyGetStyle();}						//风格
			virtual std::wstring PropertyGetDescription(){return _Mp3Data->PropertyGetDescription();}					//描述
			virtual std::wstring PropertyGetBPM(){return _Mp3Data->PropertyGetBPM();}							//BMP
			virtual std::wstring PropertyGetNameSort(){return _Mp3Data->PropertyGetNameSort();}					//名称排序
			virtual std::wstring PropertyGetAlbumSort(){return _Mp3Data->PropertyGetAlbumSort();}					//专辑名排序
			virtual std::wstring PropertyGetPerformerSort(){return _Mp3Data->PropertyGetPerformerSort();}				//表演者排序
			virtual std::wstring PropertyGetAlbumPerformerSort() {return _Mp3Data->PropertyGetAlbumPerformerSort();}			//专辑表演者排序
			virtual std::wstring PropertyGetComposerSort() {return _Mp3Data->PropertyGetComposerSort();}				//作曲者排序
			virtual std::wstring PropertyGetMediaTypes_Prodcast() {return _Mp3Data->PropertyGetMediaTypes_Prodcast();}			//Prodcast 的文件类型
			virtual std::wstring PropertyGetMediaTypes_iTunesU() {return _Mp3Data->PropertyGetMediaTypes_iTunesU();}			//iTunesU的文件类型
			virtual UINT PropertyGetVolumeAdjustment() {return _Mp3Data->PropertyGetVolumeAdjustment();}						//音量调整
			virtual bool PropertyGetSelectWorks(){return _Mp3Data->PropertyGetSelectWorks();}							//是否入选专辑
			virtual bool PropertySetSelectWorks(bool val) {return _Mp3Data->PropertySetSelectWorks(val);}					//设置是否入选专辑
			virtual std::string PropertyGetArtWork(){return _Mp3Data->PropertyGetArtWork();}						//取封面数据
			virtual bool PropertySetArtWork(std::string val) {return _Mp3Data->PropertySetArtWork(val);}				//设置封面数据
	private:
		MP3Data* _Mp3Data;
	};
}