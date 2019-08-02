#pragma once
#include <string>

class UBDGLIB_API UserData
{
public:
	UserData(){_time = 0;}
	~UserData();
		 
	std::string _userName;		
	std::string _phoneNumber;	
	std::string _comment;
	long		_time;
};

class UBDGLIB_API PackageData
{
public:
	PackageData(){_nPackageId = -1; _nLegal = -1;_packTime = 0;}
	~PackageData();

	int			_nPackageId;
	std::string _Name;
	std::string _Size;
	std::string _ImgSrc;
	std::string _nPlatfrom;
	int			_nLegal;
	long		_packTime;
	std::string _strUrl;
};

class UBDGLIB_API SoftData
{
public:
	SoftData(){_nSoftId = -1; _nLegal = -1;}
	~SoftData();

	int			_nSoftId;
	std::string _Name;
	std::string _Size;
	std::string _ImgSrc;
	std::string _Version;
	std::string _Extension;
	std::string _DownloadUrl;
	std::string _Identifier;
	std::string _Summary;
	int			_nLegal;

};

