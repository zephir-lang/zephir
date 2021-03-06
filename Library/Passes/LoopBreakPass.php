<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Passes;

use Zephir\StatementsBlock;

/**
 * LoopBreakPass.
 *
 * This pass checks if the statement block related to a 'loop' loop has at least a 'break'
 * statement disallowing full infinite loops
 */
class LoopBreakPass
{
    protected $hasBreak = false;

    /**
     * Do the compilation pass.
     *
     * @param StatementsBlock $block
     */
    public function pass(StatementsBlock $block)
    {
        $this->passStatementBlock($block->getStatements());
    }

    public function passStatementBlock(array $statements)
    {
        foreach ($statements as $statement) {
            switch ($statement['type']) {
                case 'if':
                    if (isset($statement['statements'])) {
                        $this->passStatementBlock($statement['statements']);
                    }
                    if (isset($statement['else_statements'])) {
                        $this->passStatementBlock($statement['else_statements']);
                    }
                    break;

                case 'break':
                case 'return':
                case 'throw':
                    $this->hasBreak = true;
                    break;
            }
        }
    }

    public function hasBreak()
    {
        return $this->hasBreak;
    }
}
