
namespace Test\Oo;

class ConstantsInterface implements \Test\TestInterface
{
	const CLASSNAME = __CLASS__;
	const NAMESPACENAME = __NAMESPACE__;

	public function testReadInterfaceConstant1()
	{
		return \Test\TestInterface::C1;
	}

	public function testReadInterfaceConstant2()
	{
		return \Test\TestInterface::C2;
	}

	public function testReadInterfaceConstant3()
	{
		return \Test\TestInterface::C3;
	}

	public function testReadInterfaceConstant4()
	{
		return \Test\TestInterface::C4;
	}

	public function testReadInterfaceConstant5()
	{
		return \Test\TestInterface::C5;
	}

	public function testReadInterfaceConstant6()
	{
		return \Test\TestInterface::C6;
	}
}
