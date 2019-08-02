
#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <grp.h>
#include <dirent.h>
#include <pwd.h>
#include <stdlib.h>
#include <errno.h>

#include "log.h"
#include "buffer.h"
#include "ndstream.h"

bool HasChildDir(char* path);
struct nd_data GetDirInfo(char* path, bool isGetSubWritable);
struct nd_data GetDirInfo_nd_data(struct nd_data* dataReceive, bool isGetSubWritable);
struct nd_data GetFileAttr(char* path, bool isGetWritable);
struct nd_data GetFileAttr_nd_data(struct nd_data* data_receive, bool isGetWritable);
struct nd_data MakeDir(const char* path);
struct nd_data MakeDir_nd_data(struct nd_data* data_receive);
bool IsInvalidPath(char* path);
struct nd_data MoveFile(char* pathOld, char* pathNew);
struct nd_data MoveFile_nd_data(struct nd_data* data_receive);
struct nd_data CopyFile(const char* srcPath, const char* desPath);
int CopyFileOrDir(const char* srcPath, const char* desPath);
int CopyOneFile(const char* srcPath, const char* desPath);
struct nd_data CopyFile_nd_data(struct nd_data* data_receive) ;
int RemoveFileInternal(const char* path);
struct nd_data RemoveFile(const char* path);
struct nd_data RemoveFile_nd_data(struct nd_data* dataReceive);
struct nd_data ReadFile(struct nd_data* data_receive);
char* Combine(const char* folder, const char* file);
struct nd_data Chmod(char* path, int mode);
struct nd_data Chmod_nd_data(struct nd_data* dataReceive);
int RecursiveChmod(char* path, int mode);
struct nd_data ChownAndGrp_nd_data(struct nd_data* dataReceive);
struct nd_data ChownAndGrp(char* pszPath, int uid, int gid);
int RecursiveChownAndGrp(char* pszPath, int uid, int gid);
int ConvertErrorNo();
int ConvertErrorNo(int err);
char* GetDirPath(const char* filePath);
struct nd_data RenameFile_nd_data(struct nd_data* dataReceive);
struct nd_data MakeFile_nd_data(struct nd_data* dataReceive);
struct nd_data WriteFile_nd_data(struct nd_data* dataReceive);
struct nd_data ReaddataFile_nd_data(struct nd_data* dataReceive);


