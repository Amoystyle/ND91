#include "file.h"

struct nd_data GetFileAttr(char* path, bool isGetWritable)
{
	//DebugNdsh("GetFileAttr(\"%s\"", path);
	struct nd_data dataResult = {0, 0, NULL};
	struct stat flag;
	if(lstat(path, &flag) == 0)
	{
		WriteIntToBuffer(&dataResult, UNKNOW_ERROR);
		if(S_ISLNK(flag.st_mode))
		{
			char* pszResolvedName = (char*) malloc(PATH_MAX);
			if (pszResolvedName == NULL)
			{
				ErrorNdsh("malloc(PATH_MAX) fail! path: %s, errno: %d\n", path, errno);
				WriteIntToBufferInPos(&dataResult, 0, ConvertErrorNo());

				return dataResult;
			}

			ssize_t size = readlink(path, pszResolvedName, PATH_MAX);
			pszResolvedName[size] = '\0';
			WriteStringToBuffer(&dataResult, pszResolvedName, strlen(pszResolvedName));
			free(pszResolvedName);
		}
		else
		{
			WriteStringToBuffer(&dataResult, NULL, 0);
		}

		if(S_ISDIR(flag.st_mode))
		{
			WriteULongToBuffer(&dataResult, 0);
		}
		else
		{
			WriteULongToBuffer(&dataResult, flag.st_size);
		}
		WriteULongToBuffer(&dataResult, flag.st_mtime);
		WriteUIntToBuffer(&dataResult, flag.st_mode);

		bool hasSubDir = false;
		if(S_ISDIR(flag.st_mode))
		{
			hasSubDir = HasChildDir(path);
		}

		if(hasSubDir == true)
		{
			WriteTrueToBuffer(&dataResult);
		}
		else
		{
			WriteFalseToBuffer(&dataResult);
		}

		if (isGetWritable)
		{
			//文件是否可写
			if (access(path, W_OK) == 0)
			{
				WriteTrueToBuffer(&dataResult);
			}
			else
			{
				WriteFalseToBuffer(&dataResult);
			}
		}

		WriteIntToBufferInPos(&dataResult, 0, SUCCESS);
	}
	else
	{
		ErrorNdsh("stat(\"%s\", &flag) fail !    error: %d\n", path, errno);
		WriteIntToBuffer(&dataResult, ConvertErrorNo());
	}

	return dataResult;
}

struct nd_data GetFileAttr_nd_data(struct nd_data* dataReceive, bool isGetWritable)
{
	//DebugNdsh("GetDirAttr_nd_data(data_receive)\n");
	struct nd_data dataResult = {0, 0, NULL};
	char* path = ReadStringFromBuffer(dataReceive);

	dataResult = GetFileAttr(path, isGetWritable);
	free(path);

	return dataResult;
}

struct nd_data GetDirInfo(char* path, bool isGetSubWritable)
{
	//DebugNdsh("GetDirInfo(\"%s\", %s)", path, isGetSubWritable ? "true" : "false");
	struct nd_data dataResult = {0, 0, NULL};
	struct stat flag;

