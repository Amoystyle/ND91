#pragma once

#pragma warning(disable : 4251)

#include "IosScreen.h"

namespace ND91Assistant
{
	typedef enum eInterfaceOrientation
	{
		eInterfaceOrientation_LandScape, 	/// 横屏
		eInterfaceOrientation_Porttrait 	/// 竖屏
	};

	class IIosIconDataOperation;

	//图标信息存储操作类
	class CLASSINDLL_CLASS_DECL IosIconSet
	{
		friend class IosScreenOperation;

    public:
		//************************************
		// Method:    IosIconSet ：构造函数
		// FullName:  IosIconSet::IosIconSet
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: void * pCFData ：通过服务获取到的图标信息的CFData值
		// Parameter: int nVer ：设备版本的数字表示
		// Parameter: bool biPad ：是否为IPAD
		// Parameter: eInterfaceOrientation eType ：横竖屏标识
		//************************************
		IosIconSet(void* pCFData, int nVer, bool biPad, eInterfaceOrientation eType);
		~IosIconSet(void);

        // Summary  : 交换屏幕
        // Returns  : void
        // Parameter: int pos1 要交换的屏幕在容器中的位置
        // Parameter: int pos2 被交换的屏幕在容器中的位置
        void ExchangeScreen(int pos1, int pos2);

        // Summary  : 添加屏幕（要将任意屏幕中的一个图标添加到新屏幕中）
        // Returns  : void
        // Parameter: int screenPos 任意屏幕在容器中的的位置
        // Parameter: IosIcon* addIcon 在新屏幕中添加的图标
        void AddScreen(int screenPos,IosIcon* addIcon);

        // Summary  : 同个屏幕移动图标位置（底部屏幕）
        // Returns  : void
        // Parameter: string iconId1 移动的图标Id
        // Parameter: string iconId2 被插入的图标Id
        void ExchangeIcon(string iconId1, string iconId2);

        // Summary  : 同个屏幕移动图标位置（普遍屏幕）
        // Returns  : void
        // Parameter: int screenPos 普通屏幕在容器中的的位置
        // Parameter: string iconId1 移动的图标Id
        // Parameter: string iconId2 被插入的图标Id
        void ExchangeIcon(int screenPos,string iconId1,string iconId2);

        // Summary  : 不同屏幕移动图标位置（普遍屏幕移动到底部屏幕）
        // Returns  : void
        // Parameter: int screenPos 普通屏幕在容器中的的位置
        // Parameter: string iconId 移动的图标Id
        // Parameter: int folderId 移动的文件夹在屏幕中的位置
        // Parameter: eIosIconType iconType 图标类型
        // Parameter: int pos 被插入的图标在底部屏幕的位置
        void ExchangeIconDiffent(int screenPos,string iconId,int folderId,eIosIconType iconType,int pos);

        // Summary  : 不同屏幕移动图标位置（底部屏幕移动到普遍屏幕）
        // Returns  : void
        // Parameter: int pos 移动的图标在底部屏幕的位置
        // Parameter: string iconId 被插入的图标Id
        void ExchangeIconDiffent(int pos,string iconId);

        // Summary  : 不同屏幕移动图标位置（普遍屏幕移动到普遍屏幕）
        // Returns  : void
        // Parameter: int screenPos1 屏幕在容器中的的位置
        // Parameter: string iconId1 移动的图标Id
        // Parameter: int screenPos2 屏幕在容器中的的位置
        // Parameter: string iconId2 被插入的图标Id
        void ExchangeIconDiffent(string iconId1,string iconId2);

        // Summary  : 移动图标到文件夹(只支持4.X以上)
        // Returns  : void
        // Parameter: int screenPos1 屏幕在容器中的的位置
        // Parameter: string iconId 图标Id
        // Parameter: int screenPos2 文件夹所在的屏幕在容器中的的位置
        // Parameter: int folderId 文件夹在屏幕中的位置
        void MoveToFolder(int screenPos1,string iconId,int screenPos2,int folderId);

        // Summary  : 将图标从文件夹移出(只支持4.X以上)
        // Returns  : void
        // Parameter: string iconId2 图标Id
        // Parameter: int screenPos1 文件夹所在的屏幕在容器中的的位置
        // Parameter: int folderId 文件夹在屏幕中的位置
        // Parameter: int screenPos2 图标要移到的屏幕在容器中的的位置
        void MoveFromFolder(string iconId,int screenPos1,int folderId,int screenPos2);

        // Summary  : 添加文件夹（要将任意屏幕中的一个图标添加到新文件夹中）(只支持4.X以上)
        // Returns  : void
        // Parameter: int screenPos 任意屏幕在容器中的的位置
        // Parameter: string iconId1 图标Id
        // Parameter: string iconId2 图标Id
        // Parameter: wstring folderName 文件夹的名称
        void AddFolder(int screenPos,string iconId1,string iconId2,wstring folderName);

    private:
        // Summary  : 获取图标在屏幕的行列位置
        // Returns  : void
        // Parameter: int screenPos 屏幕在容器中的的位置
        // Parameter: string iconId 图标Id
        // Parameter: int* linePos 图标在屏幕的行位置
        // Parameter: int* rowPos 图标在屏幕的列位置
        void GetIconLineAndRow(int screenPos,string iconId,int* linePos,int* rowPos);

        // Summary  : 获取屏幕第一个空位
        // Returns  : void
        // Parameter: int screenPos 屏幕在容器中的的位置
        // Parameter: int* linePos 屏幕的行位置
        // Parameter: int* rowPos 屏幕的列位置
        void GetIconLineAndRow(int screenPos,int* linePos,int* rowPos);

        // Summary  : 获取文件夹在屏幕的行列位置
        // Returns  : void
        // Parameter: int folderId 文件夹在屏幕中的位置
        // Parameter: int* linePos 文件夹在屏幕的行位置
        // Parameter: int* rowPos 文件夹在屏幕的列位置
        void GetFolderLineAndRow(int folderId,int* linePos,int* rowPos);

    public:
		vector<IosScreen*> _vtScreen; //屏幕信息
		IosIcon* _BottomBar[6];	//底部图标信息
		int _nBottomBarMaxIconCount; //底部图标可存放的图标个数（IPAD为6个，其他为4个）
		int _nScreenLineCount; // 屏幕图标的行数
		int _nScreenRowCount; // 屏幕图标的列数
		wstring _strWallPaperFullName; // 屏幕的壁纸

	private:

		//打包数据成CFDATA，外层不需关心
		void* PackToCFData();

		IIosIconDataOperation* _pIosIconDataOperation; //图标数据操作类指针
		int _nVer; // 设备版本的数字表示
	};
}
