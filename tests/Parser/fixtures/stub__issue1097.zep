namespace Stub;

/**
 * Regression coverage for https://github.com/zephir-lang/zephir/issues/1097
 *
 * elseif conditions that contain sub-expressions with side effects (such as
 * array element access myvar[0]) were compiled by hoisting the side-effect code
 * before the entire if/elseif chain. This caused a PHP Notice when a scalar was
 * passed, because zephir_array_fetch_long() ran unconditionally even when the
 * first if-branch had already returned.
 *
 * Fix: preamble code emitted during elseif-condition compilation must be placed
 * inside the else branch, not before the outer if, so it only executes when
 * needed.
 */
class Issue1097
{
    /**
     * The canonical case from the bug report: if the first branch is taken
     * (is_string(myvar) is true), the elseif must NOT attempt myvar[0].
     */
    public function testStringOrArrayOfString(var myvar) -> string
    {
        if is_string(myvar) {
            return "string";
        } elseif is_string(myvar[0]) {
            return "array-of-string";
        }

        return "other";
    }

    /**
     * Variant with an else branch to ensure the else path is unaffected.
     */
    public function testWithElse(var myvar) -> string
    {
        if is_string(myvar) {
            return "string";
        } elseif is_array(myvar) {
            return "array";
        } else {
            return "other";
        }
    }

    /**
     * Two elseif branches, each with a sub-expression that generates preamble
     * code. Only the first matching branch must evaluate.
     */
    public function testMultipleElseif(var myvar) -> string
    {
        if is_string(myvar) {
            return "string";
        } elseif is_string(myvar[0]) {
            return "array-of-string";
        } elseif is_int(myvar[0]) {
            return "array-of-int";
        }

        return "other";
    }

    /**
     * elseif with a simple condition (no preamble) must continue to work.
     */
    public function testSimpleElseif(int num) -> string
    {
        if num > 0 {
            return "positive";
        } elseif num == 0 {
            return "zero";
        } else {
            return "negative";
        }
    }
}
