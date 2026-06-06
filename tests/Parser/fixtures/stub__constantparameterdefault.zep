
namespace Stub;

/**
 * Follow-up to https://github.com/zephir-lang/zephir/pull/2566
 *
 * A class constant used as a parameter default (`self::CONST`) must be folded
 * into the arg_info default value so that it is readable through Reflection,
 * the same way the property declaration and the method body already fold it.
 */
class ConstantParameterDefault
{
    const DEFAULT_CIPHER = "aes-256-cfb";
    const DEFAULT_ROUNDS = 12;

    protected cipher;
    protected rounds;

    public function __construct(
        string cipher = self::DEFAULT_CIPHER,
        int rounds = self::DEFAULT_ROUNDS
    ) {
        let this->cipher = cipher;
        let this->rounds = rounds;
    }

    public function getCipher() -> string
    {
        return this->cipher;
    }

    public function getRounds() -> int
    {
        return this->rounds;
    }
}
