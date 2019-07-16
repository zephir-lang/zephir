namespace Test;

class Operator
{
	public function testIdentical(param1, param2)
	{
		return param1 === param2;
	}

	public function testIdenticalIfComplex(var input)
	{
		var a, b;
		let a = ["1": input];
		let b = false;

		if b !== a["1"] {
			return true;
		}
		return false;
	}

	public function testIdenticalVarFalse(param)
	{
		return param === FALSE;
	}

	public function testIdenticalFalseVar(param)
	{
		return FALSE === param;
	}

	public function testBoolComparison(varTrue, varFalse)
    {
		var var1 = true;
        var var2 = false;

        return [
			varTrue == var1,
			varTrue === var1,
			varFalse == var2,
			varFalse === var2,
			varTrue != var2,
			varTrue !== var2,
			varFalse != var1,
			varFalse !== var1
	  	];
    }

    public function testNewInstanceOperator(className)
    {
        return new {className}();
    }

}