	//写入结果占位符
	WriteIntToBuffer(&dataResult, UNKNOW_ERROR);
	if(lstat(path, &flag) == 0)
	{
		if(S_ISDIR(flag.st_mode))
		{
			DIR* dir = opendir(path);
			if(dir == NULL)
			{
				ErrorNdsh("opendir(\"%s\") fail !    errno: %d\n", path, errno);				
				WriteIntToBufferInPos(&dataResult, 0, ConvertErrorNo());

				return dataResult;
			}

			//目录是否可写
			if(access(path, W_OK) == 0)
			{
				WriteTrueToBuffer(&dataResult);
			}
			else
			{
				WriteFalseToBuffer(&dataResult);
			}

			struct dirent* entry;
			int count = 0;
			WriteIntToBuffer(&dataResult, count);
			while((entry = readdir(dir)) != NULL)
			{
				if(IsInvalidPath(entry->d_name))
					continue;

				char* pszSubPath = Combine(path, entry->d_name);
				if(pszSubPath == NULL)
				{
					WriteIntToBufferInPos(&dataResult, 0, ConvertErrorNo());
					ErrorNdsh("combine(\"%s\", \"%s\") fail !    errno: %d\n", path, entry->d_name, errno);

					return dataResult;
				}

				struct stat subFlag;
				if(lstat(pszSubPath, &subFlag) != 0)
				{
					free(pszSubPath);
					continue;
				}

				WriteStringToBuffer(&dataResult, pszSubPath, strlen(pszSubPath));
				//是否为链接
				if(S_ISLNK(subFlag.st_mode))
				{
					char* pszResolvedName = (char*) malloc(PATH_MAX);
					if (pszResolvedName == NULL)
					{
						ErrorNdsh("malloc(PATH_MAX) fail! pszSubPath: %s, errno: %d\n", pszSubPath, errno);
						free(pszSubPath);

						continue;
					}

					ssize_t size = readlink(pszSubPath, pszResolvedName, PATH_MAX);
					pszResolvedName[size] = '\0';
					WriteStringToBuffer(&dataResult, pszResolvedName, strlen(pszResolvedName));
					free(pszResolvedName);
				}
				else
				{
					WriteStringToBuffer(&dataResult, NULL, 0);
				}

				WriteULongToBuffer(&dataResult, subFlag.st_size);
				WriteULongToBuffer(&dataResult, subFlag.st_mtime);
				WriteUIntToBuffer(&dataResult, subFlag.st_mode);

				//子文件是否包含自己的子目录
				bool childFolderIncludeSubdir = false;
				if(S_ISDIR(subFlag.st_mode))
				{
					childFolderIncludeSubdir = HasChildDir(pszSubPath);
				}
				if(childFolderIncludeSubdir)
				{
					WriteTrueToBuffer(&dataResult);
				}
				else
				{
					WriteFalseToBuffer(&dataResult);
				}

				if (isGetSubWritable)
				{
					//子目录是否可写
					if (access(path, W_OK) == 0)
					{
						WriteTrueToBuffer(&dataResult);
					}
					else
					{
						WriteFalseToBuffer(&dataResult);
					}
				}

				free(pszSubPath);

				++count;
			}
			closedir(dir);

			WriteIntToBufferInPos(&dataResult, 5, count);
			WriteIntToBufferInPos(&dataResult, 0, SUCCESS);
		}
		else
		{
			ErrorNdsh("\"%s\" not dir !    error: %d\n", path, errno);
			WriteIntToBufferInPos(&dataResult, 0, ConvertErrorNo(ENOTDIR));
		}
	}
	else
	{
		ErrorNdsh("lstat(\"%s\", &flag) fail !    error: %d\n", path, errno);
		WriteIntToBufferInPos(&dataResult, 0, ConvertErrorNo());
	}

	return dataResult;	
}

struct nd_data GetDirInfo_nd_data(struct nd_data* dataReceive, bool isGetSubWritable)
{
	//DebugNdsh("GetDirInfo(struct nd_data* dataReceive, bool isGetSubWritable)\n");

	struct nd_data dataRet = {0, 0, NULL};
	char* path = ReadStringFromBuffer(dataReceive);

	//DebugNdsh("get dir info: \"%s\"\n", path);
	dataRet = GetDirInfo(path, isGetSubWritable);
	free(path);

	return dataRet;
}

bool HasChildDir(char* path)
{
	//DebugNdsh("GetDirChildFolderCount(\"%s\")\n", path);

	bool result = false;

	DIR* dir = opendir(path);
	if(dir != NULL)
	{
		struct dirent* entry;
		while((entry = readdir(dir)) != NULL)
		{
			if(IsInvalidPath(entry->d_name))
				continue;

			struct stat subFlag;
			char* pszSubPath = Combine(path, entry->d_name);
			if(lstat(pszSubPath, &subFlag) == 0)
			{
				if(S_ISDIR(subFlag.st_mode))
				{
					result = true;
					free(pszSubPath);

					break;
				}
			}
			free(pszSubPath);
		}

		closedir(dir);
	}

	return result;
}

struct nd_data MakeDir(const char* path)
{
	//DebugNdsh("MakeDir(\"%s\")\n", path);

	int i = 1;
	int len = strlen(path);
	int result = 0;

