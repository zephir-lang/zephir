
namespace Stub\Globals;

class Env
{
	public function read(string name) -> var
	{
		return isset _ENV[name] ? _ENV[name] : false;
	}
}
