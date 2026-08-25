
namespace Stub;

/**
 * Expression initializers in declaration position.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2061
 */
final class Issue2061
{
	/** The declaration from the issue report */
	const INT8_MIN = -0x7f - 1;
	const INT8_MAX = 0x7f;
	const INT16_MIN = -0x7fff - 1;
	const INT16_MAX = 0x7fff;

	const KB = 1024;
	const MB = 1024 * 1024;
	const MASK = 0xff << 8 | 0x0f;
	const INVERTED = ~0;
	const RATIO = (1 + 4) / 2;
	const REMAINDER = 7 % 3;

	const NAME = "issue" . "2061";
	const PATH = self::NAME . "/limits";

	const DOUBLED = self::KB * 2;
	const FROM_ENV = PHP_INT_SIZE * 8;

	const FLAG = 1 < 2;
	const NEGATED = !false;
	const WORD = PHP_INT_SIZE == 8 ? 64 : 32;
	const FALLBACK = 0 ?: 42;

	const NUMBERS = [1 + 1, 2 * 2, "sum": 3 + 4];

	public size = 1024 * 8 { get };
	protected mask = 0xff << 8 { get };
	public int total = 2 + 3 { get };
	public label = "a" . "/b" { get };
}
