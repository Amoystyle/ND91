#pragma once
extern "C" void Encrypt(char szPlain[], byte key[], byte *pCryptBuf);
extern "C" void Decrypt(byte crypt[], byte key[], char *outPtr);
