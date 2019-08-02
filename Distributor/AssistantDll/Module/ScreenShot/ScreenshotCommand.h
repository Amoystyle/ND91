///////////////////////////////////////////////////////////
//  ScreenshotCommand.h
//  Implementation of the Class ScreenshotCommand
//  Created on:      01-����-2011 9:43:03
//  Original author: michael
///////////////////////////////////////////////////////////

#if !defined(EA_E5F5FD3B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
#define EA_E5F5FD3B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_

namespace ND91Assistant
{
    // ��������
    class ScreenshotCommand : public BaseCommand
    {

    public:

        ScreenshotCommand(const DeviceData* pDevice, wstring strSavePath);
        virtual ~ScreenshotCommand();

        // ִ������
        virtual void Execute();


		// Summary  : �����ĻͼƬ����������
		// Returns	: string
		string  GetScreenImageBinaryData(){return _ScreenImagebinaryData;}

#pragma region ���ĸ�������Command����Ҫ���ݣ��������ʽ������ʽ

		// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
        virtual std::string GetAndroidBodyData();

		// �����������ݰ�
		virtual void SetAndroidReturnData(std::string data);

        // ��IOS�ϻ�ȡ���������ļ�����ȫ·����
        virtual std::wstring GetIosResultFilePath();

		// ����ios�豸�ķ����ļ���������ɾ���ļ�
		virtual void SetIosReturnFile(std::wstring filename);

#pragma endregion
	
    private:
        //���������ظ�ʽ
        bool parseAndroidPackageHead( BinaryBufferReader* pReader );
        bool SaveScreenShot(wstring strFilePath);

        void WifiExecute();
        void AndroidUSBExecute();
        void IosUSBExecute();

        wstring _strSavePath;	// ȡ�õ���Ļ��ͼҪ���浽��PC�е�·��	

		string    _ScreenImagebinaryData;

    };
}

#endif // !defined(EA_E5F5FD3B_5D1C_40d9_9E43_EBB15DF4A14C__INCLUDED_)
