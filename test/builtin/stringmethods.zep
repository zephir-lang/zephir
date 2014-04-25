
namespace Test\BuiltIn;

class StringMethods
{
	public function getLength1()
	{
		return "hello"->length();
	}

	public function getLength2()
	{
		return ("hello")->length();
	}

	public function getLength3()
	{
		return ("hello" . "hello")->length();
	}

	public function getLength4(string a)
	{
		return a->length();
	}

	public function getLength5(string a)
	{
		return ("hello" . a)->length();
	}

	public function getIndex(string str, string needle)
	{
		return str->index(needle);
	}

	public function getIndexWithPosition(string str, string needle, int position)
	{
		return str->index(needle, position);
	}

	public function getTrimmed()
	{
		return " hello \t\n"->trim();
	}

	public function getTrimmed1(string str)
	{
		return str->trim();
	}

	public function getLeftTrimmed(string str)
	{
		return str->trimLeft();
	}

	public function getRightTrimmed(string str)
	{
		return str->trimRight();
	}

	public function getLower(string str)
	{
		return str->lower();
	}

	public function getUpper(string str)
	{
		return str->upper();
	}

	public function getLowerFirst(string str)
	{
		return str->lowerFirst();
	}

	public function getUpperFirst(string str)
	{
		return str->upperFirst();
	}

	public function getFormatted(string str)
	{
		return "hello %s!"->format(str);
	}

	public function getMd5(string str)
	{
		return str->md5();
	}

	public function getSha1(string str)
	{
		return str->sha1();
	}

	public function getNl2br(string str)
	{
		return str->nl2br();
	}

	public function getParsedCsv(string str)
	{
		return str->parseCsv();
	}

	public function getParsedJson(string str, boolean assoc = true)
	{
		return str->parseJson(str, assoc);
	}

	public function getRepeatted(string str, int count)
	{
		return str->repeat(count);
	}

	public function getShuffled(string str)
	{
		return str->shuffle();
	}

	public function getSplited(string str, string del)
	{
		return str->split(del);
	}

	public function getCompare(string left, string right)
	{
		return left->compare(right);
	}

	public function getCompareLocale(string left, string right)
	{
		return left->compareLocale(right);
	}

	public function getReversed(string str)
	{
		return str->rev();
	}

	public function getHtmlSpecialChars(string str)
	{
		return str->htmlSpecialChars();
	}
}