	char* subPath = (char*) malloc(len * sizeof(char));


	//创建父目录
	while(i < len && result == 0 )
	{
		if(path[i] == '/')
		{
			memset(subPath, '\0', len);
			strncpy(subPath, path, i);

			if(access(subPath, F_OK) != 0)
			{
				result = mkdir(subPath, S_IRWXU | S_IRWXG | S_IRWXO);
				if(result == 0)
				{
					//三星s3创建文件时给定777无效，创建完后改权限
					chmod(path, 0777);
				}                   
			}
		}

		i++;
	}

	//如果给定路径目录不存在，就创建
	if(result == 0 && access(path, F_OK) != 0)
	{
		result = mkdir(path, 0777);
	}

	if(result == 0 && access(path, F_OK) == 0)
	{
		//三星s3创建文件时给定777无效，创建完后改权限
		chmod(path, 0777);
		//DebugNdsh("chmod(\"%s\", 0777)", path);         
	}       


	struct nd_data dataResult = {0, 0, NULL};
	if(result == 0)
	{
		WriteIntToBuffer(&dataResult, SUCCESS);
	}
	else
	{
		ErrorNdsh("mkdir fail, path:%s,  error: %d\n", path, errno);
		WriteIntToBuffer(&dataResult, ConvertErrorNo());
	}

	return dataResult;
}

struct nd_data MakeDir_nd_data(struct nd_data* data_receive)
{
	//DebugNdsh("MakeDir(struct nd_data* data_receive)\n");

	struct nd_data dataResult = {0, 0, NULL};
	char* path = ReadStringFromBuffer(data_receive);

	dataResult = MakeDir(path);
	free(path);

	return dataResult;
}

bool IsInvalidPath(char* path)
{
	if(path == NULL || strlen(path) == 0)
	{
		return true;
	}

	if(strcmp(path, ".") == 0 || strcmp(path, "..") == 0)
	{
		return true;
	}

	return false;
}

struct nd_data MoveFile(char* pathOld, char* pathNew)
{
	DebugNdsh("MoveFile(\"%s\", \"%s\")", pathOld, pathNew);
	struct nd_data dataResult = {0, 0, NULL};

	struct stat flag;
	if(lstat(pathOld, &flag) == 0)
	{
		//创建目标文件夹
		char* dir = GetDirPath(pathNew);
		if(dir != NULL)
		{
			MakeDir(dir);
			if (access(dir, F_OK) != 0)
			{
				free(dir);
				WriteIntToBuffer(&dataResult, ConvertErrorNo());
				
				return dataResult;
			}
			free(dir);
		}

		if(rename(pathOld, pathNew) == 0)
		{
			WriteIntToBuffer(&dataResult, SUCCESS);
		}
		else
		{
			ErrorNdsh("rename(\"%s\", \"%s\") fail !    errno: %d\n", pathOld, pathNew, errno);
			WriteIntToBuffer(&dataResult, ConvertErrorNo());			
		}
	}
	else
	{
		ErrorNdsh("movefile lstat failed. errno: %d\n", errno);
		WriteIntToBuffer(&dataResult, ConvertErrorNo());	
	}

	return dataResult;
}

struct nd_data MoveFile_nd_data(struct nd_data* data_receive)
{
	//DebugNdsh("MoveFile(struct nd_data* data_receive)\n");

	struct nd_data dataResult = { 0, 0, NULL };
	char* pathOld = ReadStringFromBuffer(data_receive);
	char* pathNew = ReadStringFromBuffer(data_receive);

	dataResult = MoveFile(pathOld, pathNew);
	free(pathOld);
	free(pathNew);

	return dataResult;
}

struct nd_data CopyFile(const char* srcPath, const char* desPath)
{
	struct nd_data data_result = {0, 0, NULL};
	int err = CopyFileOrDir(srcPath, desPath);
	WriteIntToBuffer(&data_result, err);
	return data_result;
}

