
namespace Stub;

/**
 * Issue #2063: concat-assign (.=) must work with object properties.
 */
class Issue2063
{
    private store = "";

    public function add(string value) -> void
    {
        let this->store .= value;
    }

    public function add2(string value) -> void
    {
        let this->store = this->store . value;
    }

    public function addLiteral() -> void
    {
        let this->store .= "ab";
        let this->store .= "cd";
    }

    public function get() -> string
    {
        return this->store;
    }
}
