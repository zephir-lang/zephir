<?php

namespace Stubs;

/**
 * Union types (#2613) must survive into the generated stub on class
 * properties and method parameters, mirroring the union return-type
 * handling. Each member is coerced to its PHP name (double->float) and the
 * members are joined with `|`; `null` is a union member, not a `?` shorthand.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2613
 */
class Issue_2613
{

    public int|float $num = 1;

    public int|string|null $note = null;

    public string|\Iterator $tag;

    /**
     * @param int|float $value
     * @return string
     */
    public function scalar(int|float $value): string
    {
    }

    /**
     * @param string|\Iterator $value
     * @return string
     */
    public function classOrScalar(string|\Iterator $value): string
    {
    }
}
