#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
    class PictureData;

	// 存储图库中的所有图片
    class PictureLibrary
    {
	public:
        PictureLibrary() {}
		~PictureLibrary();

        void Add(const PictureData* pData);

        void CreatePictureData(int pictureId, string groupId);

        void Del(const PictureData* pData);
        void Del(int id);

        PictureData* Get(int id);

		// Summary  : 添加一张图库图片时获得文件名和缩略图位置
		// Parameter: wstring & number		新文件名数字
		// Parameter: int & thumbIndex		缩略图位置   (仅针对5.0.0以上版本)
		void GetMaxFileData(int& number, int& thumbIndex);

        void GetAllPictures(vector<PictureData*>* pDeque) const;

        int Size() const { return _library.size(); }

        void Clear();
    private:
        map<int, PictureData*>    _library;
    };

}
