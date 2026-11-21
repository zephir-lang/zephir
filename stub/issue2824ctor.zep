namespace Stub;

/**
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
class Issue2824Ctor extends \ArrayObject
{
    public function __construct(var a = "x", var b = "y")
    {
        parent::__construct([a, b]);
    }
}
