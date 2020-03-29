#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(LogicFunctionalTests)
	{
	public:
		TEST_METHOD(IntegerGreaterThan)
		{
			char* output1 = RunInterpreter("print 7 > 6;");
			char* output2 = RunInterpreter("print 6 > 7;");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			free(output1);
			free(output2);
		}

		TEST_METHOD(IntegerLessThan) {
			char* output1 = RunInterpreter("print 6 < 7;");
			char* output2 = RunInterpreter("print 7 < 6;");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			free(output1);
			free(output2);
		}

		TEST_METHOD(IntegerGreaterThanOrEqualTo) {
			char* output1 = RunInterpreter("print 7 >= 7;");
			char* output2 = RunInterpreter("print 7 >= 5;");
			char* output3 = RunInterpreter("print 5 >= 7");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("true", output2);
			Assert::AreEqual("false", output3);
			free(output1);
			free(output2);
			free(output3);
		}

		TEST_METHOD(IntegerLessThanOrEqualTo) {
			char* output1 = RunInterpreter("print 7 <= 7;");
			char* output2 = RunInterpreter("print 7 <= 5;");
			char* output3 = RunInterpreter("print 5 <= 7");
			Assert::AreEqual("true", output1);
			Assert::AreEqual("false", output2);
			Assert::AreEqual("true", output3);
			free(output1);
			free(output2);
			free(output3);
		}
	};
}