
/**
 * Chars specific tests
 */

namespace Stub;

class Chars
{
	public function sumChars1() -> char
	{
        char ch = 'A';
        char chlower;
        let chlower = ch + 32;
		return chlower;
    }

	public function sumChars2(char ch) -> char
	{
        char chlower;
        let chlower = ch + 32;
		return chlower;
    }

	public function diffChars1() -> char
	{
        char ch = 'a';
        char chlower;
        let chlower = ch - 32;
		return chlower;
    }

	public function diffChars2(char ch) -> char
	{
        char chlower;
        let chlower = ch - 32;
		return chlower;
    }
}
