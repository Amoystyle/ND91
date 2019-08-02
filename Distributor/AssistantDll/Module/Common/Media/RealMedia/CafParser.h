#pragma once

#include "Module/Common/Media/MediaBase.h"
#include "Module/Common/Media/MediaData.h"
#include "Common/BinaryBufferReader.h"
#include "Common/AssistantException.h"
#include "AssistantDll.h"

namespace ND91Assistant
{
	// aac文件解析类
	class CafParser : public MediaBase
	{ 
	public:

		CafParser( const wstring path );
		~CafParser(void);

		// 获取媒体文件播放时长
		virtual	long GetMediaDuration();

		// 判断是否属于某种格式文件
		bool ParserFormat();
        int GetDataLength();
    private:
        enum FORMAT_ID{
            CAF_LinearPCM      = 'lpcm',
            CAF_AppleIMA4      = 'ima4',
            CAF_MPEG4AAC       = 'aac ',
            CAF_MACE3          = 'MAC3',
            CAF_MACE6          = 'MAC6',
            CAF_ULaw           = 'ulaw',
            CAF_ALaw           = 'alaw',
            CAF_MPEGLayer1     = '.mp1',
            CAF_MPEGLayer2     = '.mp2',
            CAF_MPEGLayer3     = '.mp3',
            CAF_AppleLossless  = 'alac'
        };

        enum CHUNK_TYPE {
            DATA_CHUNK              =   'data',
            PACKET_TABLE_CHUNK      =   'pakt',
            CHANNEL_LAYOUT_CHUNK    =   'chan',
            MAGIC_COOKIE_CHUNK      =   'kuki',
            STRINGS_CHUNK           =   'strg',
            MARKER_CHUNK            =   'mark',
            REGION_CHUNK            =   'regn',
            INSTRUMENT_CHUNK        =   'inst',
            MIDI_CHUNK              =   'midi',
            OVERVIEW_CHUNK          =   'ovvw',
            PEAK_CHUNK              =   'peak',
            EDIT_COMMENT_CHUNK      =   'edct',
            INFORMATION_CHUNK       =   'info',
            UNIQUE_MATERIAL_ID_CHUNK=   'umid',
            USER_DEFINE_CHUNK       =   'uuid',
            FREE_CHUNK              =   'free',
        };
        enum LPCM_FORMAT_FLAG {
            kCAFLinearPCMFormatFlagIsFloat  = (1L << 0),   // 1 for floating point, 0 for signed integer.
            kCAFLinearPCMFormatFlagIsLittleEndian  = (1L << 1) // 1 for little endian, 0 for big endian.
        };

        enum AAC_FORMAT_FLAG{
            kMP4Audio_AAC_LC_ObjectType  = 2
        };

        class CAFFileHeader {
        public:
            CAFFileHeader(BinaryBufferReader* pReader)
            {
                pReader->ReadUInt32(_type);
                pReader->ReadUInt16(_version);
                pReader->ReadUInt16(_flags);
            }
        private:
            /*UInt32*/uint32_t  _type;	    // 'caff'
            /*UInt16*/uint16_t  _version;
            /*UInt16*/uint16_t  _flags;	    // reserved， set to 0
        };

        class ChunkBody
        {
        };

        class DescChunkBody
        {
        public:
            DescChunkBody(BinaryBufferReader* pReader)
            {
                pReader->ReadDouble(_sampleRate);
                pReader->ReadUInt32(_formatId);
                pReader->ReadUInt32(_formatFlags);
                pReader->ReadUInt32(_bytesPerPacket);
                pReader->ReadUInt32(_framesPerPacket);
                pReader->ReadUInt32(_channelsPerFrame);
                pReader->ReadUInt32(_bitsPerChannel);
            }         

