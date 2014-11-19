namespace Test;

class ExitDie
{

	public function testExit(param = "")
	{
		if typeof param == "string" && param == "" {
			exit();
		} else {
			exit(param);
		}
	}

	public function testDie(param = "")
	{
		if typeof param == "string" && param == "" {
			die();
		} else {
			die(param);
		}
	}
}
