
namespace Stub\Invokes;

abstract class AbstractProtected
{
    protected text;

    protected function protectedMethod1(string text) -> string
    {
        var ret;
        let ret = text . "1";

        return ret;
    }

    protected function protectedMethod2(string text) -> string
    {
        var ret;
        let ret = text . "2";

        return ret;
    }

    protected function renderArrayElements(string text)
    {
        return call_user_func_array([this, "protectedMethod2"], [text]);
    }

    protected function renderArrayElementsComplex(string text)
    {
        var item;
        string result = "";

        array elements = [
            [
                "protectedMethod1",
                [text]
            ],
            [
                "protectedMethod2",
                [text]
            ]
        ];

        for item in elements {
            let result .= call_user_func_array([this, item[0]], item[1]);
        }

        return result;
    }
}
