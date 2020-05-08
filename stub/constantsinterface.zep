
namespace Stub;

class ConstantsInterface implements TestInterface
{
	public function testReadInterfaceConstant1()
	{
		return TestInterface::C1;
	}

	public function testReadInterfaceConstant2()
	{
		return TestInterface::C2;
	}

	public function testReadInterfaceConstant3()
	{
		return TestInterface::C3;
	}

	public function testReadInterfaceConstant4()
	{
		return TestInterface::C4;
	}

	public function testReadInterfaceConstant5()
	{
		return TestInterface::C5;
	}

	public function testReadInterfaceConstant6()
	{
		return TestInterface::C6;
	}

	public function testReadInheritanceFromInterfaceConstant1()
	{
		return self::C1;
	}

	public function testReadInheritanceFromInterfaceConstant2()
	{
		return self::C2;
	}

	public function testReadInheritanceFromInterfaceConstant3()
	{
		return self::C3;
	}

	public function testReadInheritanceFromInterfaceConstant4()
	{
		return self::C4;
	}

	public function testReadInheritanceFromInterfaceConstant5()
	{
		return self::C5;
	}

	public function testReadInheritanceFromInterfaceConstant6()
	{
		return self::C6;
	}
}
