#include "pch_module.h"

#if defined( PLATFORM_IOS ) && defined( MODULE_APP )

#include "Module/Application/IosIcon/IosIconSet.h"
#include "Module/Application/IosIcon/IosIconDataOperation3.h"
#include "Module/Application/IosIcon/IosIconDataOperation4.h"

IosIconSet::IosIconSet(void* pCFData, int nVer, bool biPad, eInterfaceOrientation eType)
{
    _strWallPaperFullName = L"";
    _nVer = nVer;
    _nBottomBarMaxIconCount = 4;
    _nScreenLineCount = 4;
    _nScreenRowCount = 4;
    if (biPad)
    {
        _nBottomBarMaxIconCount = 6;
        if (eType == eInterfaceOrientation_LandScape)
            _nScreenRowCount = 5;
        else if (eType == eInterfaceOrientation_Porttrait)
            _nScreenLineCount = 5;
    }

    for (int i=0; i<6; i++)
        _BottomBar[i] = NULL;

    _pIosIconDataOperation = NULL;
    if (nVer >= 400)
        _pIosIconDataOperation = new IosIconDataOperation4(this);
    else if (nVer >= 300)
        _pIosIconDataOperation = new IosIconDataOperation3(this);

    if (_pIosIconDataOperation)
        _pIosIconDataOperation->PraseIconCFData(pCFData);

}

IosIconSet::~IosIconSet(void)
{
    vector<IosScreen*>::iterator it;
    for (it=_vtScreen.begin(); it!=_vtScreen.end(); it++)
    {
        if (*it)
            delete (*it);
        *it = NULL;
    }
    _vtScreen.clear();

    for (int i=0; i<6; i++)
    {
        if (_BottomBar[i])
            delete _BottomBar[i];
        _BottomBar[i] = NULL;
    }

    if (_pIosIconDataOperation)
    {
        delete _pIosIconDataOperation;
        _pIosIconDataOperation = NULL;
    }

    if (CFileOperation::IsFileExist(_strWallPaperFullName.c_str()))
        CFileOperation::DeleteFile(_strWallPaperFullName);
}

void* IosIconSet::PackToCFData()
{
    if (_pIosIconDataOperation)
        return _pIosIconDataOperation->PackIconCFData();

    return NULL;
}

void IosIconSet::ExchangeScreen(int pos1, int pos2)
{
    IosScreen* tempScreen   = _vtScreen.at(pos1);
    _vtScreen.at(pos1)      = _vtScreen.at(pos2);
    _vtScreen.at(pos2)      = tempScreen;
}

void IosIconSet::AddScreen(int screenPos,IosIcon* addIcon)
{
    int linePos = 0;
    int rowPos  = 0;
    // ��ȡͼ������Ļ������λ��
    GetIconLineAndRow(screenPos,addIcon->_strDisplayIndentifier,&linePos,&rowPos);

    if (screenPos == (int)_vtScreen.size())// �ײ���Ļ��ӵ�����Ļ
    {
        // ��ͼ���������Ļ
        IosScreen* newScreen = new IosScreen();
        _vtScreen.push_back(newScreen);

        IosIcon* pTemp = newScreen->_screenInfo[0][0];
        newScreen->_screenInfo[0][0] = _BottomBar[linePos];
        _BottomBar[linePos] = pTemp;
    }
    else
    {
        // ��ͼ���������Ļ
        IosScreen* newScreen = new IosScreen();
        _vtScreen.push_back(newScreen);

        IosIcon* pTemp = newScreen->_screenInfo[0][0];
        newScreen->_screenInfo[0][0] = _vtScreen.at(screenPos)->_screenInfo[linePos][rowPos];
        _vtScreen.at(screenPos)->_screenInfo[linePos][rowPos] = pTemp;
    }
}

void IosIconSet::GetIconLineAndRow(int screenPos,string iconId,int* linePos,int* rowPos)
{
    if (screenPos == (int)_vtScreen.size())// �ײ���Ļ��ӵ�����Ļ
    {
        for (int i = 0; i < _nBottomBarMaxIconCount; i++)
            if(_BottomBar[i] != NULL && _BottomBar[i]->_strDisplayIndentifier == iconId)
                *linePos = i;
            else
                continue;
    }
    else
    {
        IosScreen* screen = _vtScreen.at(screenPos);
        for (int i = 0; i < _nScreenLineCount; i++)
        {
            for (int j = 0; j < _nScreenRowCount; j++)
            {
                IosIcon* icon = screen->_screenInfo[i][j];
                if (icon == NULL || icon->_strDisplayIndentifier != iconId)
                    continue;

                *linePos = i;
                *rowPos = j;
                return;
            }
        }
    }
}