            double              _sampleRate;        // number of sample frames per second of the data. must be nonzero.
            /*unsigned __int32*/uint32_t    _formatId;          // four-character code indicating the general kind of data in the stream
            /*unsigned __int32*/uint32_t    _formatFlags;       // Flags specific to each format. May be set to 0 to indicate no format flags.
            /*unsigned __int32*/uint32_t    _bytesPerPacket;    // number of bytes in a packet of data. set to 0 for formats with a variable packet size
            /*unsigned __int32*/uint32_t    _framesPerPacket;   // 1
            /*unsigned __int32*/uint32_t    _channelsPerFrame;  // 1
            /*unsigned __int32*/uint32_t    _bitsPerChannel;    // 10
        };

        class PacketTableChunkBody : public ChunkBody
        {
        public:
            PacketTableChunkBody(BinaryBufferReader* pReader, /*__int32*/int32_t size)
            {
            }

        };

        class UnknownChunkBody : public ChunkBody
        {
        public:
            UnknownChunkBody(BinaryBufferReader* pReader, /*__int32*/int32_t size)
            {
                pReader->AddOffset(size);
            }
        };

        class DataChunkBody : public ChunkBody
        {
        public:
            DataChunkBody(BinaryBufferReader* pReader, /*__int32*/int32_t size)
            {
                pReader->ReadUInt32(_editCount);
                if ( size == -1 )
                {
                    // 数据段长度等于-1时，表示一直到文件结束
                    size = pReader->GetSize() - pReader->GetOffset();
                }
                else
                {
                    size -= 4;  // _editCount占用4个字节
                }

                pReader->AddOffset(size);
            }

            /*UINT32*/uint32_t  _editCount;
            /*__int32*/int32_t _size;
        };

        class Chunk
        {
        public:
            Chunk(BinaryBufferReader* pReader)
            {
                pReader->ReadUInt32(_type);
                pReader->ReadInt64(_size);
                _pBody = NULL;
            }
            ~Chunk()
            {
                SAFE_DELETE(_pBody);
            }

            void ReadBody(BinaryBufferReader* pReader)
            {
                switch(_type)
                {
                case DATA_CHUNK              :     //  'data'
                case PACKET_TABLE_CHUNK      :     //  'pakt'
                case CHANNEL_LAYOUT_CHUNK    :     //  'chan'
                case MAGIC_COOKIE_CHUNK      :     //  'kuki'
                case STRINGS_CHUNK           :     //  'strg'
                case MARKER_CHUNK            :     //  'mark'
                case REGION_CHUNK            :     //  'regn'
                case INSTRUMENT_CHUNK        :     //  'inst'
                case MIDI_CHUNK              :     //  'midi'
                case OVERVIEW_CHUNK          :     //  'ovvw'
                case PEAK_CHUNK              :     //  'peak'
                case EDIT_COMMENT_CHUNK      :     //  'edct'
                case INFORMATION_CHUNK       :     //  'info'
                case UNIQUE_MATERIAL_ID_CHUNK:     //  'umid'
                case USER_DEFINE_CHUNK       :     //  'uuid'
                case FREE_CHUNK              :     //  'free'
                    break;  // 一切正常

                default:
                    THROW_ASSISTANT_EXCEPTION(NOT_SUPPORT); // 不认识的ID
                    break;
                }

                SAFE_DELETE(_pBody);

                // 没有打算支持2G以上的文件
                /*__int32*/int32_t size = (/*__int32*/int32_t) _size;

                switch(_type)
                {
                case 'data':
                    {
                        DataChunkBody* pBody = new DataChunkBody(pReader, size);
                        if ( _size == -1 )
                            _size = pBody->_size + 4;

                        _pBody = pBody;
                    }
                    break;

                case 'pakt':
                    _pBody = new PacketTableChunkBody(pReader, size);
                    break;

                default:
                    _pBody = new UnknownChunkBody(pReader, size);
                    break;
                }

            };

            /*unsigned __int32*/uint32_t    _type;	// 4byte，仅允许小写字母和空格、点
            /*__int64*/int64_t             _size;	// chunk数据区长度（若=-1，则必须是最后一个Chunk）
            ChunkBody*          _pBody; // Chunk内容
        };

    private:
        CAFFileHeader*  _pHead;     // 文件头
        DescChunkBody*  _pDesc;     // desc数据段
        vector<Chunk*>  _chunks;   // 除了desc之外的其他数据段
	};
}
