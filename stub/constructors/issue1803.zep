
namespace Stub\Constructors;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1803
 */
class Issue1803
{
    public value = "Original value";

    public function Issue1803()
    {
        let this->value = "Value that won't be updated on class init";
    }
}
