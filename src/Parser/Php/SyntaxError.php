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

namespace Zephir\Parser\Php;

use RuntimeException;

/**
 * Internal control-flow exception used by {@see PhpParser} to unwind to the
 * top-level parse() call, which converts it into the C extension's
 * "return an error node, never throw" contract.
 */
final class SyntaxError extends RuntimeException
{
}
