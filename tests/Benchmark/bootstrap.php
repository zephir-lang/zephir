<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

/**
 * Bootstrap for the PHPBench suite. Verifies the freshly built stub extension
 * is loaded so subjects don't silently fall back to "method not found" errors
 * masquerading as zero-cost calls.
 */

require __DIR__ . '/../../vendor/autoload.php';

if (!extension_loaded('stub')) {
    fwrite(
        STDERR,
        "The 'stub' extension is not loaded. Build it first:\n"
        . "  php zephir build\n"
        . "Then run benchmarks via:\n"
        . "  php -d extension=ext/modules/stub.so vendor/bin/phpbench run --report=aggregate\n"
    );
    exit(1);
}
