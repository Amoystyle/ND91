#pragma once
#include "AssistantDll.h"
#include "Module/Common/Media/IMediaParser.h"
#include "Common/CodeOperation.h"
#include "Common/StrOperation.h"
#include "MP3Decoder.h"

namespace ND91Assistant
{
	//id3标签类型
	typedef enum eAvailableSetProperty
	{
		eAvailableSetProperty_MusicName,						//名称
		eAvailableSetProperty_MusicPerformer,					//表演者
		eAvailableSetProperty_AlbumPerformer,					//专辑表演者
		eAvailableSetProperty_Composer,							//作曲者
		eAvailableSetProperty_AlbumName,						//专辑
		eAvailableSetProperty_Lyrics,							//歌词
		eAvailableSetProperty_FrontCover,						//封面
		eAvailableSetProperty_TrackNumber,						//轨道号码
		eAvailableSetProperty_CDNumber,							//光盘号码
		eAvailableSetProperty_Year,								//年份
		eAvailableSetProperty_Notes,							//注释
		eAvailableSetProperty_Classify,							//归类
		eAvailableSetProperty_Style,							//风格
		eAvailableSetProperty_Description,						//描述
		eAvailableSetProperty_BPM,								//BPM
		eAvailableSetProperty_SelectWorks,						//加入选集
		eAvailableSetProperty_NameSort,							//名称排序
		eAvailableSetProperty_AlbumSort,						//专辑排序
		eAvailableSetProperty_PerformerSort,					//表演者排序
		eAvailableSetProperty_AlbumPerformerSort,				//专辑表演者排序
		eAvailableSetProperty_ComposerSort,						//作曲者排序
		eAvailableSetProperty_VolumeAdjustment,					//音量调整
		eAvailableSetProperty_MediaTypes_Prodcast,				//媒体种类podcast
		eAvailableSetProperty_MediaTypes_iTunesU,				//媒体种类iTunes U	
	};		

	class MP3Data  : public IMediaParser
	{
	public:
		MP3Data(wstring strFileFullName);
		~MP3Data(void);

		// 获取媒体文件播放时长
		virtual long GetMediaDuration()	{ return _nPlayTime/1000; } 

	public:


		//************************************
		// Method:    IsValidMp3 ：判断是否是可识别的MP3文件
		// Access:    public static 
		// Returns:   bool ：是则返回TRUE，否则返回FALSE，返回FALSE则不可再用此管理类进行操作
		// Qualifier:
		// Parameter: wstring strFileFullName ：MP3文件的本地文件名（含路径）
		//************************************
		static bool IsValidMp3(wstring strFileFullName);

		//可识别文件
		bool IsValid(){return _bIsValid;}
		//设备上的文件名
		std::wstring GetDevFileName() const { return _strDevFileName; }
		void SetDevFileName(std::wstring val) { _strDevFileName = val; }
		//本地文件名
		std::wstring GetLocalFileName() const { return _strLocalFileName; }

		//Mp3文件只读属性
		/*UINT*/unsigned int GetFileSize() const { return _nFileSize; }
		/*UINT*/unsigned int GetPlayTime() const { return _nPlayTime; }
		/*UINT*/unsigned int GetiBitSpeed() const { return _niBitSpeed; }
		/*UINT*/unsigned int GetSampleRate() const { return _nSampleRate; }
		/*UINT*/unsigned int GetVolumeAdjustment() const { return _strVolumeAdjustment; }
		bool GetSelectWorks() const { return _bSelectWorks; }
		std::wstring GetID3Ver() const { return _strID3Ver; }
		std::wstring GetChannel() const { return _strChannel; }
		std::wstring GetFormat() const { return _strFormat; }
		

		//Mp3信息属性读写
		std::wstring PropertyGetMusicName() const { return CCodeOperation::StrFilter(_strMusicName); }
		bool PropertySetMusicName(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_MusicName,val);}

		std::wstring PropertyGetMusicPerformer() const { return CCodeOperation::StrFilter(_strMusicPerformer); }
		bool PropertySetMusicPerformer(std::wstring val) {return ModifyMp3Info(eAvailableSetProperty_MusicPerformer,val); }

