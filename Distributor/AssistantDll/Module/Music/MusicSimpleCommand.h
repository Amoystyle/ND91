#pragma once
#include "Module/BaseCommand.h"

using namespace std;

namespace ND91Assistant
{

class AudioData;
class AlbumData;
// 歌曲和专辑操作的一些简单命令
class MusicSimpleCommand : public BaseCommand
{
public:
	MusicSimpleCommand(const DeviceData* pDevice);
	virtual ~MusicSimpleCommand(void);

	// Summary  : 修改歌曲的专辑名和艺术家名
	// Returns	: bool
	// Parameter: AudioData * pTrack 要修改的歌曲
	// Parameter: wstring newAlbumName修改后专辑名
	// Parameter: wstring newArtist修改后艺术家名
	bool ChangeAlbumArtist(AudioData* pTrack, wstring newAlbumName, wstring newArtist);

    // Summary  : 重命名歌曲
    // Returns	: bool
    // Parameter: AudioData * pTrack
    // Parameter: wstring newName
    bool RenameTrack(AudioData* pTrack, wstring newName);

    // Summary  : 移除歌曲
    // Returns	: bool 成功失败
    // Parameter: AudioData* pTrack 歌曲
    bool RemoveTrack(AudioData* pTrack);

    // Summary  : 获取专辑封面
    // Returns	: bool
    // Parameter: AlbumData* pAlbum 专辑
    bool GetAlbumImage(AlbumData* pAlbum);

	// Summary  : 设置专辑封面
	// Returns	: bool
	// Parameter: AlbumData * palbum
	// Parameter: const wstring remotepath
	bool SetAlbumImage(AlbumData* pAlbum, const wstring remotepath);

	// Summary  : 移除专辑封面
	// Returns	: bool
	// Parameter: AlbumData * palbum
	bool RemoveAlbumImage(AlbumData* pAlbum);

	// Summary  : 获取所占总空间
	// Returns	: long
	long    GetTotalSpace();

	virtual void Execute();

protected:

#pragma region 这6个方法是Command的主要内容，处理令格式与结果格式

	// 生成Android数据体，在GetAndroidDatagram()方法中调用本方法
	virtual std::string GetAndroidBodyData();

	// Summary  : 解析返回数据包
	// Returns	: void
	// Parameter: std::string data
	virtual void SetAndroidReturnData(std::string data);

	// Summary  : 解析返回数据包头
	// Returns	: bool
	// Parameter: BinaryBufferReader * pReader
	bool parseAndroidPackageHead(BinaryBufferReader* pReader);

	// 命令传送到IOS设备上的文件名（全路径, unicode）
	virtual std::wstring GetIosCommandFilePath(){return L"";}

	// 从IOS上获取的命令结果文件名（全路径, unicode）
	virtual std::wstring GetIosResultFilePath() {return L"";}

	// 生成ios文件。返回文件名（全路径, unicode），调用者负责删除文件
	virtual std::wstring GetIosCommandFileContent(){return L"";}

	// 解析ios设备的返回文件（全路径, unicode）并删除文件
	virtual void SetIosReturnFile(std::wstring filename) { }

private:

	bool RemoveMusic();
	int  GetMusicAllSpace();
	bool ChangeAlbumCover();

	enum CommandType
	{
		ERemoveTrack		= 1, // 移除歌曲
		RENAME_TRACK		= 3, // 重命名歌曲
		CHANGE_ALBUM_ARTIST = 4, // 修改歌曲专辑名和艺术家名
		SET_ALBUM_IMAGE		= 14,// 设置专辑封面
		REMOVE_ALBUM_IMAGE	= 15,// 移除专辑封面 
		EGetSpace			= 23,// 获得空间大小
		EQueryAlbum			= 99 // 获取专辑封面
	};

	CommandType _type;          // 命令类型
	AudioData*	_pTrack;		// 参数，歌曲
	AlbumData*	_pAlbum;		// 参数，专辑
	wstring     _newName;       // 参数，新名称
	wstring     _ImagePath;     // 参数，专辑图路径
	wstring     _newArtistname; // 参数，艺术家新名称

	bool		_success;		// 返回结果，成功标志
	long		_size;			// 返回结果，空间大小
};

}
