#pragma once

#include "Module/BaseData.h"
#include <deque>
using namespace std;

namespace ND91Assistant
{
enum BucketType
{
	BucketType_Normal		= 0,  // 
	BucketType_Camera		= 1,  // ����� 
	BucketType_Wallpaper	= 2,  // ��ֽ
	BucketType_ScreenShot   = 3,  // ��Ļ��ͼ 
	BucketType_All			= 128,// ȫ��
	BucketType_Unset		= 255,// δ����
};
class PictureData;
class BinaryBufferReader;
// ͼƬ�������
class CLASSINDLL_CLASS_DECL PhotoBucketData :public BaseData
{
	friend class LoadPhotoBucketsCommand;
public:
	PhotoBucketData(void);
	virtual ~PhotoBucketData(void);
	PhotoBucketData(const PhotoBucketData* pBucket);

	// Summary  :��÷����µ�����ͼƬ��Ϣ
	// Returns	: const deque<PictureData*>ͼƬ��Ϣ����
	const deque<PictureData*> GetPictures(){return _pictures;}

	// Summary  : ����·���жϷ�������
	// Returns	: ND91Assistant::BucketType ��������
	// Parameter: wstring path ����·��
	static BucketType GetBucketTypeByPath(wstring path);

	// Summary  : ����һ��ͼƬ������
	// Returns	: void
	// Parameter: PictureData * pic �������ͼƬ
	virtual void InsertPicture(PictureData* pic){}

	// Summary  : �Ӷ������Ƴ�һ��ͼƬ
	// Returns	: bool ͼƬ���ڷ���true
	// Parameter: PictureData * pic Ҫ�Ƴ���ͼƬ
	virtual bool RemovePictureFromList(PictureData* pic){return false;}

	string    _Id;			// ����Id
	wstring   _DisplayName;	// ��������
	wstring   _path;		// ·��
	bool      _visible;		// �Ƿ�ɼ�

private:
	bool ParseAndroidData(BinaryBufferReader& reader);

protected:

#pragma warning(disable : 4251)     // ���ô˾���
	deque<PictureData*>  _pictures;//������ͼƬ
#pragma warning(disable : 4251)     // ���ô˾���

};

}
