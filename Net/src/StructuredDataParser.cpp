//
// RemoteSyslogListener.cpp
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogListener
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "StructuredDataParser.h"


namespace Poco {
namespace Net {


const Poco::RegularExpression Poco::Net::StructuredDataParser::structuredDataRegex{R"(\[(\^|[^\s^\]^@^"^=])+(@[0-9]+)?( ((\^|[^\s^\]^@^"^=])+(@[0-9]+)?="(\^|[^\\^"^\]]|((\\")|(\\\])|(\\\\)))*" )*((\^|[^\s^\]^@^"^=])+(@[0-9]+)?="(\^|[^\\^"^\]]|((\\")|(\\\])|(\\\\)))*"))?\][ \[])"};
std::string Poco::Net::StructuredDataParser::parseStructuredDataString(const std::string& line, std::size_t& pos)
{
	static constexpr char nullvalue{'-'};
	std::string result{};
	Poco::RegularExpression::Match match;

	if(line[pos] == nullvalue)
	{
		pos+=2;
		return result;
	}

	static constexpr size_t shortestPossibleStructuredDataElement{3U};
	const size_t maxIterations{line.size() / shortestPossibleStructuredDataElement};

	for(size_t i = 0; i < maxIterations; ++i)
	{
		structuredDataRegex.match(line,pos,match);
		if(match.offset == pos && match.length > 1)
		{
			result += line.substr(pos,match.length-1);
			pos += match.length-1;
		}
		else
		{
			break;
		}
	}

	pos+=1;
	return result;
}

} } // namespace Poco::Net
