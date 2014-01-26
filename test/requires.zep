
namespace Test;

class Requires
{

	public function requireExternal1(var path)
	{
		return require path;
	}

	public function requireExternal2(var path)
	{
		require path;
		return true;
	}

}
