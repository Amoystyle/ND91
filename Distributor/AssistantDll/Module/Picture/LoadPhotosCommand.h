#pragma once

namespace ND91Assistant
{
//  ����ͼƬ��Ϣ
class PictureData;
class LoadPhotosCommand :public TableQueryCommand
{
public:
	LoadPhotosCommand(const DeviceData* pDevice);
	virtual ~LoadPhotosCommand(void);

	// ִ������
	virtual void Execute();

	// Summary  : ���ȫ��ͼƬ��Ϣ
	// Returns	: const deque<PictureData*>
	const deque<PictureData*> GetPictures(){return _Pictures;}

	int GetPictureCounts();


protected:
	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

private:
	deque<PictureData*>  _Pictures;    // ͼƬ��Ϣ�б�

	bool  _getcount;

	int  _picCount;

};

}
