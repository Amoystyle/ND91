#include "pch_module.h"
#include "TableQueryCommand.h"
#include "Module/AndroidDefine.h"

TableQueryCommand::TableQueryCommand(const DeviceData* pDevice)
:BaseCommand(pDevice)
{
	_nAndroidCommandID = TABLE_QUERY; 
}

TableQueryCommand::~TableQueryCommand(void)
{
}

void ND91Assistant::TableQueryCommand::SetParam( string tablename, const string columns, string condition, string sort )
{
	_tableName = tablename;
	_condition = condition;
	_sort      = sort;

	_tablecolumns.clear();

	_tablecolumns = CStrOperation::parseStrings(columns,',');
}

void ND91Assistant::TableQueryCommand::Execute()
{
	DefaultExecute();
}

std::string ND91Assistant::TableQueryCommand::GetAndroidBodyData()
{
	BinaryBufferWriter  bufWrite;
	bufWrite.Clear();
	bufWrite.WriteInt(_nAndroidChannelID);
	bufWrite.WriteChar((char)_nAndroidCommandID);

	// ����
	bufWrite.WriteString(_tableName);
	// ����
	bufWrite.WriteInt(_tablecolumns.size());
	// ������
	for(size_t i=0; i<_tablecolumns.size(); i++)
	{
		bufWrite.WriteString( _tablecolumns[i]);
	}
	//��ѯ����
	bufWrite.WriteString(_condition);

	// ����������
	bufWrite.WriteString(_sort);

	return bufWrite.GetBuffer(); 
}

void ND91Assistant::TableQueryCommand::SetAndroidReturnData( std::string data )
{

}

bool ND91Assistant::TableQueryCommand::parseAndroidPackageHead( BinaryBufferReader* pReader )
{
	pReader->ReadBuffer(_pReturnHead, 4);
	pReader->ReadShort(_nReturnFlag);
	pReader->ReadShort(_nReturnLongConnectSign);
	pReader->ReadInt32(_nReturnBodySize);
	pReader->ReadInt32(_nReturnChannelID);
	if(_nReturnChannelID != _nAndroidChannelID)
		return false;
	return true;
}