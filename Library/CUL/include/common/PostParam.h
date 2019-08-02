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
	//�����Ҫpost�Ĳ���
	bool AddBuffers(string key,string value);
	//�����Ҫpost���ļ�
	bool AddFiles(string key, string filePath);

	//************************************
	// Method:    SetHead
	// FullName:  PostParam::SetHead
	// Access:    public 
	// Returns:   void
	// Qualifier: ����Httpͷ
	// Parameter: string head
	//************************************
	void SetHead(string head);
	//��ȡBuffers
	const map<string,string>& GetBuffers();
	//��ȡFiles
	const map<string,string>& GetFiles();

	const string& GetHeads();
private:

#pragma warning(disable:4251)
	map<string,string> mBuffers;
	map<string,string> mFiles;
#pragma warning(default:4251)

	string mHeads;
};

