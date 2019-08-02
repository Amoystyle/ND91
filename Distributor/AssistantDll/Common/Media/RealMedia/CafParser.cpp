#include "stdafx.h"
#include "CafParser.h"

CafParser::CafParser( const wstring path )
: MediaBase(path)
{		
    _pDesc = NULL;
    _pHead = NULL;
}

CafParser::~CafParser(void)
{
    SAFE_DELETE(_pHead);
    SAFE_DELETE(_pDesc);
    RELEASE_VECTOR(_chunks);
}

bool ND91Assistant::CafParser::ParserFormat()
{
    RETURN_FALSE_IF(_pHead);

	BinaryBufferReader reader(_path);
    reader.SetBigEndian(true);

    // 读取文件头和desc数据段
    _pHead = new CAFFileHeader(&reader);
    Chunk desc(&reader);    // unused
    _pDesc = new DescChunkBody(&reader);

    // 读取其他数据段
    while( ! reader.IsEnd() )
    {
        Chunk* pChunk = new Chunk(&reader);
        pChunk->ReadBody(&reader);

        _chunks.push_back(pChunk);
    }

    return true;
}

long ND91Assistant::CafParser::GetMediaDuration()
{
    RETURN_VALUE_IF( ! _pHead, -1);

    switch(_pDesc->_formatId)
    {
    case CAF_LinearPCM:     // 'lpcm'
        {
            //  as long as the format has a constant number of frames per packet,
            //  you can calculate the duration of each packet by dividing the
            //  SampleRate value by the FramesPerPacket value.
            double packetLength = _pDesc->_framesPerPacket / _pDesc->_sampleRate;

            int packetCount = GetDataLength() / _pDesc->_bytesPerPacket;
            double duration = packetLength * packetCount;
            return long(duration + 0.5); // 四舍五入
        }

    case CAF_AppleIMA4:     // 'ima4'
    case CAF_MPEG4AAC:      // 'aac '
    case CAF_MACE3:         // 'MAC3'
    case CAF_MACE6:         // 'MAC6'
    case CAF_ULaw:          // 'ulaw'
    case CAF_ALaw:          // 'alaw'
    case CAF_MPEGLayer1:    // '.mp1'
    case CAF_MPEGLayer2:    // '.mp2'
    case CAF_MPEGLayer3:    // '.mp3'
    case CAF_AppleLossless: // 'alac'
    default:
        return -1;
    }


    //  The duration of the audio in the file is mNumberValidFrames value divided by the sample rate specified in the file’s Audio Description chunk.

}

int CafParser::GetDataLength()
{
    int bytes = 0;

    for(unsigned int i = 0; i < _chunks.size(); ++i)
    {
        Chunk* pChunk = _chunks[i];
        if ( pChunk->_type == DATA_CHUNK )
        {
            bytes += (__int32)pChunk->_size - 4;
        }
    }

    return bytes;
}
