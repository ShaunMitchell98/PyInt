#include "CppUnitTest.h"

extern "C" {
#include "Scanner.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntUnitTests
{
	TEST_CLASS(ScannerUnitTests)
	{
	public:

		TEST_METHOD(InitScanner_GivenSourceCode_InitialisesScanner)
		{
			Scanner scanner;
			const char* sourceCode = "Source code";
			const char* path = "path";
			InitScanner(&scanner, sourceCode, path);

			Assert::AreEqual("path", scanner.path);
			Assert::AreEqual("Source code", scanner.start);
			Assert::AreEqual("Source code", scanner.current);
			Assert::AreEqual(1, scanner.stackIndex);
			Assert::AreEqual(0, scanner.indentStack[0]);
		}
	};
}