void IosIconSet::ExchangeIcon(string iconId1, string iconId2)
{
    if (iconId1 == iconId2)
        return ;

    int a = 0;
    int b = 0;
    for (int i = 0; i < _nBottomBarMaxIconCount; i++)
        if(_BottomBar[i] != NULL && _BottomBar[i]->_strDisplayIndentifier == iconId1)
            a = i;
        else if (_BottomBar[i] != NULL && _BottomBar[i]->_strDisplayIndentifier == iconId2)
            b = i;
        else 
            continue;

    IosIcon* pTemp = _BottomBar[a];
    if (a > b) // ����ͼ�������ƶ�
    {
        for (int i = a; i >= b; i--)
            _BottomBar[i] = _BottomBar[i-1];
    }
    else // ����ͼ����ǰ�ƶ�
    {
        for (int i = a; i <= b; i++)
            _BottomBar[i] = _BottomBar[i+1];
    }
    _BottomBar[b] = pTemp;
}

void IosIconSet::ExchangeIcon(int screenPos,string iconId1,string iconId2)
{
    if (iconId1 == iconId2)
        return ;

    int linePos1 = 0;
    int rowPos1  = 0;
    int linePos2 = 0;
    int rowPos2  = 0;
    // ��ȡ�ƶ���ͼ��ͱ������ͼ������Ļ�е�����λ��
    GetIconLineAndRow(screenPos,iconId1,&linePos1,&rowPos1);
    GetIconLineAndRow(screenPos,iconId2,&linePos2,&rowPos2);

    IosScreen* screen = _vtScreen.at(screenPos);
    // ��һ����ͨ��Ļ��ͼ���������
    IosIcon* icons[1000];
    for (int i = 0; i < 1000; i++)
        icons[i] = NULL;

    int count = 0;
    for (int i = 0; i < _nScreenLineCount; i++)
        for (int j = 0; j < _nScreenRowCount; j++)
            icons[count++] = screen->_screenInfo[i][j];

//     int a = linePos1*_nScreenLineCount+rowPos1;
//     int b = linePos2*_nScreenLineCount+rowPos2;

    int a = linePos1*_nScreenRowCount+rowPos1;
    int b = linePos2*_nScreenRowCount+rowPos2;

    IosIcon* pTemp = icons[a];
    if (a > b) // ����ͼ�������ƶ�
        for (int i = a; i >= b; i--)
            icons[i] = icons[i-1];
    else // ����ͼ����ǰ�ƶ�
        for (int i = a; i <= b; i++)
            icons[i] = icons[i+1];
    icons[b] = pTemp;

    // �������е�ͼ��д����Ļ
    count = 0;
    for (int i = 0; i < _nScreenLineCount; i++)
        for (int j = 0; j < _nScreenRowCount; j++)
                _vtScreen.at(screenPos)->_screenInfo[i][j] = icons[count++];
}

void IosIconSet::ExchangeIconDiffent(int screenPos,string iconId,int folderId,eIosIconType iconType,int pos)
{
    int linePos = 0;
    int rowPos = 0;
    if (iconType == eIosIconType_Normal)
        // ��ȡͼ������Ļ������λ��
        GetIconLineAndRow(screenPos,iconId,&linePos,&rowPos);
    else 
        // ��ȡ�ļ�������Ļ������λ��
        GetFolderLineAndRow(folderId,&linePos,&rowPos);

    for (int i = 0; i < _nBottomBarMaxIconCount; i++)
    {
        IosIcon* icon = _BottomBar[i];
        if (icon != NULL)
            continue;

        _BottomBar[i] = _vtScreen.at(screenPos)->_screenInfo[linePos][rowPos];

        ExchangeIcon(_BottomBar[i]->_strDisplayIndentifier, _BottomBar[pos]->_strDisplayIndentifier);

        // ���ƶ���ͼ��λ����Ϊ��
        _vtScreen.at(screenPos)->_screenInfo[linePos][rowPos] = NULL; 
    }
}

