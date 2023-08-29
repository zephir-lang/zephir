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

final class CastHint implements TypeInterface
{
    public function __construct(
        private string $dataType,
        private ?string $value = null,
        private ?string $type = null,
        private bool $collection = false
    ) {
        $this->type = $type ?: TypeInterface::TYPE_PARAMETER;
    }

    public function getType(): string
    {
        return $this->type;
    }

    public function getDataType(): string
    {
        return $this->dataType;
    }

    public function getValue(): ?string
    {
        return $this->value;
    }

    public function isMandatory(): bool
    {
        return false;
    }

    public function isCollection(): bool
    {
        return $this->collection;
    }

    public function isRealType(): bool
    {
        return false;
    }
}