//复制源路径和目录路径必须为最终路径。例如复制文件,srcPath:/sdcard/a.txt, desPath:/sdcard/nd/a.txt;复制目录,srcPath: /sdcard/dir_a, desPath: /sdcard/nd/dir_a
int CopyFileOrDir(const char* srcPath, const char* desPath)
{
	int err = SUCCESS;
	struct stat flag;

	if(lstat(srcPath, &flag) == 0)
	{		
		if(S_ISLNK(flag.st_mode))
		{
			return SUCCESS;
		}
		else if(S_ISDIR(flag.st_mode))
		{			
			MakeDir(desPath);
			if (access(desPath, F_OK) != 0)
			{
				return ConvertErrorNo();
			}

			DIR* dir = opendir(srcPath);
			if(dir == NULL)
			{
				ErrorNdsh("opendir(\"%s\") fail !  errno: %d\n", srcPath, errno);

				return ConvertErrorNo();
			}

			struct dirent* entry;		
			while(err == SUCCESS && (entry = readdir(dir)) != NULL)
			{
				if(IsInvalidPath(entry->d_name))
					continue;

				char* subSrcPath = Combine(srcPath, entry->d_name);
				char* subDesPath = Combine(desPath, entry->d_name);

				err = CopyFileOrDir(subSrcPath, subDesPath);	

				free(subSrcPath);
				free(subDesPath);
			}
			closedir(dir);		
		}
		else 
		{	
			return CopyOneFile(srcPath, desPath);
		}		
	}
	else
	{
		ErrorNdsh("stat(\"%s\", &flag) fail !    error: %d\n", srcPath, errno);
		err = ConvertErrorNo();
	}

	return err;
}

