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

final class RealType implements TypeInterface
{
    /**
     * @var string
     */
    private $dataType;

    /**
     * @var bool
     */
    private $mandatory;

    /**
     * @var string
     */
    private $type;

    /**
     * Real return type constructor.
     *
     * @param string      $dataType
     * @param bool        $mandatory
     * @param string|null $type
     */
    public function __construct($dataType, $mandatory = false, $type = null)
    {
        $this->dataType = $dataType;
        $this->mandatory = (bool) $mandatory;
        $this->type = $type ?: TypeInterface::TYPE_PARAMETER;
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getDataType()
    {
        return $this->dataType;
    }

    /**
     * {@inheritdoc}
     *
     * Getting values for a real data types is not supported.
     */
    public function getValue()
    {
        return null;
    }

    /**
     * {@inheritdoc}
     *
     * @return mixed
     */
    public function isMandatory()
    {
        return $this->mandatory;
    }

    /**
     * {@inheritdoc}
     *
     * Checking, whether the return type is the collection for a real data types is not supported.
     *
     * @return bool
     */
    public function isCollection()
    {
        return false;
    }

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
    public function isRealType()
    {
        return true;
    }
}
