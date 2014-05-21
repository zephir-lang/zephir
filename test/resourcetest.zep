
namespace Test;

class ResourceTest
{
	public function testLetStatementSTDIN()
	{
		var a;
		let a = STDIN;

		return a;
	}

	public function testLetStatementSTDOUT()
	{
		var a;
		let a = STDOUT;

		return a;
	}

	public function testLetStatementSTDERR()
	{
		var a;
		let a = STDERR;

		return a;
	}

	public function testTypeOffResource()
	{
		var a;
		let a = STDIN;

		return typeof a;
	}

	public function testIsResource()
	{
		var a;
		let a = STDIN;

		return is_resource(a);
	}

	public function testFunctionsForSTDIN()
	{
		var a;
		let a = STDIN;

		stream_set_blocking(a, 1);
	}
}
