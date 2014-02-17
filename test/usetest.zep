
namespace Test;

use \stdClass as StandardClass;
use \Countable;

use Oo\OoConstruct;
use Oo as NsAlias;
use Strings as TestStrings;
use \Test\Echoes as Echoes;

class UseTest implements Countable
{
	public function createInstance()
	{
		return new StandardClass();
	}

	public function count()
	{

	}

	public function testUseClass1()
	{
		return new OoConstruct();
	}

	public function testUseClass2()
	{
		return new TestStrings();
	}

	public function testUseNamespaceAlias()
	{
		return new NsAlias\OoConstruct();
	}
}
