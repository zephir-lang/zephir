namespace Stub;

/**
 * `var_export()` takes any expression and honors its `return` argument as
 * PHP does.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2774
 */
class Issue2774
{
    public function exportCast(var flag) -> string
    {
        return var_export((bool) flag, true);
    }

    public function exportNative(var flag) -> string
    {
        bool native;

        let native = (bool) flag;

        return var_export(native, true);
    }

    public function exportInt(int value)
    {
        return var_export(value, true);
    }

    public function exportDouble(double value)
    {
        return var_export(value, true);
    }

    public function exportSum(int a, int b)
    {
        return var_export(a + b, true);
    }

    public function exportLiteral()
    {
        return var_export(5, true);
    }

    public function exportShadowedLiteral(var value)
    {
        return var_export("value", true);
    }

    public function dumpShadowedLiteral(var value)
    {
        var_dump("value");
    }

    public function exportVar(var value)
    {
        return var_export(value, true);
    }

    public function exportArray(array value)
    {
        return var_export(value, true);
    }

    public function exportString(string value)
    {
        return var_export(value, true);
    }

    public function exportConcat(var value)
    {
        return "<" . var_export(value, true) . ">";
    }

    public function printReturned(var value)
    {
        return var_export(value);
    }

    public function printFalse(var value)
    {
        return var_export(value, false);
    }

    public function printStatement(var value)
    {
        var_export(value, false);
    }

    public function discarded(var value)
    {
        var_export(value, true);
    }

    public function exportFlag(var value, var flag)
    {
        return var_export(value, flag);
    }

    public function echoExport(var value)
    {
        echo var_export(value, true);
    }
}
