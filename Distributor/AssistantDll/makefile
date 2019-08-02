CC = g++ -g -finput-charset=GB2312 -I $(INC_PATH) -c $< -o obj/$@
INC_PATH=/Users/wh/AssistantDll/AssistantDll

91AssistantDll.o:Path.o BRMath.o DealTime.o md5.o SenvenZipOperation.o FileOperation.o 
##########################
FileOperation.o:AssistantDll.o StrOperation.o Version.o CodeOperation.o Log.o NDZlib.o #w3c.o
FileOperation.o:Common/FileOperation.cpp Common/FileOperation.h AssistantDll.h Common/StrOperation.h Common/Version.h Common/CodeOperation.h Common/Log.h Common/NDZlib.h #Common/w3c.h
	g++ -g -finput-charset=UTF-8 -I $(INC_PATH) -c $< -o obj/$@
	
#AssistantDll.o:NDMobileCommu.o
AssistantDll.o:AssistantDll.cpp AssistantDll.h Core/NDMobileCommu.h
	${CC}

NDMobileCommu.o:GlobalOption.o Log.o FileOperation.o
NDMobileCommu.o:Core/NDMobileCommu.cpp Core/NDMobileCommu.h Core/NDMMPubDef.h Core/GlobalOption.h Log.h AssistantException.h Common/FileOperation.h
	${CC}

GlobalOption.o:AssistantDll.o stdafx.o
GlobalOption.o:Core/GlobalOption.cpp Core/GlobalOption.h AssistantDll.h stdafx.h
	${CC}
	
stdafx.o:stdafx.cpp stdafx.h
	${CC}
	
Log.o:AssistantDll.o GlobalOption.o CodeOperation.o
Log.o:Common/Log.cpp Common/Log.h AssistantDll.h Core/GlobalOption.h Common/CodeOperation.h
	${CC}
	
StrOperation.o:AssistantDll.o
StrOperation.o:Common/StrOperation.cpp Common/StrOperation.h AssistantDll.h
	${CC}
	
Version.o:StrOperation.o
Version.o:Common/Version.cpp Common/Version.h Common/StrOperation.h
	${CC}

CodeOperation.o:AssistantDll.o StrOperation.o
CodeOperation.o:Common/CodeOperation.cpp Common/CodeOperation.h AssistantDll.h Common/SystemException.h Common/StrOperation.h
	${CC}
	
BaseException.o:Log.o CodeOperation.o
BaseException.o:Common/BaseException.h
	${CC}
	
NDZlib.o:zip.o unzip.o CodeOperation.o FileOperation.o
NDZlib.o:Common/NDZlib.cpp Common/NDZlib.h Common/zip/zip.h Common/zip/unzip.h Common/CodeOperation.h Common/FileOperation.h
	${CC}
	
zip.o:zutil.o
zip.o:Common/zip/zip.c Common/zip/zip.h Common/zip/zutil.h Common/zip/zlib.h
	${CC}
	
unzip.o:zutil.o
unzip.o:Common/zip/unzip.c Common/zip/unzip.h Common/zip/zutil.h Common/zip/zlib.h
	${CC}
	
zutil.o:Common/zip/zutil.c Common/zip/zutil.h Common/zip/zlib.h
	${CC}
	
#w3c.o:Common/w3c.cpp Common/w3c.h
#	${CC}

#################################
SenvenZipOperation.o:Alloc.o 7zFile.o LzmaDec.o LzmaEnc.o 7zAlloc.o 7zCrc.o 7zBuf.o 7zStream.o 7zIn.o FileOperation.o StrOperation.o CodeOperation.o
SenvenZipOperation.o:Common/SenvenZipOperation.cpp Common/SenvenZipOperation.h Common/7z/Alloc.h Common/7z/7zFile.h Common/7z/LzmaDec.h Common/7z/LzmaEnc.h Common/7z/7zAlloc.h Common/7z/7zCrc.h Common/CodeOperation.h Common/StrOperation.h Common/FileOperation.h Common/7z/7z.h Common/7z/7zVersion.h
	${CC}

7zStream.o:Common/7z/7zStream.c Common/7z/Types.h
	${CC}

7zIn.o:7zDec.o
7zIn.o:Common/7z/7zIn.c Common/7z/7z.h Common/7z/Types.h
	${CC}

7zDec.o:Bcj2.o Bra.o LzmaDec.o Lzma2Dec.o Bra86.o
7zDec.o:Common/7z/7zDec.c Common/7z/Bcj2.h Common/7z/Bra.h
	${CC}

Bra86.o:Bra.o
Bra86.o:Common/7z/Bra86.c Common/7z/Bra.h
	${CC}
	
