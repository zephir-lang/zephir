
namespace Stub\Oo;

class ConstantsInterface implements \Stub\TestInterface
{
	const CLASSNAME = __CLASS__;
	const NAMESPACENAME = __NAMESPACE__;

	public function testReadInterfaceConstant1()
	{
		return \Stub\TestInterface::C1;
	}

	public function testReadInterfaceConstant2()
	{
		return \Stub\TestInterface::C2;
	}

	public function testReadInterfaceConstant3()
	{
		return \Stub\TestInterface::C3;
	}

	public function testReadInterfaceConstant4()
	{
		return \Stub\TestInterface::C4;
	}

	public function testReadInterfaceConstant5()
	{
		return \Stub\TestInterface::C5;
	}

	public function testReadInterfaceConstant6()
	{
		return \Stub\TestInterface::C6;
	}
}
