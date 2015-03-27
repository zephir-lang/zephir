namespace Test;

class Operator
{
	public function testIdentical(param1, param2)
	{
		return param1 === param2;
	}

	public function testIdenticalVarFalse(param)
	{
		return param === FALSE;
	}

	public function testIdenticalFalseVar(param)
	{
		return FALSE === param;
	}
}
