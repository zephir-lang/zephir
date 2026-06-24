namespace Stub;

class ExitDie
{
	public function testExit(var param = null) -> void
	{
		if empty(param) {
			exit();
		}

		exit(param);
	}

	public function testDie(var param = null) -> void
	{
		if empty(param) {
			die();
		}

		die(param);
	}
}
