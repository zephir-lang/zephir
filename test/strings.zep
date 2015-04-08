namespace Test;

class Strings
{
	public function testTrim(var str)
	{
		return trim(str);
	}

	public function testRtrim(var str)
	{
		return rtrim(str);
	}

	public function testLtrim(var str)
	{
		return ltrim(str);
	}

	public function testTrim2Params(var str, var charlist)
	{
		return trim(str, charlist);
	}

	public function testRtrim2Params(var str, var charlist)
	{
		return rtrim(str, charlist);
	}

	public function testLtrim2Params(var str, var charlist)
	{
		return ltrim(str, charlist);
	}

	public function testImplode(var glue, var pieces)
	{
		return implode(glue, pieces);
	}

	public function testStrpos(var haystack, var needle)
	{
		return strpos(haystack, needle);
	}

	public function testStrposOffset(var haystack, var needle, int offset)
	{
		return strpos(haystack, needle, offset);
	}

	public function testExplode(var delimiter, var str)
	{
		return explode(delimiter, str);
	}

	public function testExplodeStr(var str)
	{
		return explode(",", str);
	}

	public function testExplodeLimit(var str, int limit)
	{
		return explode(",", str, limit);
	}

	public function testSubstr(var str, int from, int len)
	{
		return substr(str, from, len);
	}

	public function testSubstr2(var str, int from)
	{
		return substr(str, from);
	}

	public function testSubstr3(var str)
	{
		return substr(str, -1);
	}

	public function testSubstr4(var str)
	{
		return substr(str, 0, -1);
	}

	public function testAddslashes(var str)
	{
		return addslashes(str);
	}

	public function testStripslashes(var str)
	{
		return stripslashes(str);
	}

    public function testHardcodedMultilineString()
    {
        return "
            Hello world
        ";
    }

    public function testEchoMultilineString()
    {
        echo "
            Hello world
        ";
    }

    public function testTrimMultilineString()
    {
        return trim("
            Hello world
        ");
    }

    public function testWellEscapedMultilineString()
    {
        return trim("
            \\\"\}\$hello\$\\\"\'
        ");
    }

	public function testInternedString1()
	{
		return ~"hello";
	}

	public function testInternedString2()
	{
		return ~" hello "->trim();
	}
}
