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

interface TypeInterface
{
    /**
     * Gets return data type name.
     *
     * @return string
     */
    public function getDataType();

    /**
     * Gets casted return type.
     *
     * @return string|null
     */
    public function getValue();

    /**
     * Gets intermediate type.

     *
     * @return string
     */
    public function getType();

    /**
     * Is return type mandatory.
     *
     * @return mixed
     */
    public function isMandatory();

    /**
     * Is the return value a collection.
     *
     * @return bool
     */
    public function isCollection();

    /**
     * Is the return type a real type or it is just a cast hint.
     *
     * @return bool
     */
    public function isReal();
}
