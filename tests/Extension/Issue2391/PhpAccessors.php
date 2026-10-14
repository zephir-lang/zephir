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

namespace Extension\Issue2391;

use Stub\Issue2391\Collaborator;

/**
 * The PHP oracle: what each `Stub\Issue2391\Accessors` getter means when
 * written by hand.
 */
final class PhpAccessors
{
    protected $flag;
    protected $thing;
    protected $obj;
    protected $maybe;
    protected $key;
    protected $listOrFalse;
    protected $ratio;
    protected $name;
    protected $label;

    public function poke(string $property, $value): self
    {
        $this->{$property} = $value;

        return $this;
    }

    public function getFlag(): bool
    {
        return $this->flag;
    }

    public function getThing(): Collaborator
    {
        return $this->thing;
    }

    public function getObj(): object
    {
        return $this->obj;
    }

    public function getMaybe(): ?Collaborator
    {
        return $this->maybe;
    }

    public function getKey(): int|string
    {
        return $this->key;
    }

    public function getListOrFalse(): array|false
    {
        return $this->listOrFalse;
    }

    public function getRatio(): float
    {
        return $this->ratio;
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function __toString(): string
    {
        return $this->label;
    }

    public function flagByHand(): bool
    {
        return $this->flag;
    }

    public function thingByHand(): Collaborator
    {
        return $this->thing;
    }

    public function maybeByHand(): ?Collaborator
    {
        return $this->maybe;
    }

    public function objByHand(): object
    {
        return $this->obj;
    }
}
