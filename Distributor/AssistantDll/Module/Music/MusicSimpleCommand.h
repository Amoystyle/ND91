#pragma once
#include "Module/BaseCommand.h"

using namespace std;

namespace ND91Assistant
{

class AudioData;
class AlbumData;
// ������ר��������һЩ������
class MusicSimpleCommand : public BaseCommand
{
public:
	MusicSimpleCommand(const DeviceData* pDevice);
	virtual ~MusicSimpleCommand(void);

	// Summary  : �޸ĸ�����ר��������������
	// Returns	: bool
	// Parameter: AudioData * pTrack Ҫ�޸ĵĸ���
	// Parameter: wstring newAlbumName�޸ĺ�ר����
	// Parameter: wstring newArtist�޸ĺ���������
	bool ChangeAlbumArtist(AudioData* pTrack, wstring newAlbumName, wstring newArtist);

    // Summary  : ����������
    // Returns	: bool
    // Parameter: AudioData * pTrack
    // Parameter: wstring newName
    bool RenameTrack(AudioData* pTrack, wstring newName);

    // Summary  : �Ƴ�����
    // Returns	: bool �ɹ�ʧ��
    // Parameter: AudioData* pTrack ����
    bool RemoveTrack(AudioData* pTrack);

    // Summary  : ��ȡר������
    // Returns	: bool
    // Parameter: AlbumData* pAlbum ר��
    bool GetAlbumImage(AlbumData* pAlbum);

	// Summary  : ����ר������
	// Returns	: bool
	// Parameter: AlbumData * palbum
	// Parameter: const wstring remotepath
	bool SetAlbumImage(AlbumData* pAlbum, const wstring remotepath);

	// Summary  : �Ƴ�ר������
	// Returns	: bool
	// Parameter: AlbumData * palbum
	bool RemoveAlbumImage(AlbumData* pAlbum);

	// Summary  : ��ȡ��ռ�ܿռ�
	// Returns	: long
	long    GetTotalSpace();

	virtual void Execute();

protected:

#pragma region ��6��������Command����Ҫ���ݣ��������ʽ������ʽ

	// ����Android�����壬��GetAndroidDatagram()�����е��ñ�����
	virtual std::string GetAndroidBodyData();

	// Summary  : �����������ݰ�
	// Returns	: void
	// Parameter: std::string data
	virtual void SetAndroidReturnData(std::string data);

	// Summary  : �����������ݰ�ͷ
	// Returns	: bool
	// Parameter: BinaryBufferReader * pReader
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

	// ����͵�IOS�豸�ϵ��ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// ��IOS�ϻ�ȡ���������ļ�����ȫ·��, unicode��
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// ����ios�ļ��������ļ�����ȫ·��, unicode���������߸���ɾ���ļ�
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// ����ios�豸�ķ����ļ���ȫ·��, unicode����ɾ���ļ�
	virtual void SetIosReturnFile(std::wstring filename) { }

private:

	bool RemoveMusic();
	int  GetMusicAllSpace();
	bool ChangeAlbumCover();

	enum CommandType
	{
		ERemoveTrack		= 1, // �Ƴ�����
		RENAME_TRACK		= 3, // ����������
		CHANGE_ALBUM_ARTIST = 4, // �޸ĸ���ר��������������
		SET_ALBUM_IMAGE		= 14,// ����ר������
		REMOVE_ALBUM_IMAGE	= 15,// �Ƴ�ר������ 
		EGetSpace			= 23,// ��ÿռ��С
		EQueryAlbum			= 99 // ��ȡר������
	};

	CommandType _type;          // ��������
	AudioData*	_pTrack;		// ����������
	AlbumData*	_pAlbum;		// ������ר��
	wstring     _newName;       // ������������
	wstring     _ImagePath;     // ������ר��ͼ·��
	wstring     _newArtistname; // ������������������

	bool		_success;		// ���ؽ�����ɹ���־
	long		_size;			// ���ؽ�����ռ��С
};

}
