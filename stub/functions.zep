
namespace Stub;

class Functions
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/658
     */
    public function filterVar1() -> bool
    {
        var ret;
        let ret = "0";

        return false === filter_var(ret, FILTER_VALIDATE_FLOAT, 20480);
    }

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/658
     */
    public function filterVar2() -> bool
    {
        var ret;
        let ret = "0";

        return false == filter_var(ret, FILTER_VALIDATE_FLOAT, 20480);
    }
}
