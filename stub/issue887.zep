namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/887
 */
class Issue887
{
    public data;

    public function __construct()
    {
        let this->data = [];
    }

    public function add(key, value)
    {
        let this->data[key] = value;
    }

    public function fetch_array()
    {
        return this->data;
    }

    public function fetch_object()
    {
        return (object) this->data;
    }
}
