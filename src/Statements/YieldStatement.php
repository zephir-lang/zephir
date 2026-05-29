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

namespace Zephir\Statements;

use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;

/**
 * Statement-level handler for `yield` AST nodes emitted by the parser.
 *
 * The Zephir compiler does not yet generate code for generator methods.
 * PHP's `ZEND_YIELD` opcode runs only inside `ZEND_USER_FUNCTION` op_arrays,
 * and Zephir methods compile to `ZEND_INTERNAL_FUNCTION` (a plain C function
 * pointer) which has no opcode VM frame to suspend. Bridging the two
 * requires synthesizing a PHP source body for generator methods and
 * `zend_compile_string()`-ing it at `MINIT` so the engine treats the body
 * as a user function. That work is tracked separately.
 *
 * This handler exists so that:
 *   - parser-accepted `yield` syntax produces a precise, located diagnostic
 *     instead of the previous noisy "Unsupported statement" fallout,
 *   - the statement dispatch table covers the type, keeping
 *     `StatementFactory::isSupported('yield')` honest.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1849
 */
final class YieldStatement extends StatementAbstract
{
    /**
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        throw new CompilerException(
            "'yield' is parsed but code generation for generator methods is not yet implemented. "
            . 'Place generator logic in a plain PHP file and load it through the extensions '
            . "'extra-classes' / 'extra-sources' configuration as a workaround. "
            . 'See https://github.com/zephir-lang/zephir/issues/1849',
            $this->statement
        );
    }
}
