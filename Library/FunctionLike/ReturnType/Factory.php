<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType;

use const Zephir\T_VOID;

final class Factory
{
    /**
     * @param array $ir
     *
     * @return TypeInterface
     */
    public function create(array $ir)
    {
        if (isset($ir['cast'])) {
            return new Cast(
                $ir['cast']['type'],
                $ir['cast']['value'],
                isset($ir['type']) ? $ir['type'] : null,
                isset($ir['cast']['collection']) && $ir['cast']['collection']
            );
        }

        return $this->createReal($ir['data-type'], $ir);
    }

    /**
     * @param array $ir
     *
     * @return TypeInterface
     */
    public function createVoid(array $ir)
    {
        return $this->createReal(T_VOID, $ir);
    }

    /**
     * @param string $type
     * @param array  $ir
     *
     * @return TypeInterface
     */
    public function createReal($type, array $ir)
    {
        return new Real(
            $type,
            isset($ir['mandatory']) && $ir['mandatory'],
            isset($ir['type']) ? $ir['type'] : null
        );
    }
}
