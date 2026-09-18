namespace Stub\Attributes;

use Stub\Attributes\Marker as M;

/**
 * Every attribute target PHP allows on a class member, so the compiled
 * extension can be checked with the same Reflection API a hand-written PHP
 * class would be (#2466).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
#[M("class-level", flag: true)]
#[M("second")]
class Demo
{
    #[M("on-a-constant")]
    const VERSION = "1.0";

    #[M("on-a-property", extra: [1, "k": "v"], number: 2 * 3)]
    public int counter = 0;

    #[M(self::VERSION)]
    protected legacy = null;

    #[M(Marker::class)]
    public function getValue(#[M("first")] string name, #[\SensitiveParameter] string token = "") -> string
    {
        return name . token;
    }

    public function plain() -> int
    {
        return 1;
    }
}
