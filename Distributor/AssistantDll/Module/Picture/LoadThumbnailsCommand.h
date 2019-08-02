#pragma once

namespace ND91Assistant
{
	// ��������ͼ��Ϣ
	class ThumbnailData;
	class LoadThumbnailsCommand :public TableQueryCommand
	{
	public:
		LoadThumbnailsCommand(const DeviceData* pDevice);
		virtual ~LoadThumbnailsCommand(void);

		// ִ������
		virtual void Execute();

		// Summary  : �������ͼ��Ϣ���
		// Returns	: deque<ThumbnailData*>
		deque<ThumbnailData*> GetThumbNails(){return _thumbnails;}

        const deque<ThumbnailData*>* GetData()
        {
            return &_thumbnails;
        }

	protected:
		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);
	private:

		deque<ThumbnailData*>   _thumbnails;  // ����ͼ��Ϣ�б�

	};

}
