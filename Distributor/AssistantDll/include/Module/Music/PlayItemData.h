#pragma once
#include "Module/BaseData.h"
#include <deque>

using namespace std;
namespace ND91Assistant
{

class BinaryBufferReader;
class PlaylistData;
//播放列表项
class CLASSINDLL_CLASS_DECL PlayItemData : public BaseData
{
	friend class LoadPlayItemsCommand;
	friend class PlaylistOperationCommand;
public:
	PlayItemData(void);
	virtual ~PlayItemData(void);
    
	int  _id;      // id
	int  _trackId; // 曲目id
	int  _index;   // 索引

	PlaylistData*  _owner;//所属播放列表

private:
	// Summary  : 解析数据
	// Returns	: bool
	// Parameter: BinaryBufferReader & reader
	bool ParseAndroidData(BinaryBufferReader& reader);
};

}
