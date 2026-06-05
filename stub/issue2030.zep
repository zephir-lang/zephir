namespace Stub;

/**
 * Double-quoted string escape sequences must match PHP byte-for-byte.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2030
 */
class Issue2030
{
	public function escapeChar() -> string
	{
		return "\e[0;31m";
	}

	public function unicodeAscii() -> string
	{
		return "\u{41}";
	}

	public function unicode2Byte() -> string
	{
		return "\u{E9}";
	}

	public function unicode3Byte() -> string
	{
		return "\u{20AC}";
	}

	public function unicode4Byte() -> string
	{
		return "\u{1F600}";
	}

	public function hexFollowedByHex() -> string
	{
		return "\x41BC";
	}
}
