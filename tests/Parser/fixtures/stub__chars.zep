
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

    /**
     * Issue #1291: char arithmetic must behave as a byte (uchar),
     * without sign extension when the value exceeds 127.
     */
	public function sumOverflowHex() -> string
	{
        char ch = ' '; // space character ASCII 32
        let ch += 64;
        let ch += 42;
		return ch->toHex();
    }

	public function sumOverflowValue() -> char
	{
        char ch = ' '; // space character ASCII 32
        let ch += 64;
        let ch += 42;
		return ch;
    }

	public function sumCharParamOverflow(char ch) -> char
	{
        let ch += 100;
		return ch;
    }

	public function charUnderflow() -> char
	{
        char ch = 0;
        let ch -= 1;
		return ch;
    }

	public function maxByteValue() -> char
	{
        char ch = 0;
        let ch += 255;
		return ch;
    }

	public function sumUchar() -> uchar
	{
        uchar ch = 'd'; // 100
        let ch += 100;
		return ch;
    }

    /**
     * Issue #1988: char-typed variables used as array literal items
     * must be boxed as their byte (integer) value instead of raising
     * "Unknown char" at compile time.
     */
	public function arrayOfChars() -> array
	{
        char ch1, ch2;
        string name = "peter";
        let ch1 = name[0],
            ch2 = name[1];
		return [ch1, ch2, ch1];
    }
}
