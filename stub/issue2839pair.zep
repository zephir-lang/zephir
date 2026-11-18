namespace Stub;

/**
 * Keeps the constructor arguments in its storage, so a test can read back
 * which value reached which parameter.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2839
 */
class Issue2839Pair extends \ArrayObject
{
    public function __construct(var a, var b = null)
    {
        parent::__construct([a, b]);
    }
}
