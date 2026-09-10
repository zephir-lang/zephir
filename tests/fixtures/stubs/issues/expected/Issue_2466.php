<?php

namespace Stubs;

use Stubs\Marker as M;

/**
 * PHP attributes (#2466) must reach the generated stub, so an IDE and any
 * reflection-driven tool see what the compiled extension actually declares.
 *
 * The name is printed exactly as written, because the stub reproduces the same
 * namespace and the same `use` statements. Arguments are printed in their
 * reduced form, since an arithmetic argument has no PHP spelling as an AST node.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2466
 */
#[M('class-level', flag: true)]
class Issue_2466
{
    #[M]
    const VERSION = '1.0';

    #[M(6, [1, 'k' => 'v'])]
    public int $counter = 0;

    #[M]
    #[\Deprecated]
    protected $legacy = null;

    /**
     * @param string $name
     * @param string $token
     * @return string
     */
    #[M('1.0')]
    public function run(#[M] string $name, #[\SensitiveParameter] string $token = ''): string
    {
    }
}
