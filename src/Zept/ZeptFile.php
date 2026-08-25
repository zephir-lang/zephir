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

namespace Zephir\Zept;

/**
 * Parsed representation of a `.zept` (Zephir Test) file.
 *
 * A `.zept` file bundles, in the style of PHP's own `.phpt` format, the Zephir
 * implementation under test together with the PHP usage that exercises the
 * compiled extension and the expected program output. See {@see ZeptParser}.
 */
final class ZeptFile
{
    /**
     * @param list<string>              $files  One Zephir source per `--FILE--` section, in order.
     * @param array<string, mixed>|null $config Merged into the project's `config.json`.
     */
    public function __construct(
        public string $test,
        public array $files,
        public string $usage,
        public string $expect,
        public bool $expectIsFormat,
        public ?string $skipif = null,
        public ?string $ini = null,
        public ?array $config = null,
    ) {
    }
}
