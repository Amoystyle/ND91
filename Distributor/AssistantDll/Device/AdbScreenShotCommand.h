#pragma once
#include "Device/AdbRemoteCommand.h"

namespace ND91Assistant
{
// usb ��������
class AdbScreenShotCommand :public AdbRemoteCommand
{
public:
	AdbScreenShotCommand(const wstring sn,wstring id);
	virtual ~AdbScreenShotCommand(void);

	// Summary  : �����Ļ��Ϣ
	// Returns	: string ����Ҫ����������
	// Parameter: int & bpp     pixelformat
	string  GetScreenImageInfoData(int& bpp);

	// Summary  : �����ĻͼƬ����������
	// Returns	: string
	string  GetScreenImageData();

protected:

	// ����ʵ��ʱ���������⴦��ֻ�����CommonExecute����
	virtual void InternalExecute();

	// ������������
	// ����ֵ��true ��ʾ�ɹ���false ��ʾʧ��
	virtual bool ParseReturnData()
	{
		return true;
	}

	bool  _IsInfo;

	int   _bpp;
};

}