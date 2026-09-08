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

namespace Zephir\Traits;

use Zephir\CompilationContext;
use Zephir\Variable\Variable;

/**
 * The pair of temps a write-context property fetch needs.
 *
 * PHP's `ZEND_FETCH_OBJ_W` hands the VM an IS_INDIRECT to the property itself,
 * and a by-reference argument needs exactly that: a container it may separate
 * in place, an element it may create, and storage a callee may assign to. The
 * three property expressions all fetch it the same way and differ only in which
 * backend emitter carries it out.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
trait WriteContextSlotTrait
{
    /**
     * @return array{0: Variable, 1: Variable} the slot, then its fallback
     */
    protected function writeContextSlot(CompilationContext $compilationContext): array
    {
        /**
         * The fallback holds an owned value on the paths where the object has
         * no slot to give -- a magic __get, a readonly property -- so the
         * memory frame has to own it.
         */
        $fallback = $compilationContext->symbolTable->getTempVariableForObserve(
            'variable',
            $compilationContext
        );
        $fallback->setDynamicTypes('undefined');

        /**
         * The slot is a live pointer into the object, so it is a double pointer
         * (emitted without an ampersand) and is never observed: releasing the
         * frame would destroy the property it points at.
         */
        $slot = $compilationContext->symbolTable->addTemp('variable', $compilationContext);
        $slot->setIsDoublePointer(true);
        $slot->setDynamicTypes('undefined');
        $slot->setIsWriteContextReference(true);

        $compilationContext->headersManager->add('kernel/object');

        return [$slot, $fallback];
    }
}
