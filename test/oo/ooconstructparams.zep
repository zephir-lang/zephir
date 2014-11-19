
/**
 * Class with constructor + params
 */

namespace Test\Oo;

class OoConstructParams
{
	protected a;

	protected b;

	public function __construct(a, b)
	{
		let this->a = a,
			this->b = b;
	}

	public function getA()
	{
		return this->a;
	}

	public function getB()
	{
		return this->b;
	}
}