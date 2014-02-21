
namespace Test;

use Exception, RuntimeException;

class TryTest
{
	public function testThrow1()
	{
		throw new Exception("error");
	}

	public function testThrow2()
	{
		var message = "error";
		throw new Exception(message);
	}

	public function testTry1()
	{
		try {}
	}

	public function testTry2()
	{
		try {
			throw new Exception("error!");
		}
	}

	public function testTry3()
	{
		try {
			throw new Exception("error!");
		} catch Exception {
			return "error";
		}

		return false;
	}

	public function testTry4(boolean a)
	{
		try {
			if a {
				throw new Exception("error!");
			} else {
				throw new RuntimeException("error!");
			}
		} catch RuntimeException {
			return "domain error";
		} catch Exception {
			return "error";
		}

		return false;
	}

	public function testTry5(boolean a)
	{
		try {
			if a {
				throw new Exception("error!");
			} else {
				throw new RuntimeException("error!");
			}
		} catch RuntimeException|Exception  {
			return "any error";
		}

		return false;
	}

	public function testTry6(boolean a)
	{
		var e;

		try {
			if a {
				throw new Exception("error!");
			} else {
				throw new RuntimeException("error!");
			}
		} catch RuntimeException, e {
			return "domain error";
		} catch Exception, e {
			return "error";
		}

		return false;
	}

	public function testTry7(boolean a)
	{
		var e;

		try {
			if a {
				throw new Exception("error!");
			} else {
				throw new RuntimeException("error!");
			}
		} catch RuntimeException|Exception, e  {
			return "any error";
		}
	}

	public function testTry8()
	{
		try {
			throw new Exception("error 1!");
		}
		throw new Exception("error 2!");
	}

}
