#include "buffer.h"

void WriteStringToBuffer(struct nd_data* ndData, char* content, int length)
{
	if (content == NULL || length <= 0) {
		WriteIntToBuffer(ndData, 0);
		return;
	}

	CheckBufferSize(ndData, length + 4);
	WriteIntToBuffer(ndData, length);
	char* p = ndData->buffer + ndData->pos;
	memcpy(p, content, length);
	ndData->pos += length;
}

void WriteBytesToBuffer(struct nd_data* ndData, char data[])
{

}

void WriteIntToBuffer(struct nd_data* ndData, int value)
{
	char tmp[4];
	tmp[0] = (char) (value & 0x000000FF);
	tmp[1] = (char) ((value & 0x0000FF00) >> 8);
	tmp[2] = (char) ((value & 0x00FF0000) >> 16);
	tmp[3] = (char) ((value & 0xFF000000) >> 24);

	CheckBufferSize(ndData, 4);
	char* p = ndData->buffer + ndData->pos;
	memcpy(p, tmp, 4);
	ndData->pos += 4;
}

void WriteUIntToBuffer(struct nd_data* ndData, unsigned int value)
{
	char tmp[4];
	tmp[0] = (char) (value & 0x000000FF);
	tmp[1] = (char) ((value & 0x0000FF00) >> 8);
	tmp[2] = (char) ((value & 0x00FF0000) >> 16);
	tmp[3] = (char) ((value & 0xFF000000) >> 24);

	CheckBufferSize(ndData, 4);
	char* p = ndData->buffer + ndData->pos;
	memcpy(p, tmp, 4);
	ndData->pos += 4;
}

void WriteULongToBuffer(struct nd_data* ndData, unsigned long value)
{
	char tmp[8];
	int index = 0;
	while (index < 8) {
		tmp[index] = (char) ((value >> (8 * index)) & 0xFF);
		index++;
	}

	CheckBufferSize(ndData, 8);
	char* p = ndData->buffer + ndData->pos;
	memcpy(p, tmp, 8);
	ndData->pos += 8;
}

void WriteIntToBufferInPos(struct nd_data* ndData, int pos, int value)
{
	char tmp[4];
	tmp[0] = (char) (value & 0x000000FF);
	tmp[1] = (char) ((value & 0x0000FF00) >> 8);
	tmp[2] = (char) ((value & 0x00FF0000) >> 16);
	tmp[3] = (char) ((value & 0xFF000000) >> 24);

	CheckBufferSize(ndData, 4);
	char* p = ndData->buffer + pos;
	memcpy(p, tmp, 4);
}

void WriteByteToBufferInPos(struct nd_data* ndData, int pos, char ch)
{
	CheckBufferSize(ndData, 1);
	char* p = ndData->buffer + pos;
	*p = ch;
}

void WriteTrueToBuffer(struct nd_data* ndData)
{
	CheckBufferSize(ndData, 1);
	char* ptr = ndData->buffer + ndData->pos;
	ptr[0] = TRUE;
	ndData->pos += 1;
}

void WriteByteToBuffer(struct nd_data* ndData, char ch)
{
	CheckBufferSize(ndData, 1);
	char* ptr = ndData->buffer + ndData->pos;
	*ptr = ch;
	ndData->pos += 1;
}

void WriteFalseToBuffer(struct nd_data* ndData)
{
	CheckBufferSize(ndData, 1);
	char* ptr = ndData->buffer + ndData->pos;
	ptr[0] = FALSE;
	ndData->pos += 1;
}

int CheckBufferSize(struct nd_data* ndData, int insertSize)
{
	int dataSize = ndData->size;
	int needSize = ndData->pos + insertSize;
	int mul = 1;
	while (needSize > dataSize)
	{
		dataSize = ndData->size + mul * BUFSIZ;
		mul *= 2;
	}

	if (dataSize > ndData->size) 
	{
		char* tmpData = ndData->buffer;
		ndData->buffer = (char*) malloc(dataSize * sizeof(char));
		memset(ndData->buffer, '\0', dataSize);
		if (ndData->buffer == NULL)
		{
			return -1;
		}
		if (ndData->pos > 0)
		{
			memcpy(ndData->buffer, tmpData, ndData->pos);
		}
		free(tmpData);
		ndData->size = dataSize;
	}
}

int ReadIntFromBuffer(struct nd_data* ndData)
{
	int value = 0;
	int pos = ndData->pos;
	value += ndData->buffer[pos] & 0x000000FF;
	value += (ndData->buffer[pos + 1] << 8) & 0x0000FF00;
	value += (ndData->buffer[pos + 2] << 16) & 0x00FF0000;
	value += (ndData->buffer[pos + 3] << 24) & 0xFF000000;

	ndData->pos +=4;
	
	return value;
}

char* ReadStringFromBuffer(struct nd_data* ndData) 
{
	char* content;
	int len = ReadIntFromBuffer(ndData);

	content = (char*) malloc(len+1);
	char* p = ndData->buffer + ndData->pos;
	memcpy(content, p, len);
	ndData->pos += len;

	content[len] = '\0';
	return content;
}

char ReadCharFromBuffer(struct nd_data* ndData)
{
	char c = ndData->buffer[ndData->pos];
	ndData->pos += 1;
	return c;
}

BOOLEAN ReadBooleanFromBuffer(struct nd_data* ndData)
{
	return ReadCharFromBuffer(ndData);
}