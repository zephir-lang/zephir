
/**
 * Function calls
 */

namespace Test;

class Fcall
{
	public function testCall1()
	{
		return strpos("hello", "h");
	}

	public function testCall2()
	{
		var result, row; int debitos = 0, creditos = 0;

		mysql_connect("localhost", "root", "hea101");

		mysql_select_db("ramocol");

		let result = mysql_query("SELECT deb_cre, cuenta FROM movi");
		loop {

			let row = mysql_fetch_array(result);
			if typeof row != "array" {
				break;
			}

			if row["deb_cre"] == "D" {
				let debitos++;
			} else {
				let creditos++;
			}
		}

		return [debitos, creditos];
	}

	public function testCall1FromVar()
	{
		var funcName;
		let funcName = "strpos";
		return {funcName}("hello", "l");
	}

	public function testStrtokFalse()
	{
		return strtok("/");
	}

	public function testStrtokVarBySlash(var value)
	{
		return strtok(value, "/");
	}

	public function testFunctionGetArgs(var param1, var param2)
	{
		return func_get_args();
	}
}
