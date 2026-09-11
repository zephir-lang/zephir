namespace Stub\Attributes;

/**
 * A Zephir class used as a PHP attribute (#2466).
 *
 * `#[\Attribute(...)]` on an internal class is all `newInstance()` needs: it
 * reads the marker straight off `ce->attributes` and then calls the constructor
 * with the stored arguments, so the properties below are what reflection
 * observes. `\Attribute::TARGET_ALL` is a class constant of an internal class
 * and is folded at compile time, exactly as php-src's own stubs do — which
 * means the emitted literal follows the building PHP, where 8.5 renumbered
 * these flags.
 *
 * Every constructor parameter is typed on purpose: only a typed parameter gets
 * its default into arg_info, and without that PHP cannot skip one to honour a
 * named argument.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
#[\Attribute(\Attribute::TARGET_ALL | \Attribute::IS_REPEATABLE)]
class Marker
{
    public string text = "";
    public bool flag = false;
    public array extra = [];
    public int number = 0;

    public function __construct(string text = "", bool flag = false, array extra = [], int number = 0)
    {
        let this->text = text;
        let this->flag = flag;
        let this->extra = extra;
        let this->number = number;
    }
}
