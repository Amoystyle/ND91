#pragma once

#pragma warning(disable : 4251)

#include "IosScreen.h"

namespace ND91Assistant
{
	typedef enum eInterfaceOrientation
	{
		eInterfaceOrientation_LandScape, 	/// ����
		eInterfaceOrientation_Porttrait 	/// ����
	};

	class IIosIconDataOperation;

	//ͼ����Ϣ�洢������
	class CLASSINDLL_CLASS_DECL IosIconSet
	{
		friend class IosScreenOperation;

    public:
		//************************************
		// Method:    IosIconSet �����캯��
		// FullName:  IosIconSet::IosIconSet
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: void * pCFData ��ͨ�������ȡ����ͼ����Ϣ��CFDataֵ
		// Parameter: int nVer ���豸�汾�����ֱ�ʾ
		// Parameter: bool biPad ���Ƿ�ΪIPAD
		// Parameter: eInterfaceOrientation eType ����������ʶ
		//************************************
		IosIconSet(void* pCFData, int nVer, bool biPad, eInterfaceOrientation eType);
		~IosIconSet(void);

        // Summary  : ������Ļ
        // Returns  : void
        // Parameter: int pos1 Ҫ��������Ļ�������е�λ��
        // Parameter: int pos2 ����������Ļ�������е�λ��
        void ExchangeScreen(int pos1, int pos2);

        // Summary  : �����Ļ��Ҫ��������Ļ�е�һ��ͼ����ӵ�����Ļ�У�
        // Returns  : void
        // Parameter: int screenPos ������Ļ�������еĵ�λ��
        // Parameter: IosIcon* addIcon ������Ļ����ӵ�ͼ��
        void AddScreen(int screenPos,IosIcon* addIcon);

        // Summary  : ͬ����Ļ�ƶ�ͼ��λ�ã��ײ���Ļ��
        // Returns  : void
        // Parameter: string iconId1 �ƶ���ͼ��Id
        // Parameter: string iconId2 �������ͼ��Id
        void ExchangeIcon(string iconId1, string iconId2);

        // Summary  : ͬ����Ļ�ƶ�ͼ��λ�ã��ձ���Ļ��
        // Returns  : void
        // Parameter: int screenPos ��ͨ��Ļ�������еĵ�λ��
        // Parameter: string iconId1 �ƶ���ͼ��Id
        // Parameter: string iconId2 �������ͼ��Id
        void ExchangeIcon(int screenPos,string iconId1,string iconId2);

        // Summary  : ��ͬ��Ļ�ƶ�ͼ��λ�ã��ձ���Ļ�ƶ����ײ���Ļ��
        // Returns  : void
        // Parameter: int screenPos ��ͨ��Ļ�������еĵ�λ��
        // Parameter: string iconId �ƶ���ͼ��Id
        // Parameter: int folderId �ƶ����ļ�������Ļ�е�λ��
        // Parameter: eIosIconType iconType ͼ������
        // Parameter: int pos �������ͼ���ڵײ���Ļ��λ��
        void ExchangeIconDiffent(int screenPos,string iconId,int folderId,eIosIconType iconType,int pos);

        // Summary  : ��ͬ��Ļ�ƶ�ͼ��λ�ã��ײ���Ļ�ƶ����ձ���Ļ��
        // Returns  : void
        // Parameter: int pos �ƶ���ͼ���ڵײ���Ļ��λ��
        // Parameter: string iconId �������ͼ��Id
        void ExchangeIconDiffent(int pos,string iconId);

        // Summary  : ��ͬ��Ļ�ƶ�ͼ��λ�ã��ձ���Ļ�ƶ����ձ���Ļ��
        // Returns  : void
        // Parameter: int screenPos1 ��Ļ�������еĵ�λ��
        // Parameter: string iconId1 �ƶ���ͼ��Id
        // Parameter: int screenPos2 ��Ļ�������еĵ�λ��
        // Parameter: string iconId2 �������ͼ��Id
        void ExchangeIconDiffent(string iconId1,string iconId2);

        // Summary  : �ƶ�ͼ�굽�ļ���(ֻ֧��4.X����)
        // Returns  : void
        // Parameter: int screenPos1 ��Ļ�������еĵ�λ��
        // Parameter: string iconId ͼ��Id
        // Parameter: int screenPos2 �ļ������ڵ���Ļ�������еĵ�λ��
        // Parameter: int folderId �ļ�������Ļ�е�λ��
        void MoveToFolder(int screenPos1,string iconId,int screenPos2,int folderId);

        // Summary  : ��ͼ����ļ����Ƴ�(ֻ֧��4.X����)
        // Returns  : void
        // Parameter: string iconId2 ͼ��Id
        // Parameter: int screenPos1 �ļ������ڵ���Ļ�������еĵ�λ��
        // Parameter: int folderId �ļ�������Ļ�е�λ��
        // Parameter: int screenPos2 ͼ��Ҫ�Ƶ�����Ļ�������еĵ�λ��
        void MoveFromFolder(string iconId,int screenPos1,int folderId,int screenPos2);

        // Summary  : ����ļ��У�Ҫ��������Ļ�е�һ��ͼ����ӵ����ļ����У�(ֻ֧��4.X����)
        // Returns  : void
        // Parameter: int screenPos ������Ļ�������еĵ�λ��
        // Parameter: string iconId1 ͼ��Id
        // Parameter: string iconId2 ͼ��Id
        // Parameter: wstring folderName �ļ��е�����
        void AddFolder(int screenPos,string iconId1,string iconId2,wstring folderName);

    private:
        // Summary  : ��ȡͼ������Ļ������λ��
        // Returns  : void
        // Parameter: int screenPos ��Ļ�������еĵ�λ��
        // Parameter: string iconId ͼ��Id
        // Parameter: int* linePos ͼ������Ļ����λ��
        // Parameter: int* rowPos ͼ������Ļ����λ��
        void GetIconLineAndRow(int screenPos,string iconId,int* linePos,int* rowPos);

        // Summary  : ��ȡ��Ļ��һ����λ
        // Returns  : void
        // Parameter: int screenPos ��Ļ�������еĵ�λ��
        // Parameter: int* linePos ��Ļ����λ��
        // Parameter: int* rowPos ��Ļ����λ��
        void GetIconLineAndRow(int screenPos,int* linePos,int* rowPos);

        // Summary  : ��ȡ�ļ�������Ļ������λ��
        // Returns  : void
        // Parameter: int folderId �ļ�������Ļ�е�λ��
        // Parameter: int* linePos �ļ�������Ļ����λ��
        // Parameter: int* rowPos �ļ�������Ļ����λ��
        void GetFolderLineAndRow(int folderId,int* linePos,int* rowPos);

    public:
		vector<IosScreen*> _vtScreen; //��Ļ��Ϣ
		IosIcon* _BottomBar[6];	//�ײ�ͼ����Ϣ
		int _nBottomBarMaxIconCount; //�ײ�ͼ��ɴ�ŵ�ͼ�������IPADΪ6��������Ϊ4����
		int _nScreenLineCount; // ��Ļͼ�������
		int _nScreenRowCount; // ��Ļͼ�������
		wstring _strWallPaperFullName; // ��Ļ�ı�ֽ

	private:

		//������ݳ�CFDATA����㲻�����
		void* PackToCFData();

		IIosIconDataOperation* _pIosIconDataOperation; //ͼ�����ݲ�����ָ��
		int _nVer; // �豸�汾�����ֱ�ʾ
	};
}
