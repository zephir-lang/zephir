
namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1134
 */
class Issue1134
{
    private data;

    public function __construct(array config = [])
    {
        let this->data = config;
    }

    public function arrayWithFilledDefaultValue(array someDefaultData = ["key": "val"])
    {
        return someDefaultData;
    }
}
