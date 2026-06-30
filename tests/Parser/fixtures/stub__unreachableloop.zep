namespace Stub;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1170
 */
class UnreachableLoop
{
    public static function toPropertyString(array properties) -> string
    {
        var x, ret, k, v;

        let x = false;
        let ret = "{";
        for k, v in properties {
            if x {
                let ret .= ", ";
            }
            let x = true;
            let ret .= k . ": ";
            if is_array(v) {
                let ret .= "[\"" . implode("\", \"", v) . "\"]";
            } else {
                let ret .= "\"" . v . "\"";
            }
        }
        return ret . "}";
    }
}
