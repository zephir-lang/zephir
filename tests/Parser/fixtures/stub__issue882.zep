/**
 * Dynamic instantiation must respect constructor visibility.
 *
 * @see https://github.com/zephir-lang/zephir/issues/882
 */

namespace Stub;

class Issue882
{
    public static function initClass(string className)
    {
        return new {className}();
    }
}
