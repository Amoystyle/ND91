#pragma once
#include <map>
#include <string>
#include "cul.h"
using namespace std;
class CUL_CLASS_DECL PostParam
{
public:
	PostParam(void);
	~PostParam(void);
	//添加需要post的参数
	bool AddBuffers(string key,string value);
	//添加需要post的文件
	bool AddFiles(string key, string filePath);

	//************************************
	// Method:    SetHead
	// FullName:  PostParam::SetHead
	// Access:    public 
	// Returns:   void
	// Qualifier: 设置Http头
	// Parameter: string head
	//************************************
	void SetHead(string head);
	//获取Buffers
	const map<string,string>& GetBuffers();
	//获取Files
	const map<string,string>& GetFiles();

	const string& GetHeads();
private:

#pragma warning(disable:4251)
	map<string,string> mBuffers;
	map<string,string> mFiles;
#pragma warning(default:4251)

	string mHeads;
};

