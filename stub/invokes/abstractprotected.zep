
namespace Stub\Invokes;

abstract class AbstractProtected
{
    protected text;

    protected function protectedMethod(string text) -> string
    {
        var ret;
        let ret = text . "2";

        return ret;
    }

    protected function renderArrayElements(string text)
    {
        return call_user_func_array([this, "protectedMethod"], [text]);
    }
}
