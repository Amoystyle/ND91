#pragma once
#include "Module/TableQueryCommand.h"
namespace ND91Assistant
{
	//����ͼ�����
class AndroidPhotoBucket;
class LoadPhotoBucketsCommand :public TableQueryCommand
{
public:
	LoadPhotoBucketsCommand(const DeviceData* pDevice);
	virtual ~LoadPhotoBucketsCommand(void);

	// ִ������
	virtual void Execute();

	// Summary  : ���ͼ������б�
	// Returns	: const deque<AndroidPhotoBucket*>
	const deque<AndroidPhotoBucket*> GetPhotoBuckets(){return _PhotoBuckets;}

protected:
	// �����������ݰ�
	virtual void SetAndroidReturnData(std::string data);

private:
	deque<AndroidPhotoBucket*>   _PhotoBuckets; //ͼ������б� 
};

}
