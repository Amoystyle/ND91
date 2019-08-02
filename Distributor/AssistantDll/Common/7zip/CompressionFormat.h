#pragma once


#include "Enum.h"


namespace SevenZip
{
	struct CompressionFormat
	{
		enum _Enum
		{
			Unknown,
			SevenZip,
			Zip
		};
	
		typedef intl::EnumerationDefinitionNoStrings _Definition;
		typedef intl::EnumerationValue< _Enum, _Definition, Unknown > _Value;
	};
	
	typedef CompressionFormat::_Value CompressionFormatEnum;
}
