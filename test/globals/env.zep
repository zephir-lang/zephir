
namespace Test\Globals;

class Env
{
	public function read(string name) -> var
	{
		return _ENV[name];
	}
}
