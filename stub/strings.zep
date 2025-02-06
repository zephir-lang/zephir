
namespace Stub;

class Strings
{
	public function camelize(string str, var delimiter = null)
	{
		return camelize(str, delimiter);
	}

	public function uncamelize(string str, var delimiter = null)
	{
		return uncamelize(str, delimiter);
	}

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

	public function testStripcslashes(var str) -> string
	{
		return stripcslashes(str);
	}

	public function testHashEquals(var str1, var str2) -> bool
	{
		return hash_equals(str1, str2);
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

	public function strToHex(string value) -> string
	{
		int i = 0;
		string ret = "";
		while (i < value->length()) {
			let ret .= dechex(ord(value[i]));
			let i++;
		}
		return ret;
	}

	public function issue1267(value)
	{
		var x;
		let value = str_replace(["\\", "\"", "'"], "", value);
		let value = filter_var(value, FILTER_UNSAFE_RAW);
		let x = trim(stripslashes(strip_tags(value)));
		return trim(stripcslashes(strip_tags(value)));
	}

	public function issue2186(string val = null) -> bool
	{
	    return val === null;
	}

	public function issue2186SegFault(string val = null) -> bool
	{
	    if val {
	        return true;
	    }

	    return false;
	}

	public function issue2186SegFaultCall(string val = null) -> string
	{
	    return this->issue2186Child1(val);
	}

	protected function issue2186Child1(string val = null) -> string
	{
	    return this->issue2186Child2(val);
	}

    protected function issue2186Child2(string val = null) -> string
    {
        var output = "";

        if val {
            let output = val . " all ok";
        }

        return output;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2234
     */
    public function issue2234Strict(string! val = null) -> bool
    {
        return val === null;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2234
     */
    public function issue2234StrictParent(string! val = null)
    {
        return this->issue2234StrictChild(val);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2234
     */
    protected function issue2234StrictChild(string! val = null)
    {
        return val;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1932
     */
    public function nullableStringReturnType(string! val = null) -> string | null
    {
        if (val !== null) {
            return val;
        }

        return null;
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/2299
     */
    public function issue2299NullableStringCondition(string roleName = null) -> string | null
    {
        if null !== roleName {
            return roleName;
        }

        return null;
    }
}
