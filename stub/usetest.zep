
namespace Stub;

use stdClass as StandardClass;
use Countable;

use Oo\OoConstruct;
use Oo as NsAlias;
use Strings as TestStrings;
use Stub\Echoes as Echoes;

class UseTest implements Countable
{
	public function createInstance()
	{
		return new StandardClass();
	}

	public function count() -> int
	{
	    return 0;
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