void IosIconSet::ExchangeIconDiffent(int pos,string iconId)
{
    IosIcon* icons[1000];
    for (int i = 0; i < 1000; i++)
        icons[i] = NULL;

    // ��������Ļ��ͼ���������
    int count = 0;
    for (size_t x = 0; x < _vtScreen.size(); x++)
    {
        for (int i = 0; i < _nScreenLineCount; i++)
            for (int j = 0; j < _nScreenRowCount; j++)
                icons[count++] = _vtScreen.at(x)->_screenInfo[i][j];
    }

    // ��¼���һ��ͼ���λ��
    int lastIconPos = 0; 
    for (int i = _vtScreen.size()*_nScreenLineCount*_nScreenRowCount - 1; i >= 0; i--)
    {
        IosIcon* icon = icons[i];
        if (icon == NULL)
            continue;

        lastIconPos = i;
        break;
    }

    // �ҳ��������ͼ��ID��λ��
    int a = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (icons[i] == NULL || icons[i]->_strDisplayIndentifier != iconId)
            continue;

        a = i;
        break;
    }

    // ����ͼ�������ƶ�
    for (int i = lastIconPos+1; i >= a; i--)
        icons[i] = icons[i-1];

    // �������λ�÷����ƶ���ͼ��
    icons[a] = _BottomBar[pos];

    // �������ƶ�������Ļ���½�����Ļ
    int f = lastIconPos+1+1;
    int g = _vtScreen.size()*_nScreenLineCount*_nScreenRowCount;
    if (f > g)
        AddScreen((int)_vtScreen.size()-1,icons[lastIconPos+1]);

    // �������е�ͼ��д�ص���Ļ
    count = 0;
    for (size_t x = 0; x < _vtScreen.size(); x++)
    {
        for (int i = 0; i < _nScreenLineCount; i++)
            for (int j = 0; j < _nScreenRowCount; j++)
                _vtScreen.at(x)->_screenInfo[i][j] = icons[count++];
    }

    _BottomBar[pos] = NULL;
}

void IosIconSet::ExchangeIconDiffent(string iconId1,string iconId2)
{
    IosIcon* icons[1000];
    for (int i = 0; i < 1000; i++)
        icons[i] = NULL;

    // ��������Ļ��ͼ���������
    int count = 0;
    for (size_t x = 0; x < _vtScreen.size(); x++)
    {
        for (int i = 0; i < _nScreenLineCount; i++)
            for (int j = 0; j < _nScreenRowCount; j++)
                icons[count++] = _vtScreen.at(x)->_screenInfo[i][j];
    }

    // ��¼���һ��ͼ���λ��
    int lastIconPos = 0; 
    for (int i = _vtScreen.size()*_nScreenLineCount*_nScreenRowCount - 1; i >= 0; i--)
    {
        IosIcon* icon = icons[i];
        if (icon == NULL)
            continue;

        lastIconPos = i;
        break;
    }

    // �ҳ��ƶ���ͼ��ID��λ��
    int pos1 = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (icons[i] == NULL || icons[i]->_strDisplayIndentifier != iconId1)
            continue;

        pos1 = i;
        break;
    }

    // �ҳ��������ͼ��ID��λ��
    int pos2 = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (icons[i] == NULL || icons[i]->_strDisplayIndentifier != iconId2)
            continue;

        pos2 = i;
        break;
    }

    if (pos1 < pos2)// �ƶ����ڲ����ǰ�棬�ƶ���λ��Ϊ�գ������λ���Ժ��λ������ƶ����������ƶ�������Ļ���½�����Ļ
    {
        // ����ͼ�������ƶ�
        for (int i = lastIconPos+1; i >= pos2; i--)
            icons[i] = icons[i-1];

        // �������λ�÷����ƶ���ͼ��
        icons[pos2] = icons[pos1];
        // �ƶ���ͼ��λ���ÿ�
        icons[pos1] = NULL;

        // �������ƶ�������Ļ���½�����Ļ
        int f = lastIconPos+1+1;
        int g = _vtScreen.size()*_nScreenLineCount*_nScreenRowCount;
        if (f > g)
            AddScreen((int)_vtScreen.size()-1,icons[lastIconPos+1]);

        // �������е�ͼ��д�ص���Ļ
        count = 0;
        for (size_t x = 0; x < _vtScreen.size(); x++)
        {
            for (int i = 0; i < _nScreenLineCount; i++)
                for (int j = 0; j < _nScreenRowCount; j++)
                    _vtScreen.at(x)->_screenInfo[i][j] = icons[count++];
        }
    }
    else// �ƶ����ڲ���ĺ��棬����֮���λ����������ƶ�
    {
        IosIcon* pTemp = icons[pos1];
        for (int i = pos1; i >= pos2; i--)// ����ͼ�������ƶ�
            icons[i] = icons[i-1];
        icons[pos2] = pTemp;

        // �������е�ͼ��д�ص���Ļ
        count = 0;
        for (size_t x = 0; x < _vtScreen.size(); x++)
        {
            for (int i = 0; i < _nScreenLineCount; i++)
                for (int j = 0; j < _nScreenRowCount; j++)
                    _vtScreen.at(x)->_screenInfo[i][j] = icons[count++];
        }
    }
}

