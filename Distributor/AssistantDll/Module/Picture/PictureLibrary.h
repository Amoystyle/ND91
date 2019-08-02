#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

#pragma warning(disable : 4251)

namespace ND91Assistant
{
    class PictureData;

	// �洢ͼ���е�����ͼƬ
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

		// Summary  : ���һ��ͼ��ͼƬʱ����ļ���������ͼλ��
		// Parameter: wstring & number		���ļ�������
		// Parameter: int & thumbIndex		����ͼλ��   (�����5.0.0���ϰ汾)
		void GetMaxFileData(int& number, int& thumbIndex);

        void GetAllPictures(vector<PictureData*>* pDeque) const;

        int Size() const { return _library.size(); }

        void Clear();
    private:
        map<int, PictureData*>    _library;
    };

}
