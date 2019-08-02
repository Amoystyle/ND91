#pragma once
#include "Command/Common/Media/RealMedia/MP3Data.h"
#include "Command/Common/Media/RealMedia/MP3Decoder.h"
#include <map>

using namespace std;

namespace ND91Assistant
{
	/*typedef enum eAvailableSetProperty
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
	};		*/

	//MP3文件信息管理类
	class MP3Manager
	{
	public:
		MP3Manager(wstring strFileFullName);
		~MP3Manager(void);

		//************************************
		// Method:    IsValidMp3 ：判断是否是可识别的MP3文件
		// FullName:  ND91Assistant::MP3Manager::IsValidMp3
		// Access:    public static 
		// Returns:   bool ：是则返回TRUE，否则返回FALSE，返回FALSE则不可再用此管理类进行操作
		// Qualifier:
		// Parameter: wstring strFileFullName ：MP3文件的本地文件名（含路径）
		//************************************
		static bool IsValidMp3(wstring strFileFullName);

		//************************************
		// Method:    GetMp3DataInfo ：获取MP3的信息
		// FullName:  ND91Assistant::MP3Manager::GetMp3DataInfo
		// Access:    public 
		// Returns:   const MP3Data* ：返回MP3信息，不允许外部自行修改
		// Qualifier:
		//************************************
		const MP3Data* GetMp3DataInfo();

		//************************************
		// Method:    ModifyProperty ：修改MP3属性信息，但不马上应用，DoModify才是应用，CancelModify可取消所有未应用的修改
		// FullName:  ND91Assistant::MP3Manager::ModifyProperty
		// Access:    public 
		// Returns:   void
		// Qualifier:
		// Parameter: eAvailableSetProperty eProperty ：欲修改属性的索引枚举值
		// Parameter: wstring strNewValue ：修改的MP3属性信息的新值
		//************************************
		void ModifyProperty(eAvailableSetProperty eProperty, wstring strNewValue);

		//************************************
		// Method:    CancelModify ：取消所有未应用的修改
		// FullName:  ND91Assistant::MP3Manager::CancelModify
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		void CancelModify();

		//************************************
		// Method:    DoModify ：应用所有修改的内容
		// FullName:  ND91Assistant::MP3Manager::DoModify
		// Access:    public 
		// Returns:   void
		// Qualifier:
		//************************************
		bool DoModify();

	private:
		UINT GetUintByType(ENUM_MP3_INFO_PARA eType);
		wstring GetWStringByType(ENUM_MP3_INFO_PARA eType);
		ENUM_MP3_INFO_PARA GetAvailableSetMp3InfoPara(eAvailableSetProperty eType);
		void SetAvailableSetMp3Info(eAvailableSetProperty eType, wstring strValue);

	private:
		MP3Data* _pMP3Data; //MP3的属性信息
		wstring _strFileFullName; //MP3文件名
		HANDLE _hMP3Handle; //与MP3Decoder衔接的操作MP3文件的HANDLE
 		map<eAvailableSetProperty, wstring> _mapSetInfo; //存放修改的属性及对应值
	};
}
