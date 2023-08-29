<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType;

interface TypeInterface
{
    const TYPE_PARAMETER = 'return-type-parameter';
    const TYPE_ANNOTATION = 'return-type-annotation';

    /**
     * Gets the intermediate type.
     *
     * In most cases this will be a "return-type-parameter"
     * string except for some rare cases.
     *
     * @return string
     */
    public function getType();

    /**
     * Gets return data type name.
     *
     * @return string
     */
    public function getDataType();

    /**
     * Gets cast return type.
     *
     * @return string|null
     */
    public function getValue();

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
    public function isRealType();
}
