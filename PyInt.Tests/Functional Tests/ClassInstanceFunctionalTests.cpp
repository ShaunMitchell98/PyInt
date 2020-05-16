#include "CppUnitTest.h"
extern "C" {
#include "../../PyInt/src/RunFile.h"
#include "FunctionalTestUtilities.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntFunctionalTests
{
	TEST_CLASS(ClassInstanceFunctionalTests)
	{
	public:
		TEST_METHOD(CanSetProperty)
		{
			char* output = RunInterpreter("class MyClass():\n\tpass\ninstance = MyClass()\ninstance.value = 5\nprint(instance.value)");
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(CanUpdateProperty) {
			char* output = RunInterpreter("class MyClass():\n\tpass\ninstance = MyClass()\ninstance.value = 5\n\ninstance.value = 'Hello World'\nprint(instance.value)");
			Assert::AreEqual("Hello World", output);
			free(output);
		}
	};
}