		std::wstring PropertyGetAlbumPerformer() const { return CCodeOperation::StrFilter(_strAlbumPerformer); }
		bool PropertySetAlbumPerformer(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_AlbumPerformer,val);}

		std::wstring PropertyGetComposer() const { return CCodeOperation::StrFilter(_strComposer); }
		bool PropertySetComposer(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_Composer,val);}

		std::wstring PropertyGetAlbumName() const { return CCodeOperation::StrFilter(_strAlbumName); }
		bool PropertySetAlbumName(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_AlbumName,val);}

		std::wstring PropertyGetLyrics() const { return CCodeOperation::StrFilter(_strLyrics); }
		bool PropertySetLyrics(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_Lyrics,val); }

		std::wstring PropertyGetFrontCover() const { return CCodeOperation::StrFilter(_strFrontCover); }
		bool PropertySetFrontCover(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_FrontCover,val); }

		std::wstring PropertyGetTrackNumber() const { return _strTrackNumber; }
		bool PropertySetTrackNumber(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_TrackNumber,val);}

		std::wstring PropertyGetCDNumber() const { return _strCDNumber; }
		bool PropertySetCDNumber(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_CDNumber,val);}

		std::wstring PropertyGetYear() const { return CCodeOperation::StrFilter(_strYear); }
		bool PropertySetYear(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_Year,val);}

		std::wstring PropertyGetNotes() const { return CCodeOperation::StrFilter(_strNotes); }
		bool PropertySetNotes(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_Notes,val);}

		std::wstring PropertyGetClassify() const { return _strClassify; }
		bool PropertySetClassify(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_Classify,val);}

		std::wstring PropertyGetStyle() const { return _strStyle; }
		bool PropertySetStyle(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_Style,val);}

		std::wstring PropertyGetDescription() const { return CCodeOperation::StrFilter(_strDescription); }
		bool PropertySetDescription(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_Description,val);}

		std::wstring PropertyGetBPM() const { return _strBPM; }
		bool PropertySetBPM(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_BPM,val); }

		std::wstring PropertyGetNameSort() const { return CCodeOperation::StrFilter(_strNameSort); }
		bool PropertySetNameSort(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_NameSort,val); }

		std::wstring PropertyGetAlbumSort() const { return CCodeOperation::StrFilter(_strAlbumSort); }
		bool PropertySetAlbumSort(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_AlbumSort,val); }

		std::wstring PropertyGetPerformerSort() const { return CCodeOperation::StrFilter(_strPerformerSort); }
		bool PropertySetPerformerSort(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_PerformerSort,val);}

		std::wstring PropertyGetAlbumPerformerSort() const { return CCodeOperation::StrFilter(_strAlbumPerformerSort); }
		bool PropertySetAlbumPerformerSort(std::wstring val) { return  ModifyMp3Info(eAvailableSetProperty_AlbumPerformerSort,val);}

		std::wstring PropertyGetComposerSort() const { return CCodeOperation::StrFilter(_strComposerSort); }
		bool PropertySetComposerSort(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_ComposerSort,val); }

		std::wstring PropertyGetMediaTypes_Prodcast() const { return _strMediaTypes_Prodcast; }
		bool PropertySetMediaTypes_Prodcast(std::wstring val) {  return ModifyMp3Info(eAvailableSetProperty_MediaTypes_Prodcast,val);}

		std::wstring PropertyGetMediaTypes_iTunesU() const { return _strMediaTypes_iTunesU; }
		bool PropertySetMediaTypes_iTunesU(std::wstring val) { return ModifyMp3Info(eAvailableSetProperty_MediaTypes_iTunesU,val); }

		/*UINT*/unsigned int PropertyGetVolumeAdjustment() const { return _strVolumeAdjustment; }
		bool PropertySetVolumeAdjustment(/*UINT*/unsigned int val) { return ModifyMp3Info(eAvailableSetProperty_VolumeAdjustment,CStrOperation::IntToWString(val)); }

		bool PropertyGetSelectWorks() const { return _bSelectWorks; }
		bool PropertySetSelectWorks(bool val) {return ModifyMp3Info(eAvailableSetProperty_VolumeAdjustment,val==true?L"1":L"0"); }

		std::string PropertyGetArtWork() const { return _ArtWork; }
		bool PropertySetArtWork(std::string val) { return ModifyMp3ArtWork(val); }

	private:
		//************************************
		// Method:    加载Mp3属性信息
		// Returns:   void
		//************************************
		void LoadMp3DataInfo(wstring Localpath);

		//************************************
		// Method:    编辑mp3属性
		// Returns:   void
		// Parameter: eAvailableSetProperty PropertyType  属性ID
		// Parameter: wstring val 属性值
		//************************************
		bool ModifyMp3Info(eAvailableSetProperty PropertyType,wstring val);

		//************************************
		// Method:    根据属性类型获得MP3属性返回值
		// Returns:   UINT 返回值
		// Parameter: ENUM_MP3_INFO_PARA eType 类型
		//************************************
		/*UINT*/unsigned int GetUintByType(ENUM_MP3_INFO_PARA eType);

		//************************************
		// Method:    根据属性类型获得MP3属性返回值
		// Returns:   std::wstring 返回值
		// Parameter: ENUM_MP3_INFO_PARA eType 类型
		//************************************
		wstring GetWStringByType(ENUM_MP3_INFO_PARA eType);

		//************************************
		// Method:    获得属性标签
		// Returns:   ENUM_MP3_INFO_PARA 标签名
		// Parameter: eAvailableSetProperty eType 得到的标签
		//************************************
		ENUM_MP3_INFO_PARA GetAvailableSetMp3InfoPara(eAvailableSetProperty eType);

		//获得mp3封面图片数据		
		string GetArtWorkData();

		//设置MP3封面图片信息
		bool ModifyMp3ArtWork(string val);

	private:
		wstring _strLocalFileName;		//本地文件全名（含路径）
		wstring _strDevFileName;		//手机文件全名（含路径）
		
		bool _bIsValid;					//是否有效的mp3文件
		/*UINT*/unsigned int _nFileSize;				//文件大小(单位字节)
		/*UINT*/unsigned int _nPlayTime;				//时长(单位ms)		
		/*UINT*/unsigned int _nSampleRate;				//采样速率（单位Hz）		
		/*UINT*/unsigned int _niBitSpeed;				//位速(BPS)		
		
			
		wstring _strID3Ver;				//ID3标记		
		wstring _strFormat;				//音频格式		
		wstring _strChannel;			//音频通道		

		bool _bSelectWorks;		//是否加入选集	
		
		/*UINT*/unsigned int _strVolumeAdjustment;	//音量调整

		wstring _strMusicName;			//名称
		wstring _strMusicPerformer;		//表演者		
		wstring _strAlbumPerformer;		//专辑表演者		
		wstring _strComposer;			//作曲者		
		wstring _strAlbumName;			//专辑名		
		wstring _strLyrics;				//歌词		
		wstring _strFrontCover;			//封面		
		wstring _strTrackNumber;		//轨道号码		
		wstring _strCDNumber;			//光盘号码		
		wstring _strYear;				//年份		
		wstring _strNotes;				//注释		
		wstring _strClassify;			//归类		
		wstring _strStyle;				//风格(为数字，具体含义未对应)		
		wstring _strDescription;		//描述		
		wstring _strBPM;				//BPM		
		wstring _strNameSort;			//名称排序		
		wstring _strAlbumSort;			//专辑排序		
		wstring _strPerformerSort;		//表演者排序		
		wstring _strAlbumPerformerSort;	//专辑表演者排序		
		wstring _strComposerSort;		//作曲者排序		
		wstring _strMediaTypes_Prodcast;//媒体种类podcast		
		wstring _strMediaTypes_iTunesU;	//媒体种类iTunes U	

		string _ArtWork;					//新增封面读取
		
		/*HANDLE*/void* _hMP3Handle; //与MP3Decoder衔接的操作MP3文件的HANDLE
		
	};
}
