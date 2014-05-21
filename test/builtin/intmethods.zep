
namespace Test\BuiltIn;

class IntMethods
{
	public function getAbs(int num)
	{
		return num->abs();
	}

	public function getAbs1()
	{
		return -5->abs();
	}

	public function getBinary(int num)
	{
		return num->toBinary();
	}

	public function getHex(int num)
	{
		return num->toHex();
	}

	public function getOctal(int num)
	{
		return num->toOctal();
	}

	public function getPow(int num, int exp)
	{
		return num->pow(exp);
	}

	public function getSqrt(int num)
	{
		return num->sqrt();
	}

	public function getExp(int num)
	{
		return num->exp();
	}

	public function getSin(int num)
	{
		return num->sin();
	}

	public function getCos(int num)
	{
		return num->cos();
	}

	public function getTan(int num)
	{
		return num->tan();
	}

	public function getAsin(int num)
	{
		return num->asin();
	}

	public function getAcos(int num)
	{
		return num->acos();
	}

	public function getAtan(int num)
	{
		return num->atan();
	}

	public function getLog(int num, int base = -1)
	{
		if base === -1 {
			return num->log();
		}
		return num->log(base);
	}
}