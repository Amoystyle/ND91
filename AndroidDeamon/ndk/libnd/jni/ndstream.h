#pragma once

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <buffer.h>
#include <linux/fb.h>

#include "log.h"

#ifndef _ND_H
#define _ND_H

extern const int NDPC_HEAD_LENGTH;
extern const char NDPC[4];
extern char STD_OUTPUT_BUFFER[4096];
#endif

char* GetHead(unsigned char businessId, int len);
void WriteData(int output, char* data, int len);
char* Read(int input, int len);
void Write(int output, unsigned char businessId, char* data, int len);
int ReadFromStream(FILE* stream, char** content, int length);
int ReadFromStreamEnd(FILE* stream, char** content, int timeout);
