
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
		loop {
			return mt_rand(0, 100);
		}
	}

	public function testCall3()
	{
		var handle, handle2, buffer;

		let handle = fopen("inputfile.txt", "r"), handle2 = fopen("outputfile.txt", "w");
		if handle {
			loop {
				let buffer = fgets(handle, 4096);
				if buffer === false {
					break;
				}
				fwrite(handle2, buffer);
			}
			fclose(handle);
			fclose(handle2);
		}
	}

	public function testCall4()
	{
		var handle, handle2, buffer;

		let handle = fopen(mode: "r", filename: "inputfile.txt"), 
			handle2 = fopen(filename: "outputfile.txt", mode: "w");
		if handle {
			loop {
				let buffer = fgets(handle, 4096);
				if buffer === false {
					break;
				}
				fwrite(handle2, buffer);
			}
			fclose(handle);
			fclose(handle2);
		}
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