Bcj2.o:Common/7z/Bcj2.c Common/7z/Bcj2.h Common/7z/Types.h
	${CC}
	
Bra.o:Common/7z/Bra.c Common/7z/Bra.h Common/7z/Types.h
	${CC}
	
7zBuf.o:Common/7z/7zBuf.c Common/7z/7zBuf.h Common/7z/Types.h
	${CC}

Alloc.o:Common/7z/Alloc.c Common/7z/Alloc.h
	${CC}

7zFile.o:Common/7z/7zFile.c Common/7z/7zFile.h Common/7z/Types.h
	${CC}
	
LzmaDec.o:Common/7z/LzmaDec.c Common/7z/LzmaDec.h Common/7z/Types.h
	${CC}
	
Lzma2Dec.o:LzmaDec.o
Lzma2Dec.o:Common/7z/Lzma2Dec.c Common/7z/Lzma2Dec.h Common/7z/LzmaDec.h
	${CC}

LzmaEnc.o:LzFind.o LzFindMt.o
LzmaEnc.o:Common/7z/LzmaEnc.c Common/7z/LzmaEnc.h Common/7z/LzFind.h Common/7z/LzFindMt.h Common/7z/Types.h
	${CC}
	
LzFind.o:Common/7z/LzFind.c Common/7z/LzFind.h Common/7z/LzHash.h Common/7z/Types.h
	${CC}

LzFindMt.o:Threads.o
LzFindMt.o:Common/7z/LzFindMt.c Common/7z/LzFindMt.h Common/7z/LzFind.h Common/7z/Threads.h Common/7z/LzHash.h
	${CC}
	
Threads.o:Common/7z/Threads.c Common/7z/Threads.h Common/7z/Types.h
	${CC}

7zAlloc.o:Common/7z/7zAlloc.c Common/7z/7zAlloc.h
	${CC}

7zCrc.o:CpuArch.o 7zCrcOpt.o
7zCrc.o:Common/7z/7zCrc.c Common/7z/7zCrc.h Common/7z/Types.h Common/7z/CpuArch.h
	${CC}
 	
CpuArch.o:Common/7z/CpuArch.c Common/7z/CpuArch.h Common/7z/Types.h
	${CC}
	
7zCrcOpt.o:Common/7z/7zCrcOpt.c
	${CC}
	
#############################
md5.o:Common/md5.cpp Common/md5.h
	${CC}

#############################
DealTime.o:StrOperation.o CodeOperation.o
DealTime.o:Common/DealTime.cpp Common/DealTime.h 
	g++ -g -finput-charset=UTF-8 -I $(INC_PATH) -c $< -o obj/$@

#############################
BRMath.o:DeviceInfo.o CodeOperation.o
BRMath.o:Common/BRMath.cpp Common/BRMath.h
	${CC}
	
DeviceInfo.o:Core/DeviceInfo.cpp Core/DeviceInfo.h AssistantDll.h stdafx.h
	${CC}
	
#############################
Path.o:IosPicturePath.o IosRingPath.o IosRecordingPath.o ThemePath.o FileOperation.o DeviceInfo.o #IosPublicFunc.o
Path.o:Common/Path.cpp Common/Path.h Common/IosPicturePath.h Common/IosRingPath.h Common/IosRecordingPath.h Common/ThemePath.h
	${CC}	

IosPicturePath.o:Common/IosPicturePath.cpp Common/IosPicturePath.h stdafx.h
	${CC}	

IosRingPath.o:Common/IosRingPath.cpp Common/IosRingPath.h stdafx.h
	${CC}	

IosRecordingPath.o:Common/IosRecordingPath.cpp Common/IosRecordingPath.h stdafx.h
	${CC}	

ThemePath.o:Common/ThemePath.cpp Common/ThemePath.h stdafx.h
	${CC}	

IosPublicFunc.o:FileData.o ProgressSubject.o
IosPublicFunc.o:Device/IosPublicFunc.cpp Device/IosPublicFunc.h Device/IIosFileSystem.h Data/FileData.h Monitor/ProgressSubject.h Common/StrOperation.h
	${CC}	

FileData.o:NodeData.o DirectoryOperationCommand.o
FileData.o:Data/FileData.cpp Data/FileData.h Data/NodeData.h Command/DirectoryOperationCommand.h Command/AndroidDefine.h stdafx.h
	${CC}	

DirectoryOperationCommand.o:BaseCommand.o BinaryBufferWriter.o DirData.o #AdbDirOperateCommand.o
DirectoryOperationCommand.o:Command/DirectoryOperationCommand.cpp Command/DirectoryOperationCommand.h Command/BaseCommand.h Common/BinaryBufferWriter.h Data/DirData.h Data/FileData.h
	${CC}	
	
