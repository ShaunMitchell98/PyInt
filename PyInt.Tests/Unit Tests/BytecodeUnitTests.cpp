#include "CppUnitTest.h"
extern "C" {
#include "../../PyInt/src/Services/Bytecode/Bytecode.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PyIntUnitTests
{
	TEST_CLASS(BytecodeUnitTests)
	{
	public:

		TEST_METHOD(InitBytecode_GivenBytecodePointer_InitialisesBytecode)
		{
			Bytecode bytecode;
			InitBytecode(&bytecode);

			Assert::AreEqual(0, bytecode.count);
			Assert::AreEqual(0, bytecode.capacity);
			Assert::IsNull(bytecode.code);
			Assert::IsNull(bytecode.lines);
			Assert::IsNotNull(&bytecode.constants);
		}
	};
}