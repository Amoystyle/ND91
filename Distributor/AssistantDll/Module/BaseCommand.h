#pragma once


#include "Module/BaseData.h"
#include "Monitor/ProgressSubject.h"
#include "Common/BinaryBufferWriter.h"
#include "Common/BinaryBufferReader.h"
#include "Core/DeviceInfo.h"
#include "Device/BaseIO.h"
#include "Device/IosPublicFunc.h"

namespace ND91Assistant
{
	class DeviceData;
	class BaseIO;

    // �������
    // ����ҵ��������ݣ��������ִ�������������ؽ��
    class BaseCommand : public ProgressSubject
    {
    public:
        // ���豸��Ϣ��IOʵ����ʼ��
        BaseCommand(const DeviceData* pDevice);
        virtual ~BaseCommand();

        // ȡ���豸��Ϣ
        const DeviceInfo* GetDeviceInfo();

        // ִ������
        virtual void Execute() = 0;

        // �ļ��Ѵ��ڣ�ѯ���û���δ���
        EXIST_ACTION AskUser(std::wstring destName, bool isDir = false);
        void SetRenameCopyAllAction() { _lastAction = RENAME_COPY_ALL; }

        CommandExecuteResult Result() { return _result; };
		bool Success() { return _success; };
		bool ExecuteSuccess() { return _executesuccess; };
    protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
		virtual std::string GetAndroidBodyData() = 0;

        // �����������ݰ�
        virtual void SetAndroidReturnData(std::string data) = 0;

        // ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
		// Ĭ�Ϸ���IOS_COMMOND_NORMAL_FOLDER·���£�����ԭ�����ļ�����CreatDaemonCypher���м��ܴ���
        virtual std::wstring GetIosCommandFilePath();

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
		// Ĭ����IOS_COMMOND_NORMAL_FOLDER·���£��ļ���Ϊԭ�����ļ���+".done"
        virtual std::wstring GetIosResultFilePath();

        // ����ios�ļ����ݡ�Ĭ��Ϊ��
        virtual std::wstring GetIosCommandFileContent();

        // ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
        virtual void SetIosReturnFile(std::wstring filename) = 0;


#pragma endregion

		// ����Android���ݰ��������������ͷŴ�ָ�룬��
        std::string GetAndroidDatagram();

		// ����Android���ص����ݰ�ͷ -- ǰ20���ֽ� (��׼)
		bool parseAndroidPackageHead(BinaryBufferReader* pReader);

		// ����Android���ص����ݰ�ͷ -- ǰ16���ֽ�
		bool parseAndroidPackageHead_Top16(BinaryBufferReader* pReader);

		// ����Android���ص����ݰ�ͷ -- ǰ12���ֽ�
		bool parseAndroidPackageHead_Top12(BinaryBufferReader* pReader);


        void DefaultExecute();

		// ȡIO��ָ�루���Ƕ���ҪIO�࣬��Ҫʱ�ٴ�����
		BaseIO* GetIO();
        
        
    protected:
        CommandExecuteResult _result;
		bool _success;          // ִ�н���Ƿ�ɹ�
		bool _executesuccess;   // ִ���Ƿ����

        /*const*/ DeviceData*	_pDevice;   // �豸ָ��

		std::wstring _strIosCmdFileName; //IOS�����ļ�������_strIosCmdFileName = L"kill." + CFileOperation::GetGuidW(); ������Ĺ��캯���и�ֵ

#pragma region ��׿�����ʽ��صı���

        // Android���͵İ�ͷ: ����ÿ������Ĺ��캯����ʼ��
        // ֻ��ʹ��AndroidDefine.h�ж�������ְ�ͷ֮һ
		const char*	_pHead;

        // Android���͵ı�־������ÿ������Ĺ��캯����ʼ��
        // ֻ��ʹ��enum ANDROID_COMMAND_FLAG��ֵ�������
		short	_nFlag;

        int     _nAndroidChannelID;         //��׿ҵ����
        int     _nAndroidCommandID;         //��׿������

		char	_pReturnHead[4];			// Android���صİ�ͷ
		short	_nReturnFlag;				// Android���صı�־
		short	_nReturnLongConnectSign;	// Android���صĳ����ӱ�־
		int		_nReturnChannelID;			// Android���ص�ҵ����
		int		_nReturnCommonType;			// Android���ص�ҵ������
		int		_nReturnBodySize;			// Android���ص��������С

#pragma endregion

	private:
		BaseIO*	_pBaseIO;		// IO�࣬������ͨ��GetIO()��ȡ

        EXIST_ACTION    _lastAction;    // �ļ��Ѵ���ʱ���û��ϴ�ѡ��Ķ���

	protected:
		std::wstring GetIosDoneInfoFromFileName();

	private:
		bool LookForResultFile( IIosFileSystem* pFileStream, std::wstring filePrefix, wstring& strInfo);

    };
}