BaseCommand.o:BinaryBufferReader.o BaseIO.o DeviceData.o IosFileIO.o
BaseCommand.o:Command/BaseCommand.cpp Command/BaseCommand.h Command/AndroidDefine.h Data/BaseData.h Monitor/ProgressSubject.h Common/BinaryBufferWriter.h Common/BinaryBufferReader.h Core/DeviceInfo.h Device/IosPublicFunc.h Device/BaseIO.h Core/DeviceData.h
	${CC}	
	
IosFileIO.o:Device/IosFileIO.cpp Device/IosFileIO.h Device/BaseIO.h Core/GlobalOption.h Device/AFCFileSysemBase.h Device/IosWiFiFileSystem.h
	${CC}	
	
DeviceData.o:DirectCommand.o #CDataSet.o
#DeviceData.o:Core/DeviceData.cpp Core/DeviceData.h Core/DirectCommand.h Common/Path.h Core/DeviceInfo.h AssistantDll.h Core/CDataSet.h
#	${CC}	

DirectCommand.o:UploadFileCommand.o #LogOffCommand.o ScreenshotCommand.o GetIosDeviceTypeCommand.o GetIMEICommand.o
DirectCommand.o:Core/DirectCommand.cpp Core/DirectCommand.h Core/DeviceInfo.h Data/BaseData.h
	${CC}	

UploadFileCommand.o:IosWiFiFileSystem.o AFCFileSysemBase.o AdbUploadFileCommand.o
UploadFileCommand.o:Command/UploadFileCommand.cpp Command/UploadFileCommand.h Device/IosWiFiFileSystem.h Device/AFCFileSysemBase.h Command/DirectoryOperationCommand.h Common/BinaryBufferWriter.h Device/IIosFileSystem.h
	${CC}	
	
AdbUploadFileCommand.o:Device/AdbUploadFileCommand.cpp Device/AdbUploadFileCommand.h Device/AdbRemoteCommand.h Monitor/ProgressSubject.h Command/AndroidDefine.h
	${CC}	

IosWiFiFileSystem.o:CIosWiFiMobileDevice.o
IosWiFiFileSystem.o:Device/IosWiFiFileSystem.cpp Device/IosWiFiFileSystem.h Monitor/ProgressSubject.h Device/IIosFileSystem.h
	${CC}	
	
CIosWiFiMobileDevice.o:Device/IosWiFiMobileDevice.cpp Device/IosWiFiMobileDevice.h Common/FileOperation.h Device/iPhoneInterfaceDef.h
	${CC}	

AFCFileSysemBase.o:CITunesMobileDevice.o LinkData.o ITunesService.o
AFCFileSysemBase.o:Device/AFCFileSysemBase.cpp Device/AFCFileSysemBase.h Core/DeviceInfo.h Monitor/ProgressSubject.h Device/IIosFileSystem.h
	${CC}	

ITunesService.o:IosUsbConnection.o IosRecoveryConnection.o #DeviceManager.o
ITunesService.o:Device/ITunesService.cpp Device/ITunesService.h Device/IConnection.h
	${CC}	
	
#DeviceManager.o:
#	${CC}	
	
IosRecoveryConnection.o:Device/IosRecoveryConnection.cpp Device/IosRecoveryConnection.h Device/IConnection.h Core/DeviceInfo.h
	${CC}	
	
IosUsbConnection.o:IosDaemon.o
IosUsbConnection.o:Device/IosUsbConnection.cpp Device/IosUsbConnection.h Device/IConnection.h Core/DeviceInfo.h
	${CC}	
	
IosDaemon.o:NDCrEnLib.o
IosDaemon.o:Device/IosDaemon.cpp Device/IosDaemon.h Device/IIosFileSystem.h
	${CC}	
	
NDCrEnLib.o:Device/NDCrEnLib.cpp Device/NDCrEnLib.h
	${CC}	
	
LinkData.o:ListDirectoryCommand.o DownloadFileCommand.o
LinkData.o:Data/LinkData.cpp Data/LinkData.h Data/NodeData.h
	${CC}	
	
DownloadFileCommand.o:AdbDownloadFileCommand.o AndroidTcpIO.o
DownloadFileCommand.o:Command/DownloadFileCommand.cpp Command/DownloadFileCommand.h Command/BaseCommand.h Common/BinaryBufferWriter.h
	${CC}	
	
AndroidTcpIO.o:Device/AndroidTcpIO.cpp Device/AndroidTcpIO.h Device/BaseIO.h
	${CC}	
	
AdbDownloadFileCommand.o:Device/AdbDownloadFileCommand.cpp Device/AdbDownloadFileCommand.h Device/AdbRemoteCommand.h Monitor/ProgressSubject.h Command/AndroidDefine.h
	${CC}	
	
