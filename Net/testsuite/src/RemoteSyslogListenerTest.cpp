//
// POP3ClientSessionTest.cpp
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/RegularExpression.h"
#include "RemoteSyslogListenerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Net/RemoteSyslogListener.h"



RemoteSyslogListenerTest::RemoteSyslogListenerTest(const std::string& name): CppUnit::TestCase(name)
{
}


RemoteSyslogListenerTest::~RemoteSyslogListenerTest()
{
}


void RemoteSyslogListenerTest::setUp()
{
}


void RemoteSyslogListenerTest::testSucc(const std::string &inputString, size_t offset)
{
	size_t pos{offset};
	assertTrue(inputString.substr(offset, inputString.size()- 1 - offset) == Poco::Net::StructuredDataParser::parseStructuredDataString(inputString, pos));
	assertTrue(inputString.size() == pos);
	assertTrue(']' == inputString[pos-2]);
	assertImplementation('[' != inputString[pos-1]);
}

void RemoteSyslogListenerTest::testSucc(const std::string &inputString, size_t offset, const std::string &compare)
{
	size_t pos{offset};
	assertTrue(compare == Poco::Net::StructuredDataParser::parseStructuredDataString(inputString, pos));
	assertTrue(offset + compare.size() + 1 == pos);
	assertTrue(']' == inputString[pos-2]);
	assertTrue('[' != inputString[pos-1]);
}

void RemoteSyslogListenerTest::testFail(const std::string &inputString, size_t offset)
{
	size_t pos{offset};
	assertTrue("" == Poco::Net::StructuredDataParser::parseStructuredDataString(inputString, pos));
}

std::string createMessageStringWithAllChars()
{
    std::string str{};
    for (std::int32_t i{0}; i <= std::numeric_limits<char>::max(); ++i)
    {
		if(i != '"')
		{
        	str += std::string{static_cast<char>(i)};
		}
    }
    return str;
}

std::string createMessageStringWithAllIdChars()
{
    static constexpr char readable_chars_start{33};
    static constexpr char readable_chars_end{127};
    std::string str{};
    for (char i{readable_chars_start}; i < readable_chars_end; ++i)
    {
        if (i != ']' && i != '"' && i != '=' && i != '@')
        {
            str += i;
        }
    }
    return str + '@' + '1';
}

std::string escapeChars(std::string string)
{
	Poco::RegularExpression regexToEscape3(R"(\\)");
	int replace = regexToEscape3.subst(string, R"(\\)");
	Poco::RegularExpression regexToEscape1(R"(")");
	replace = regexToEscape1.subst(string, R"(\")");
	Poco::RegularExpression regexToEscape2(R"(\])");
	replace = regexToEscape2.subst(string, R"(\])");
	return string; 
}

void RemoteSyslogListenerTest::testStructuredData()
{
	testFail(R"(- )");
	testFail(R"([a])");
	testFail(R"([] )");
	testSucc(R"([-] )");
	testFail(R"([=] )");
	testFail(R"([ ] )");
	testFail(R"([]] )");
	testFail(R"(["]"] )");
	testFail(R"([a a] )");
	testFail(R"([a]a] )");
	testFail(R"([a"a]"] )");

	testFail(R"([abciut="3"] )");
	testSucc(R"([abc iut="3"] )");

	testFail(R"([abc iut="3"abc="3"] )");
	testSucc(R"([abc iut="3" abc="3"] )");

	testFail(R"([abc@@ iut="3"abc="3"] )");
	testSucc(R"([abc@123 iut="3" abc="3"] )");

	testFail(R"([abc@123 iut@="3"] )");
	testSucc(R"([abc@123 iut@1="3"] )");
	testSucc(R"([abc@123 iut@1="3 "] )");
	testSucc(R"([abc@123 iut@1=" "] )");
	testSucc(R"([abc@123 iut@1=" 3 "] )");
	testSucc(R"([abc@123 iut@1=" 3"] )");

	std::string structuredDataWithAllPossibleChars{"[" + createMessageStringWithAllIdChars() + " " + createMessageStringWithAllIdChars() + "=\"" + escapeChars(createMessageStringWithAllChars()) + "\"]"};
	testSucc(structuredDataWithAllPossibleChars + " ");
}

void RemoteSyslogListenerTest::testStructuredDataMultipleElements()
{
	testSucc(R"([abc@123 iut@1=" 3"][abc@123 iut@1=" 3"] )");
	testSucc(R"([abc@123 iut@1=" 3"][abc@123 iut@1=" 3"][abc@123 iut@1=" 3"][abc@123 iut@1=" 3"] )");
	std::string structuredDataWithAllPossibleChars{"[" + createMessageStringWithAllIdChars() + " " + createMessageStringWithAllIdChars() + "=\"" + escapeChars(createMessageStringWithAllChars()) + "\"]"};
	testSucc(structuredDataWithAllPossibleChars + structuredDataWithAllPossibleChars + " ");
	testSucc(structuredDataWithAllPossibleChars + structuredDataWithAllPossibleChars + structuredDataWithAllPossibleChars + " ");
}

void RemoteSyslogListenerTest::testStructuredDataWithOffset()
{
	testSucc(R"( [abc@123 iut@1=" 3"] )", 1);
	std::string structuredDataWithAllPossibleChars{"[" + createMessageStringWithAllIdChars() + " " + createMessageStringWithAllIdChars() + "=\"" + escapeChars(createMessageStringWithAllIdChars()) + "\"]"};
	std::string string = structuredDataWithAllPossibleChars + " " + structuredDataWithAllPossibleChars + " ";
	testSucc(string, structuredDataWithAllPossibleChars.size() + 1, structuredDataWithAllPossibleChars);
}

void RemoteSyslogListenerTest::testStructuredDataWithOffsetAndMessage()
{
	testSucc(R"( [abc@123 iut@1=" 3"] message shit, asöldkf)", 1, R"([abc@123 iut@1=" 3"])");
	testSucc(R"( [abc@123 iut@1=" 3"] [adbc@123 iut@4=" 4"])", 1, R"([abc@123 iut@1=" 3"])");
	testFail(R"( [abc@123 iut@1=" 3"] message shit, asöldkf)", 0);
	std::string structuredDataWithAllPossibleChars{"[" + createMessageStringWithAllIdChars() + " " + createMessageStringWithAllIdChars() + "=\"" + escapeChars(createMessageStringWithAllChars()) + "\"]"};
	testSucc(structuredDataWithAllPossibleChars + " " + structuredDataWithAllPossibleChars + " " + structuredDataWithAllPossibleChars + " ", structuredDataWithAllPossibleChars.size() + 1, structuredDataWithAllPossibleChars);
	testSucc(structuredDataWithAllPossibleChars + " " + structuredDataWithAllPossibleChars + structuredDataWithAllPossibleChars + " ", structuredDataWithAllPossibleChars.size() + 1, structuredDataWithAllPossibleChars + structuredDataWithAllPossibleChars);
}

void RemoteSyslogListenerTest::tearDown()
{
}


CppUnit::Test* RemoteSyslogListenerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemoteSyslogListenerTest");

	CppUnit_addTest(pSuite, RemoteSyslogListenerTest, testStructuredData);
	CppUnit_addTest(pSuite, RemoteSyslogListenerTest, testStructuredDataMultipleElements);
	CppUnit_addTest(pSuite, RemoteSyslogListenerTest, testStructuredDataWithOffset);
	CppUnit_addTest(pSuite, RemoteSyslogListenerTest, testStructuredDataWithOffsetAndMessage);

	return pSuite;
}
