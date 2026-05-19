<?php

namespace Stubs;

class Issue_2426
{


    /**
     * Exercises issue #2426 — every parameter with `= null` must produce
     * a nullable type hint in the generated stub (`?Type` in the
     * signature, `Type|null` in the @param docblock). Mixes a class-typed
     * cast with the built-in scalar/array types so all elseif branches in
     * Stubs\Generator::buildMethod() are covered.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2426
     * @param \Iterator|null $iter
     * @param string|null $name
     * @param int|null $count
     * @param bool|null $flag
     * @param double|null $ratio
     * @param array|null $items
     */
    public function nullable(?\Iterator $iter = null, ?string $name = null, ?int $count = null, ?bool $flag = null, ?float $ratio = null, ?array $items = null)
    {
    }
}
