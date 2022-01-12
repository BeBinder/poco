//
// RemoteSyslogListener.h
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogListener
//
// Definition of the RemoteSyslogListener class.
//
// Copyright (c) 2007-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_StructuredDataParser_INCLUDED
#define Net_StructuredDataParser_INCLUDED


#include "Poco/RegularExpression.h"


namespace Poco {
namespace Net {

class StructuredDataParser
{
public:
	static std::string parseStructuredDataString(const std::string& line, std::size_t& pos);
private:
	static const Poco::RegularExpression structuredDataRegex;	
};

} } // namespace Poco::Net


#endif // Net_StructuredDataParser_INCLUDED
