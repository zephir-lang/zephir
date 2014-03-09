
namespace Test;

class InternalClasses
{
	public function testStaticCall()
	{
        return \Phalcon\Di::getDefault();
	}
}