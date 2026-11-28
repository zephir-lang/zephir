namespace Stub;

/**
 * An exception with array defaults is still created by \Exception, which
 * records where it was created.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
class Issue2799Exception extends \Exception
{
    protected errors = [];

    protected codes = [1, 2];

    public function getErrors()
    {
        return this->errors;
    }

    public function getCodes()
    {
        return this->codes;
    }
}
