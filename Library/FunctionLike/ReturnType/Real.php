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

final class Real implements TypeInterface
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
     * @param int|bool    $mandatory
     * @param string|null $type
     */
    public function __construct($dataType, $mandatory, $type = null)
    {
        $this->dataType = $dataType;
        $this->mandatory = (bool) $mandatory;
        $this->type = $type ?: 'return-type-paramater';
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
     */
    public function getValue()
    {
        return null;
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
     * @return mixed
     */
    public function isMandatory()
    {
        return $this->mandatory;
    }

    /**
     * {@inheritdoc}
     *
     * The real data type is always not a collection.
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
    public function isReal()
    {
        return true;
    }
}
