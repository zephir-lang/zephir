
namespace Test\Optimizers;

class CreateArray
{
	public function createNoSize()
	{
		return create_array();
	}

	public function createSize(int n)
	{
		return create_array(n);
	}
}
