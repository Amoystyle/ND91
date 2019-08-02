#pragma once

#include "Module/BaseData.h"
#include <deque>
using namespace std;

namespace ND91Assistant
{
enum BucketType
{
	BucketType_Normal		= 0,  // 
	BucketType_Camera		= 1,  // 照相机 
	BucketType_Wallpaper	= 2,  // 壁纸
	BucketType_ScreenShot   = 3,  // 屏幕截图 
	BucketType_All			= 128,// 全部
	BucketType_Unset		= 255,// 未设置
};
class PictureData;
class BinaryBufferReader;
// 图片分类基类
class CLASSINDLL_CLASS_DECL PhotoBucketData :public BaseData
{
	friend class LoadPhotoBucketsCommand;
public:
	PhotoBucketData(void);
	virtual ~PhotoBucketData(void);
	PhotoBucketData(const PhotoBucketData* pBucket);

	// Summary  :获得分类下的所有图片信息
	// Returns	: const deque<PictureData*>图片信息队列
	const deque<PictureData*> GetPictures(){return _pictures;}

	// Summary  : 根据路径判断分类类型
	// Returns	: ND91Assistant::BucketType 分类类型
	// Parameter: wstring path 分类路径
	static BucketType GetBucketTypeByPath(wstring path);

	// Summary  : 插入一张图片到队列
	// Returns	: void
	// Parameter: PictureData * pic 带插入的图片
	virtual void InsertPicture(PictureData* pic){}

	// Summary  : 从队列中移除一张图片
	// Returns	: bool 图片存在返回true
	// Parameter: PictureData * pic 要移除的图片
	virtual bool RemovePictureFromList(PictureData* pic){return false;}

	string    _Id;			// 分类Id
	wstring   _DisplayName;	// 分类名称
	wstring   _path;		// 路径
	bool      _visible;		// 是否可见

private:
	bool ParseAndroidData(BinaryBufferReader& reader);

protected:

#pragma warning(disable : 4251)     // 禁用此警告
	deque<PictureData*>  _pictures;//包含的图片
#pragma warning(disable : 4251)     // 禁用此警告

};

}
