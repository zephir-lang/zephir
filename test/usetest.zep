
namespace Test;

use stdClass as StandardClass;
use Countable;

class UseTest implements Countable
{
	public function createInstance()
	{
		return new StandardClass();
	}

	public function count()
	{

	}
	
}
