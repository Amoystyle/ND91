#pragma once
#include "Monitor/ISubject.h"
#include "Core/DeviceData.h"
#include <string>

using namespace std;

namespace ND91Assistant
{
enum  ScaleType
{
	Scale_None,//������
	Scale_Half,//��Сһ��
	Scale_Twice, // 2��
	Scale_Fix  // ���ŵ�ָ����С
};

class ScreenImageData;
class ScreenShotSubject :public ISubject
{
public:
	ScreenShotSubject(const DeviceData* pDevice);
	virtual ~ScreenShotSubject(void);

	// Summary  : ����ˢ����Ϣ
	// Returns	: void
	void  SendFreshScreen();

	// Summary  : �����ĻͼƬ
	// Returns	: ScreenImageData*
	ScreenImageData* GetScreenImage(){return _pScreenImage;}

	// Summary  : ������ĻͼƬ����
	// Returns	: void
	// Parameter: string data
	void  SetScreenImageBinaryData(string data);

	// Summary  : ��ת���¸�״̬
	// Returns	: void
	void  NextRotate();

	// Summary  : ������������
	// Returns	: void
	// Parameter: int width  ָ����
	// Parameter: int height ָ����
	// Parameter: ScaleType type ��������
	void  SetScaleType(ScaleType type, int width=0, int height=0);

protected:

	// Summary  : �˳�ȫ��ʱ��Ϊ����ת������״̬
	// Returns	: void
	void  ResetScreenImageShow();

	const DeviceData*  _pDevice;

private:

	ScreenImageData*   _pScreenImage; // ��ǰ��ĻͼƬ����
};

}
