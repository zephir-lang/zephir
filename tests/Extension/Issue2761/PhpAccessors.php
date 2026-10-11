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

namespace Extension\Issue2761;

use Stub\Issue2761\Collaborator;

/**
 * The PHP oracle: what each shortcut means when written by hand.
 */
final class PhpAccessors
{
    protected string $name = '';
    protected Collaborator $thing;
    protected ?Collaborator $maybe = null;
    protected ?int $count = null;
    protected int $pending;
    /** @var Collaborator */
    protected $docQualified;
    /** @var Collaborator */
    protected $docRelative;
    /** @var Collaborator|null */
    protected $docNullable;
    protected int $declaredWins = 0;

    public function getName(): string
    {
        return $this->name;
    }

    public function setName(string $name)
    {
        $this->name = $name;

        return $this;
    }

    public function getThing(): Collaborator
    {
        return $this->thing;
    }

    public function setThing(Collaborator $thing)
    {
        $this->thing = $thing;

        return $this;
    }

    public function getMaybe(): ?Collaborator
    {
        return $this->maybe;
    }

    public function setMaybe(?Collaborator $maybe)
    {
        $this->maybe = $maybe;

        return $this;
    }

    public function getCount(): ?int
    {
        return $this->count;
    }

    public function setCount(?int $count)
    {
        $this->count = $count;

        return $this;
    }

    public function getPending(): int
    {
        return $this->pending;
    }

    public function getDocQualified(): Collaborator
    {
        return $this->docQualified;
    }

    public function setDocQualified(Collaborator $docQualified)
    {
        $this->docQualified = $docQualified;

        return $this;
    }

    public function getDocRelative(): Collaborator
    {
        return $this->docRelative;
    }

    public function getDocNullable(): ?Collaborator
    {
        return $this->docNullable;
    }

    /**
     * A `@var` union keeps the setter untyped.
     */
    public function setDocNullable($docNullable)
    {
        $this->docNullable = $docNullable;

        return $this;
    }

    public function getDeclaredWins(): int
    {
        return $this->declaredWins;
    }

    public function setDeclaredWins(int $declaredWins)
    {
        $this->declaredWins = $declaredWins;

        return $this;
    }
}