int CopyOneFile(const char* srcPath, const char* desPath)
{
	DebugNdsh("copyfile, src: \"%s\", des: \"%s\"\n", srcPath, desPath);

	int from_fd = 0;
	int to_fd = 0;

	int err = SUCCESS;

	if((from_fd = open(srcPath, O_RDONLY)) == -1)
	{
		ErrorNdsh("open(\"%s\", O_RDONLY) fail !    errno: %d\n", srcPath, errno);
		err = ConvertErrorNo();
	}

	//创建文件夹
	char* desDir = GetDirPath(desPath);
	if(desDir != NULL)
	{
		MakeDir(desDir);
		if (access(desDir, F_OK) != 0)
		{
			err = ConvertErrorNo();
		}

		free(desDir);
	}
	
	if(err == SUCCESS && (to_fd = open(desPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		ErrorNdsh("open(\"%s\", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR) fail !    errno: %d\n", desPath, errno);
		err = ConvertErrorNo();
	}

	//DebugNdsh("copyfile, srcFd:%d, desFd:%d\n", from_fd, to_fd);

	if(err == SUCCESS)
	{
		//三星s3创建文件时给定777无效，创建完后改权限
		chmod(desPath, 0777);

		int bytes_read = 0;
		int bytes_write = 0;
		char buffer[16384];
		char* ptr;
		while(err == SUCCESS && (bytes_read = read(from_fd, buffer, 16384)))
		{
			if(bytes_read == -1)
			{
				ErrorNdsh("read fail! path:%s,  errno: %d\n", srcPath, errno);
				err = ConvertErrorNo();
				break;
			}

			else if(bytes_read > 0)
			{
				ptr = buffer;

				while(bytes_write = write(to_fd, ptr, bytes_read))
				{
					if(bytes_write == -1)
					{
						ErrorNdsh("write fail !    errno: %d\n", errno);
						err = ConvertErrorNo();
						break;
					}
					else if(bytes_write == bytes_read)
					{
						break;
					}
					else if(bytes_write > 0)
					{
						ptr += bytes_write;
						bytes_read -= bytes_write;
					}
				}
			}
		}
	}

	if(from_fd > 0)
	{
		close(from_fd);
	}

	if(to_fd > 0)
	{
		close(to_fd);
	}

	return err;	
}

struct nd_data CopyFile_nd_data(struct nd_data* data_receive)
{
	//DebugNdsh("CopyFile(struct nd_data* data_receive)\n");

	char* srcPath = ReadStringFromBuffer(data_receive);
	char* desPath = ReadStringFromBuffer(data_receive);

	struct nd_data data = CopyFile(srcPath, desPath);
	free(srcPath);
	free(desPath);

	return data;
}

int RemoveFileInternal(const char* path)
{
	int err = 0;

	struct stat flag;
	if(lstat(path, &flag) == 0)
	{
		if(S_ISDIR(flag.st_mode))
		{
			DIR* dir = opendir(path);
			if(dir != NULL)
			{
				struct dirent* entry = NULL;
				while((entry = readdir(dir)) != NULL)
				{
					if(IsInvalidPath(entry->d_name))
						continue;

					char* subPath = Combine(path, entry->d_name);
					struct nd_data dataRet = {0, 0, NULL};
					err = RemoveFileInternal(subPath);
					free(subPath);
			
					if(err != 0)
					{					
						break;
					}
				}
				closedir(dir);

				if(err == 0 && rmdir(path) != 0)
				{
					ErrorNdsh("rmdir(\"%s\") fail !    errno: %d\n", path, errno);
					err = errno;
				}				
			}
			else
			{
				ErrorNdsh("opendir(\"%s\") fail !    errno: %d\n", path, errno);
				err = errno;
			}
		}
		else
		{
			if(unlink(path) != 0)
			{
				ErrorNdsh("unlink(\"%s\") fail !    errno: %d\n", path, errno);
				err = errno;
			}		
		}
	}
	else
	{
		ErrorNdsh("lstat(\"%s\", &flag) fail !    errno: %d\n", path, errno);
		err = errno;
	}	

	return err;	
}

struct nd_data RemoveFile(const char* path)
{
	struct nd_data dataResult = {0, 0, NULL};
	int err =  ConvertErrorNo(RemoveFileInternal(path));
	WriteIntToBuffer(&dataResult, err);

	return dataResult;
}

struct nd_data RemoveFile_nd_data(struct nd_data* dataReceive)
{	
	char* path = ReadStringFromBuffer(dataReceive);

	struct nd_data dataResult = RemoveFile(path);
	free(path);

	return dataResult;
}

struct nd_data ReadFile(struct nd_data* data_receive)
{
	//DebugNdsh("ReadFile(struct nd_data* data_receive)\n");

	char* path = ReadStringFromBuffer(data_receive);
	int length = ReadIntFromBuffer(data_receive);

	//DebugNdsh("read \"%s\", length: %d\n", path, length);

	struct nd_data dataResult = {0, 0, NULL};

	struct stat flag;
	if(stat(path, &flag) == -1)
	{
		ErrorNdsh("stat(\"path\", &flag) fail !    errno: %d\n", path, errno);
		WriteIntToBuffer(&dataResult, ConvertErrorNo(errno));
		free(path);

		return dataResult;
	}

	if(length == -1)
	{
		if(S_ISREG(flag.st_mode) || S_ISLNK(flag.st_mode || S_ISDIR(flag.st_mode)))
		{
			length = flag.st_size;
		}
		else
		{
			errno = EACCES;
			ErrorNdsh("argument 'length' = -1, but file \"path\" type is not regular or link, errno: %d\n", path, errno);
			WriteIntToBuffer(&dataResult, ConvertErrorNo(errno));
			free(path);

			return dataResult;
		}
	}

	FILE* fileRead = NULL;
	if((fileRead = fopen(path, "r")) == NULL)
	{
		ErrorNdsh("fopen(\"%s\", \"r\") fail !    errno: %d\n", path, errno);
		WriteIntToBuffer(&dataResult, ConvertErrorNo());
		free(path);

		return dataResult;
	}

	char* content;
	int count = ReadFromStream(fileRead, &content, length);
	if(count < 0)
	{
		ErrorNdsh("ReadFromStream() read count < 0 !\n");
		WriteIntToBuffer(&dataResult, ConvertErrorNo());
		free(path);

		return dataResult;
	}

	WriteIntToBuffer(&dataResult, SUCCESS);
	WriteStringToBuffer(&dataResult, content, count);
	pclose(fileRead);
	free(path);

	return dataResult;
}

char* Combine(const char* folder, const char* file)
{
	if(folder == NULL || file == NULL)
	{
		errno = EINVAL;
		ErrorNdsh("argument 'folder' or 'file' is NULL !\n");

		return NULL;
	}

	char* pszResult = (char*) malloc(PATH_MAX * sizeof(char));
	if(pszResult == NULL)
	{
		ErrorNdsh("malloc(PATH_MAX * sizeof(char)) fail !    errno: %d\n", errno);

		return NULL;
	}

	int folderLen = strlen(folder);
	int fileLen = strlen(file);
	int separator = 0;
	if(*(folder + (folderLen - 1)) != '/')
	{
		separator = 1;
	}
	if(folderLen + separator + fileLen + 1 > PATH_MAX)
	{
		errno = ENAMETOOLONG;
		free(pszResult);
		ErrorNdsh("if combine(folder, file) filepath will too long !\n");

		return NULL;
	}

	strcpy(pszResult, folder);
	if(separator)
	{
		strcat(pszResult, "/");
	}
	strcat(pszResult, file);

	return pszResult;
}

struct nd_data Chmod(char* path, int mode)
{
	struct nd_data dataResult = {0, 0, NULL};
	int err = ConvertErrorNo(RecursiveChmod(path, mode));
	WriteIntToBuffer(&dataResult, err);

	return dataResult;
}

struct nd_data Chmod_nd_data(struct nd_data* dataReceive)
{
	char* path = ReadStringFromBuffer(dataReceive);
	int mode = ReadIntFromBuffer(dataReceive);

	struct nd_data dataResult = {0, 0, NULL};
	dataResult = Chmod(path, mode);

	free(path);

	return dataResult;
}

int RecursiveChmod(char* path, int mode)
{	
	struct stat flag;
	if(lstat(path, &flag) == 0)
	{
		if(S_ISDIR(flag.st_mode))
		{
			if(chmod(path, mode) != 0)
				return errno;

			DIR* dir = opendir(path);
			if(dir != NULL)
			{
				struct dirent* entry = NULL;
				while((entry = readdir(dir)) != NULL)
				{
					if(IsInvalidPath(entry->d_name))
						continue;

					char* subPath = Combine(path, entry->d_name);
					int chResult = RecursiveChmod(subPath, mode);
					free(subPath);

					if(chResult != 0)
					{
						closedir(dir);

						return chResult;
					}
				}

				closedir(dir);

				return 0;
			}
			else
			{
				return errno;
			}
		}
		else
		{
			if(S_ISLNK(flag.st_mode)) //符号连接不做修改
			{
				return 0;
			}
			else
			{
				if(chmod(path, mode) == 0)
				{
					return 0;
				}
				else 
				{
					return errno;
				}
			}
		}
	}
	else
	{
		return errno;
	}
}

int RecursiveChownAndGrp(char* pszPath, int uid, int gid)
{	
	struct stat flag;
	if(lstat(pszPath, &flag) == 0)
	{	
		if(S_ISLNK(flag.st_mode))
		{
			return 0;
		}
		else if(S_ISDIR(flag.st_mode))
		{
			if(chown(pszPath, uid, gid) != 0)
				return errno;

			DIR* dir = opendir(pszPath);
			if(dir != NULL)
			{
				struct dirent* entry = NULL;
				while((entry = readdir(dir)) != NULL)
				{
					if(IsInvalidPath(entry->d_name))
						continue;

					char* subPath = Combine(pszPath, entry->d_name);
					int chResult = RecursiveChownAndGrp(subPath, uid, gid);
					free(subPath);

					if(chResult != 0)
					{
						closedir(dir);

						return chResult;
					}
				}

				closedir(dir);

				return 0;
			}
			else
			{
				return errno;
			}
		}
		else
		{
			if(chown(pszPath, uid, gid) == 0)
			{
				return 0;
			}
			else 
			{
				return errno;
			}
		}
	}
	else
	{
		return errno;
	}
}


struct nd_data ChownAndGrp(char* pszPath, int uid, int gid)
{
	struct nd_data dataResult = {0, 0, NULL};
	int err = ConvertErrorNo(RecursiveChownAndGrp(pszPath, uid, gid));
	WriteIntToBuffer(&dataResult, err);

	return dataResult;
}

struct nd_data ChownAndGrp_nd_data(struct nd_data* dataReceive)
{
	char* path = ReadStringFromBuffer(dataReceive);
	int uid = ReadIntFromBuffer(dataReceive);
	int gid = ReadIntFromBuffer(dataReceive);
	struct nd_data dataResult = {0, 0, NULL};
	dataResult = ChownAndGrp(path, uid, gid);

	free(path);

	return dataResult;
}


//将errno转换为自定义类型
int ConvertErrorNo()
{
	return ConvertErrorNo(errno);
}

int ConvertErrorNo(int err)
{	
	if(err == 0)
	{
		return 1;
	}

	return 10000 + err;		     
}

char* GetDirPath(const char* filePath)
{
	int len = strlen(filePath);
	int index = -1;

	for(int i = 0; i < len; ++i)
	{
		if(*(filePath + i) == '/')
		{
			index = i;
		}
	}

	char* dir = NULL;
	if(index != -1)
	{
		dir = new char[index + 2];
		memset(dir, 0, index + 2);
		strncpy(dir, filePath, index+1);
	}

	return dir;
}

struct nd_data RenameFile_nd_data(struct nd_data* dataReceive){
	struct nd_data dataResult = {0, 0, NULL};
	int err = UNKNOW_ERROR;
	char* oldname = ReadStringFromBuffer(dataReceive);
	char* newname = ReadStringFromBuffer(dataReceive);
	DebugNdsh("RenameFile(\"%s\"  (\"%s\" ", oldname,newname);
	if (rename(oldname,newname)==0)   
        err = SUCCESS;
	WriteIntToBuffer(&dataResult, err);
	free(oldname);
	free(newname);
	return dataResult;
}

struct nd_data ReaddataFile_nd_data(struct nd_data* dataReceive){
	struct nd_data dataResult = {0, 0, NULL};
	int err = UNKNOW_ERROR;
	char* path = ReadStringFromBuffer(dataReceive);
	int num = ReadIntFromBuffer(dataReceive);
	int readsize = ReadIntFromBuffer(dataReceive);
	FILE* fileRead = NULL;
    if((fileRead = fopen(path, "r")) != NULL){
	    fseek(fileRead,0,SEEK_SET);
		char contont[readsize];
		fseek(fileRead,num*64*1024,SEEK_CUR);
		int readcount = fread(contont,1,readsize ,fileRead);
	    err = SUCCESS;
		WriteIntToBuffer(&dataResult, err);
		WriteStringToBuffer(&dataResult, contont, readcount);
	}else{
        WriteIntToBuffer(&dataResult, err);
	}
	fclose(fileRead);
	free(path);
	return dataResult;

}

struct nd_data MakeFile_nd_data(struct nd_data* dataReceive){
    struct nd_data dataResult = {0, 0, NULL};
	int err = UNKNOW_ERROR;
	char* path = ReadStringFromBuffer(dataReceive);
	DebugNdsh("MakeFile(\"%s\" ", path);
	FILE *fp;
    if((fp=fopen(path,"w+")) != NULL){
	    err = SUCCESS;
	}else{
	    DebugNdsh("MakeFile fail");
	}
	fclose(fp);
	free(path);
	WriteIntToBuffer(&dataResult, err);
	return dataResult;
}

struct nd_data WriteFile_nd_data(struct nd_data* dataReceive){
    struct nd_data dataResult = {0, 0, NULL};
	int err = UNKNOW_ERROR;
	char* path = ReadStringFromBuffer(dataReceive);
	DebugNdsh("WriteFile(\"%s\" ", path);
	int handle;
	if((handle = open(path, O_APPEND|O_RDWR)) == -1)
    {
       ErrorNdsh("fopen file fail \n");
    }else{
	    char* content;
	    int len = ReadIntFromBuffer(dataReceive);
	    content = (char*) malloc(len);
	    char* p = (char*)(dataReceive->buffer + dataReceive->pos);
	    memcpy(content, p, len);
	    dataReceive->pos += len;
		DebugNdsh("WriteFile path (\"%s\"  , %d ", path,len);
	    int  count = write(handle,content, len); 
		if(count>0){
		    err = SUCCESS;
		}
	    close(handle);
		free(content);
	}	
	WriteIntToBuffer(&dataResult, err);
	return dataResult;
}



