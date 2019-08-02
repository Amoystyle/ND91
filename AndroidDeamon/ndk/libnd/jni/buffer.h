
#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "common.h"

void WriteTrueToBuffer(struct nd_data* ndData);
void WriteFalseToBuffer(struct nd_data* ndData);
void WriteByteToBuffer(struct nd_data* ndData, char ch);
void WriteIntToBuffer(struct nd_data* ndData, int value);
void WriteIntToBufferInPos(struct nd_data* ndData, int pos, int value);
void WriteByteToBufferInPos(struct nd_data* ndData, int pos, char ch);
void WriteUIntToBuffer(struct nd_data* ndData, unsigned int value);
void WriteStringToBuffer(struct nd_data* ndData, char* content, int length);
void WriteULongToBuffer(struct nd_data* ndData, unsigned long value);
char ReadCharFromBuffer(struct nd_data* ndData);
char* ReadStringFromBuffer(struct nd_data* ndData);
int ReadIntFromBuffer(struct nd_data* ndData);
int CheckBufferSize(struct nd_data* ndData, int insertSize);
BOOLEAN ReadBooleanFromBuffer(struct nd_data* ndData);