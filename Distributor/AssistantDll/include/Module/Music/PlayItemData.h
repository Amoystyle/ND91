#pragma once
#include "Module/BaseData.h"
#include <deque>

using namespace std;
namespace ND91Assistant
{

class BinaryBufferReader;
class PlaylistData;
//�����б���
class CLASSINDLL_CLASS_DECL PlayItemData : public BaseData
{
	friend class LoadPlayItemsCommand;
	friend class PlaylistOperationCommand;
public:
	PlayItemData(void);
	virtual ~PlayItemData(void);
    
	int  _id;      // id
	int  _trackId; // ��Ŀid
	int  _index;   // ����

	PlaylistData*  _owner;//���������б�

private:
	// Summary  : ��������
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	bool ParseAndroidData(BinaryBufferReader& reader);
};

}
