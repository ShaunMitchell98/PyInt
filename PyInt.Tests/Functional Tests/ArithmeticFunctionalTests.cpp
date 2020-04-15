#include "CppUnitTest.h"
extern "C" {
#include "RunFile.h"
#include "VM.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(ArithmeticFunctionalTests)
	{
	public:	
    	TEST_METHOD(CanAddTwoIntegers)
		{
			char* output = RunInterpreter("print(6+7)");
			Assert::AreEqual("13", output);
			free(output);
		} 

		TEST_METHOD(CanSubtractTwoIntegers) {
			char* output = RunInterpreter("print(12-4)");
			Assert::AreEqual("8", output);
			free(output);
		}

		TEST_METHOD(CanMultiplyTwoIntegers) {
			char* output = RunInterpreter("print(6*7)");
			Assert::AreEqual("42", output);
			free(output);
		}

		TEST_METHOD(CanDivideTwoIntegers) {
			char* output = RunInterpreter("print(16/8)");
			Assert::AreEqual("2", output);
			free(output);
		}
	};
} 