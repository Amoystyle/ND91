#pragma once

#include <string>

using namespace std;

namespace ND91Assistant
{
    // ���������Ŀ¼
    class IosRecordingPath
	{ 
	public:
        IosRecordingPath(const wstring workFolder, bool cracked);

		wstring GetLocalRecordingPath() const;			// ���ػ�������¼���ļ���Ŀ¼
		wstring GetLocalVoiceMemosDBPath() const;		// ���ػ���ϵͳ¼�����ݿ�·��


		wstring GetIosVoiceMemosDBPath() const;			// �ֻ�¼�����ݿ��ļ�·��

    private:
        const wstring	_workFolder;					// ��ʱ����Ŀ¼
		bool			_cracked;						// �Ƿ�Խ��
    };
}
