namespace Stubs;

use Attribute as A;

/**
 * `Attribute` flag values depend on the PHP version (8.5 renumbered them),
 * so a stub must keep the source expression rather than the number the PHP
 * running `zephir stubs` reports (#2738).
 *
 * @see https://github.com/zephir-lang/zephir/issues/2738
 */
#[\Attribute(\Attribute::TARGET_METHOD | \Attribute::IS_REPEATABLE)]
class Issue_2738
{
    const FLAGS = \Attribute::TARGET_ALL & ~(A::TARGET_PARAMETER | 0x1);

    protected int targets = A::TARGET_CLASS;

    #[Marker(self::FLAGS)]
    public function run(int flags = \Attribute::TARGET_FUNCTION) -> int
    {
        return flags;
    }
}
