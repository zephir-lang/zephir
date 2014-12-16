namespace Test;

class Vars
{
    public function testVarDump()
    {
        var a, ar;

        let a = "hello";
        let ar = [1, "world", false];

        var_dump(ar);
        var_dump(a);
    }

    public function testVarDump2(ret)
    {
        var_dump(ret == 1);
    }

    public function testVarExport()
    {
        var a, ar, ret;

        let a = "hello";
        let ar = [1, "world", false];

        var_export(ar);
        let ret = var_export(ar);

        var_export(a);
        let ret = var_export(a);

        return ret;
    }

    public function test88Issue(string! param1, string param2 = null)
    {
        var_dump(param1);
        var_dump(param2);

        var_export(param1);
        var_export(param2);
    }

    public function test88IssueParam2InitString(string! param1, string param2 = "test string")
    {
        var_export(param2);
    }

    public function testVarDump2param(var p1, var p2) {
        var_dump(p1, p2);
    }

    public function testVarDump3param(var p1, var p2, var p3) {
        var_dump(p1, p2, p3);
    }

    public function testCountOptimizerVarDumpAndExport(var testVar)
    {
        var_dump(count(testVar));
        var_export(count(testVar));
    }

	public function testArrayTypeVarDumpAndExport(array testVar = [])
	{
		var_dump(testVar);
		var_export(testVar);
	}

    /**
     * @link https://github.com/phalcon/zephir/issues/681
     */
	public function testIntVarDump()
	{
	    int a;
	    let a = 1;

	    var_dump(a);
	    var_export(a);
	}

	public function testDoubleVarDump()
	{
	    double a;
	    let a = 1;

	    var_dump(a);
	    var_export(a);
	}

	public function testBoolVarDump()
	{
	    bool a;
	    let a = true;

	    var_dump(a);
	    var_export(a);
	}
}
