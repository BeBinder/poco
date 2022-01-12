//
// RemoteSyslogListenerTest.h
//
// Definition of the RemoteSyslogListenerTest class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteSyslogListenerTest_INCLUDED
#define RemoteSyslogListenerTest_INCLUDED


#include "Poco/Net/RemoteSyslogListener.h"
#include "CppUnit/TestCase.h"
#include "../src/StructuredDataParser.h"


class RemoteSyslogListenerTest: public CppUnit::TestCase
{
public:
	RemoteSyslogListenerTest(const std::string& name);
	~RemoteSyslogListenerTest();

	void testStructuredData();
	void testStructuredDataMultipleElements();
	void testStructuredDataWithOffset();
	void testStructuredDataWithOffsetAndMessage();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	void testSucc(const std::string &inputString, size_t offset = 0);
	void testSucc(const std::string &inputString, size_t offset, const std::string &compare);
	void testFail(const std::string &inputString, size_t offset = 0);
};


#endif // RemoteSyslogListenerTest_INCLUDED
