
namespace Test;

class TryTest
{
	public function testTry1()
	{
		try {}
	}

	public function testTry2()
	{
		try {
			throw new \Exception("error!");
		}
	}

	public function testTry3()
	{
		try {
			throw new \Exception("error!");
		} catch \Exception {
			return "error";
		}
	}

	public function testTry4(boolean a)
	{
		try {
			if a {
				throw new \Exception("error!");
			} else {
				throw new \RuntimeException("error!");
			}
		} catch \Exception {
			return "error";
		} catch \RuntimeException {
			return "domain error";
		}
	}

}
