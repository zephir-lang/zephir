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

use Zephir\Types;

final class Factory
{
    /**
     * @param array $ir
     *
     * @return TypeInterface
     */
    public function create(array $ir)
    {
        if (!isset($ir['cast'])) {
            return $this->createRealType(
                isset($ir['data-type']) ? $ir['data-type'] : Types::T_UNDEFINED,
                $ir
            );
        }

        if (isset($ir['collection']) && $ir['collection']) {
            $dataType = Types::T_ARRAY;
        } else {
            $dataType = isset($ir['cast']['type']) ? $ir['cast']['type'] : Types::T_UNDEFINED;
        }

        return $this->createCastHint($dataType, $ir);
    }

    /**
     * @param array $ir
     *
     * @return TypeInterface
     */
    public function createVoid(array $ir)
    {
        return $this->createRealType(Types::T_VOID, $ir);
    }

    /**
     * @param string $dataType
     * @param array  $ir
     *
     * @return TypeInterface|RealType
     */
    public function createRealType($dataType, array $ir)
    {
        return new RealType(
            $dataType,
            isset($ir['mandatory']) && $ir['mandatory'],
            isset($ir['type']) ? $ir['type'] : null
        );
    }

    /**
     * @param string $dataType
     * @param array  $ir
     *
     * @return TypeInterface|CastHint
     */
    public function createCastHint($dataType, array $ir)
    {
        return new CastHint(
            $dataType,
            isset($ir['cast']['value']) ? $ir['cast']['value'] : null,
            isset($ir['type']) ? $ir['type'] : null,
            isset($ir['collection']) && $ir['collection']
        );
    }
}
