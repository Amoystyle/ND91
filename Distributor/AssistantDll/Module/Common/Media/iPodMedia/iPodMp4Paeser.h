#pragma once

#include <string>
#include <fstream>
#include <vector>
using namespace std;
#include "IiPodMediaParser.h"
#include "Module/common/Media/RealMedia/Mp4Parser.h"
#include "Module/common/Media/MediaData.h"

namespace ND91Assistant
{
	//mp4类型的ipod媒体数据操作类
	class ciPodMP4Paeser : public IiPodMediaParser
	{ 
		public:
			ciPodMP4Paeser(wstring path);
			virtual void OnDestroy();
			virtual bool IsVideo(){return _Mp4Parser->GetMediaData()->_isVideo;}
			virtual long GetMediaDuration(){return _Mp4Parser->GetMediaDuration();}
			virtual bool IsValid(){return false;}	                    // 可识别文件
            virtual std::wstring GetDevFileName() {return L"";}			// 设备上的文件名
			virtual std::wstring GetLocalFileName(){return L"";}		// 本地文件名

			//只读属性
			virtual /*UINT*/unsigned int GetFileSize(){return _media->_fileSize;}			                     // 文件大小
			virtual /*UINT*/unsigned int GetPlayTime() {return _Mp4Parser->GetMediaDuration()*1000;}	                         // 播放时长
			virtual /*UINT*/unsigned int GetiBitSpeed() {return 0;}					                             // 码率
			virtual /*UINT*/unsigned int GetSampleRate(){return _media->_nSampleRate;}		                     // 采样率
			virtual /*UINT*/unsigned int GetVolumeAdjustment(){return 0;}					                     // 音量调整
			virtual bool GetSelectWorks(){return false;}						                 // 入选作品
			virtual std::wstring GetID3Ver(){return L"";}					                     // ID3版本
			virtual std::wstring GetChannel(){return L"";}					                     // 通道数
			virtual std::wstring GetFormat(){return L"";}					                     // 音频格式

			//读写信息属性
			virtual std::wstring PropertyGetMusicName(){return _media->_title;}	                    // 获取音乐标题
			virtual bool PropertySetMusicName(std::wstring val){return false;}			            // 设置音乐标题
			virtual std::wstring PropertyGetMusicPerformer(){return _media->_artist;}               // 获取表演者
			virtual bool PropertySetMusicPerformer(std::wstring val){return false;}		            // 设置表演者
			virtual std::wstring PropertyGetAlbumPerformer(){return L"";}				            // 获取专辑表演者
			virtual bool PropertySetAlbumPerformer(std::wstring val){return false;}		            // 设置专辑表演者
			virtual std::wstring PropertyGetComposer(){return L"";}				                    // 获取作曲者
			virtual bool PropertySetComposer(std::wstring val){return false;}			            // 设置作曲者
			virtual std::wstring PropertyGetAlbumName(){return _media->_album;}	                    // 取专辑名
			virtual bool PropertySetAlbumName(std::wstring val){return false;}			            // 设置专辑名
			virtual std::wstring PropertyGetLyrics(){return L"";}					                // 歌词
			virtual std::wstring PropertyGetTrackNumber(){return L"";}					            // 轨道号
			virtual std::wstring PropertyGetCDNumber() {return L"";}						        // cd号
			virtual std::wstring PropertyGetYear(){return L"";}						                // 年份		
			virtual std::wstring PropertyGetNotes(){return _media->_comment;}	                    // 注释	
			virtual std::wstring PropertyGetClassify(){return L"";}					                // 分类
			virtual std::wstring PropertyGetStyle(){return L"";}						            // 风格
			virtual std::wstring PropertyGetDescription(){return L"";}					            // 描述
			virtual std::wstring PropertyGetBPM(){return L"";}							            // BMP
			virtual std::wstring PropertyGetNameSort(){return L"";}					                // 名称排序
			virtual std::wstring PropertyGetAlbumSort(){return L"";}					            // 专辑名排序
			virtual std::wstring PropertyGetPerformerSort(){return L"";}				            // 表演者排序
			virtual std::wstring PropertyGetAlbumPerformerSort() {return L"";}			            // 专辑表演者排序
			virtual std::wstring PropertyGetComposerSort() {return L"";}				            // 作曲者排序
			virtual std::wstring PropertyGetMediaTypes_Prodcast() {return L"";}			            // Prodcast 的文件类型
			virtual std::wstring PropertyGetMediaTypes_iTunesU() {return L"";}			            // iTunesU的文件类型
			virtual /*UINT*/unsigned int PropertyGetVolumeAdjustment() {return 0;}						            // 音量调整
			virtual bool PropertyGetSelectWorks(){return false;}							        // 是否入选专辑
			virtual bool PropertySetSelectWorks(bool val) {return false;}					        // 设置是否入选专辑
			virtual std::string PropertyGetArtWork(){return "";}						            // 取封面数据
			virtual bool PropertySetArtWork(std::string val) {return false;}				        // 设置封面数据

    private:
        Mp4Parser*          _Mp4Parser;
        MediaData*			_media;				// 存放多媒体文件元素的数据类
	};
}