namespace Test;

class ConstantsInterfaceA implements TestInterface
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
}
