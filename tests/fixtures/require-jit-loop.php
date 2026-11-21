<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * A Zephir `require` in a hot loop. Run with the tracing JIT: the loop becomes
 * a trace, and a require that loses EG(jit_trace_num) makes its next side exit
 * crash or resume the wrong code.
 */
$requires = new \Stub\Requires();
$file = __DIR__ . '/require-me-1.php';
$iterations = (int) ($argv[1] ?? 2000);

for ($i = 0; $i < $iterations; $i++) {
    if ($requires->requireExternal1($file) !== [1, 2, 3]) {
        echo "unexpected result at iteration {$i}";
        exit(2);
    }
}

echo 'ok';
