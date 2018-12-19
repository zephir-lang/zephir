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

final class CastHint implements TypeInterface
{
    /**
     * @var string
     */
    private $dataType;

    /**
     * @var string
     */
    private $value;

    /**
     * @var string
     */
    private $type;

    /**
     * @var bool
     */
    private $collection;

    /**
     * Real return type constructor.
     *
     * @param string      $dataType
     * @param string|null $value
     * @param string|null $type
     * @param int|bool    $collection
     */
    public function __construct($dataType, $value, $type = null, $collection = false)
    {
        $this->dataType = $dataType;
        $this->value = $value;
        $this->type = $type ?: TypeInterface::TYPE_PARAMETER;
        $this->collection = (bool) $collection;
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
     * @return string|null
     */
    public function getValue()
    {
        return $this->value;
    }

    /**
     * {@inheritdoc}
     *
     * The cast hint is always not mandatory.
     *
     * @return mixed
     */
    public function isMandatory()
    {
        return false;
    }

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
    public function isCollection()
    {
        return $this->collection;
    }

    /**
     * {@inheritdoc}
     *
     * @return bool
     */
    public function isRealType()
    {
        return false;
    }
}
