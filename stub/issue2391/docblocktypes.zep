namespace Stub\Issue2391;

/**
 * PHPDoc `@var` types map onto Zephir types, and any other one leaves the
 * accessors untyped instead of failing the build.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2391
 */
class DocblockTypes
{
    /** @var float */
    protected floatValue { get, set };

    /** @var integer */
    protected integerValue { get, set };

    /** @var boolean */
    protected booleanValue { get, set };

    /** @var ?int */
    protected nullableInt { get, set };

    /** @var ?Collaborator */
    protected nullableThing { get, set };

    /** @var string[] */
    protected strings { get, set };

    /** @var array<int, string> */
    protected generic { get, set };

    /** @var Collaborator[] */
    protected things { get, set };

    /** @var numeric */
    protected numericValue { get, set };

    /** @var scalar */
    protected scalarValue { get, set };

    /** @var self */
    protected selfValue { get, set };

    /** @var iterable */
    protected iterableValue { get, set };

    /** @var void */
    protected voidValue { get, set };

    /** @var true */
    protected trueValue { get, set };

    /** @var null */
    protected nullValue { get, set };

    /** @var numeric|null */
    protected numericOrNull { get, set };
}