ListDirectoryCommand.o:regexpr2.o AdbRemoteCommand.o
ListDirectoryCommand.o:Command/ListDirectoryCommand.cpp Command/ListDirectoryCommand.h Command/BaseCommand.h Data/NodeData.h Device/IIosFileSystem.h Device/IosWiFiFileSystem.h Device/AFCFileSysemBase.h #Common/greta/regexpr2.h
	${CC}	

AdbRemoteCommand.o:AndroidAdbHelper.o
AdbRemoteCommand.o:Device/AdbRemoteCommand.cpp Device/AdbRemoteCommand.h Common/StrOperation.h Common/AssistantException.h Common/SystemException.h Core/DeviceInfo.h #Common/BaseSocket.h 
	${CC}	
	
AndroidAdbHelper.o:Device/AndroidAdbHelper.cpp Device/AndroidAdbHelper.h Core/DeviceInfo.h #Common/BaseSocket.h 
	${CC}	
	
regexpr2.o:syntax2.o
#regexpr2.o:Common/greta/regexpr2.cpp Common/greta/regexpr2.h Common/greta/syntax2.h Common/greta/restack.h Common/greta/reimpl2.h
#	${CC}	
	
syntax2.o:Common/greta/syntax2.cpp Common/greta/syntax2.h
	${CC}	
	
CITunesMobileDevice.o:CFConverter.o	
CITunesMobileDevice.o:Device/ITunesMobileDevice.cpp Device/ITunesMobileDevice.h Device/iPhoneInterfaceDef.h #Common/BaseSocket.h 
	${CC}	
	
CFConverter.o:Device/CFConverter.cpp Device/CFConverter.cpp Common/DealTime.h stdafx.h
	${CC}	

#CDataSet.o:Core/CDataSet.cpp Core/CDataSet.h Data/BaseData.h IVisitor.h AssistantDll.h
#	${CC}	
	
BinaryBufferReader.o:CodeOperation.o
BinaryBufferReader.o:Common/BinaryBufferReader.cpp Common/BinaryBufferReader.h Common/CodeOperation.h AssistantDll.h
	${CC}	

#BaseIO.o:BaseSocket.o
BaseIO.o:Device/BaseIO.cpp Device/BaseIO.h Core/DeviceInfo.h stdafx.h #Common/BaseSocket.h 
	${CC}	
	
#BaseSocket.o:Common/BaseSocket.cpp Common/BaseSocket.h Core/DeviceInfo.h stdafx.h
#	${CC}	
	
BinaryBufferWriter.o:CodeOperation.o	
BinaryBufferWriter.o:Common/BinaryBufferWriter.cpp Common/BinaryBufferWriter.h Common/CodeOperation.h
	${CC}	
	
NodeData.o:Data/NodeData.cpp Data/NodeData.h Command/AndroidDefine.h AssistantDll.h stdafx.h
	${CC}
	
DirData.o:RootData.o #CopyFileCommand.o
DirData.o:Data/DirData.cpp Data/DirData.h Data/RootData.h Data/NodeData.h Command/AndroidDefine.h stdafx.h Monitor/ProgressObserver.h
	${CC}
	
#CopyFileCommand.o:
#	${CC}
	
RootData.o:BaseData.o
RootData.o:Data/RootData.cpp Data/RootData.h Data/BaseData.h Data/NodeData.h Core/DeviceInfo.h AssistantDll.h
	${CC}

BaseData.o:Data/BaseData.cpp Data/BaseData.h Command/AndroidDefine.h AssistantDll.h
	${CC}
		
ProgressSubject.o:ISubject.o ProgressObserver.o
ProgressSubject.o:Monitor/ProgressSubject.cpp Monitor/ProgressSubject.h Monitor/ISubject.h Monitor/ProgressObserver.h AssistantDll.h
	${CC}	

ISubject.o:IObserver.o
ISubject.o:Monitor/ISubject.cpp Monitor/ISubject.h Monitor/IObserver.h AssistantDll.h
	${CC}	
	
IObserver.o:Monitor/IObserver.cpp Monitor/IObserver.h AssistantDll.h
	${CC}	
	
ProgressObserver.o:ProgressData.o
ProgressObserver.o:Monitor/ProgressObserver.cpp Monitor/ProgressObserver.h Monitor/ProgressData.h Monitor/IObserver.h Monitor/ICancel.h AssistantDll.h
	${CC}	
	
ProgressData.o:Monitor/ProgressData.cpp Monitor/ProgressData.h AssistantDll.h stdafx.h
	${CC}
	
	

#############################
clean:
	rm -rf obj/*.o

