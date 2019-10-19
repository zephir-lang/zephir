
namespace Test\Globals;

class Server
{
	public function f1() -> void
	{
		echo _SERVER["PHP_SELF"];
		this->f2();
		echo _SERVER["PHP_SELF"];
	}

	public function f2() -> void
	{
		echo _SERVER["SCRIPT_NAME"];
	}

	public function check() {
		return _SERVER["HTTP_USER_AGENT"];
	}
}