void IosIconSet::MoveToFolder(int screenPos1,string iconId,int screenPos2,int folderId)
{
    int linePos1 = 0;
    int rowPos1 = 0;
    int linePos2 = 0;
    int rowPos2 = 0;
    
    // ��ȡͼ������Ļ������λ��
    GetIconLineAndRow(screenPos1,iconId,&linePos1,&rowPos1);

    // ��ȡ�ļ�������Ļ������λ��
    GetFolderLineAndRow(folderId,&linePos2,&rowPos2);

    // �ƶ�ͼ�굽�ļ���
    IosFolderIcon* folder = (IosFolderIcon*)_vtScreen.at(screenPos2)->_screenInfo[linePos2][rowPos2];
    folder->_vtIconList.push_back(_vtScreen.at(screenPos1)->_screenInfo[linePos1][rowPos1]);
    _vtScreen.at(screenPos1)->_screenInfo[linePos1][rowPos1] = NULL;
}

void IosIconSet::GetFolderLineAndRow(int folderId,int* linePos,int* rowPos)
{
    int count = 0;
    for (int i = 0; i < _nScreenLineCount; i++)
    {
        for (int j = 0; j < _nScreenRowCount; j++)
        {
            count++;
            if (count == folderId+1)
            {
                *linePos = i;
                *rowPos = j;
                return ;
            }
        }
    }
}

void IosIconSet::MoveFromFolder(string iconId,int screenPos1,int folderId,int screenPos2)
{
    int linePos1 = 0;
    int rowPos1  = 0;
    // ��ȡ�ļ�������Ļ������λ��
    GetFolderLineAndRow(folderId,&linePos1,&rowPos1);

    int linePos2 = 0;
    int rowPos2  = 0;
    // ��ȡ��Ļ��һ����λ
    GetIconLineAndRow(screenPos2,&linePos2,&rowPos2);

    IosFolderIcon* pFolder = (IosFolderIcon*)_vtScreen.at(screenPos1)->_screenInfo[linePos1][rowPos1];

    int pos = 0;
    for (size_t i = 0; i < pFolder->_vtIconList.size(); i++)// Ҫ�Ƴ���ͼ�����ļ����е�λ��
    {
        IosIcon* icon = pFolder->_vtIconList.at(i);
        if (icon->_strDisplayIndentifier != iconId)
            continue;

        pos = i;
        break;
    }

    _vtScreen.at(screenPos2)->_screenInfo[linePos2][rowPos2] = pFolder->_vtIconList.at(pos);

    IosIcon* ic = pFolder->_vtIconList.at(pos);
    vector<IosIcon*>::iterator it = pFolder->_vtIconList.begin();
    while ( it != pFolder->_vtIconList.end() )
    {
        IosIcon* icon = *it;
        if (icon->_strDisplayIndentifier == ic->_strDisplayIndentifier)
            it = pFolder->_vtIconList.erase(it);
        else
            ++it;
    }

    if (pFolder->_vtIconList.size() == 0)
		_vtScreen.at(screenPos1)->_screenInfo[linePos1][rowPos1] = NULL;
}

void IosIconSet::GetIconLineAndRow(int screenPos,int* linePos,int* rowPos)
{
    IosScreen* screen = _vtScreen.at(screenPos);
    for (int i = 0; i < _nScreenLineCount; i++)
    {
        for (int j = 0; j < _nScreenRowCount; j++)
        {
            if (screen->_screenInfo[i][j] != NULL)
                continue;

            *linePos = i;
            *rowPos  = j;
            return ;
        }
    }
}

void IosIconSet::AddFolder(int screenPos,string iconId1,string iconId2,wstring folderName)
{
    int linePos1 = 0;
    int rowPos1  = 0;
    // ��ȡͼ������Ļ������λ��
    GetIconLineAndRow(screenPos,iconId1,&linePos1,&rowPos1);

    int linePos2 = 0;
    int rowPos2  = 0;
    // ��ȡͼ������Ļ������λ��
    GetIconLineAndRow(screenPos,iconId2,&linePos2,&rowPos2);

    IosFolderIcon* folder = new IosFolderIcon();
    folder->_iconModeDate = 0;
    folder->_strDisPlayName = CCodeOperation::UnicodeToGB2312(folderName);
    folder->_vtIconList.push_back(_vtScreen.at(screenPos)->_screenInfo[linePos1][rowPos1]);
    folder->_vtIconList.push_back(_vtScreen.at(screenPos)->_screenInfo[linePos2][rowPos2]);
    _vtScreen.at(screenPos)->_screenInfo[linePos1][rowPos1] = folder;
    _vtScreen.at(screenPos)->_screenInfo[linePos2][rowPos2] = NULL;
}

#endif
