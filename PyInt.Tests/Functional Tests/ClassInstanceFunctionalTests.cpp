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
			char* output = RunInterpreter("class MyClass:\n\tpass\ninstance = MyClass()\ninstance.value = 5\nprint(instance.value)");
			Assert::AreEqual("5", output);
			free(output);
		}

		TEST_METHOD(CanUpdateProperty) {
			char* output = RunInterpreter("class MyClass:\n\tpass\ninstance = MyClass()\ninstance.value = 5\n\ninstance.value = 'Hello World'\nprint(instance.value)");
			Assert::AreEqual("Hello World", output);
			free(output);
		}

		TEST_METHOD(CanCallMethod) {
			char* output = RunInterpreter("class MyClass:\n\tdef numberPrinter(self):\n\t\treturn 6 + 7\ninstance = MyClass()\nprint(instance.numberPrinter())");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanCallMethodFromAsProperty) {
			char* output = RunInterpreter("class MyClass:\n\tdef numberPrinter(self):\n\t\treturn 6+7\ninstance = MyClass()\nmethod = instance.numberPrinter\nprint(method())");
			Assert::AreEqual("13", output);
			free(output);
		}

		TEST_METHOD(CanAccessPropertyFromInstance) {
			char* output = RunInterpreter("class MyClass:\n\tproperty = 6\n\tproperty2 = 5\ninstance = MyClass()\nprint(instance.property + instance.property2)");
			Assert::AreEqual("11", output);
			free(output);
		}
	};
}