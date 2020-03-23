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
			char* output = RunInterpreter("CanAddTwoIntegers");
			Assert::AreEqual("13", output);
			ClearOutputFile("CanAddTwoIntegers");
		} 

		TEST_METHOD(CanSubtractTwoIntegers) {
			char* output = RunInterpreter("CanSubtractTwoIntegers");
			Assert::AreEqual("8", output);
			ClearOutputFile("CanSubtractTwoIntegers");
		}
	};
} 