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

namespace Zephir;

use Zephir\Statements\StatementAbstract;

/**
 * Represents every branch within a method
 */
class Branch
{
    public const TYPE_CONDITIONAL_FALSE = 2;
    public const TYPE_CONDITIONAL_TRUE  = 1;
    public const TYPE_EXTERNAL          = 6;
    public const TYPE_LOOP_CONDITIONAL  = 4;
    public const TYPE_LOOP_INFINITE     = 3;
    public const TYPE_ROOT              = 0;
    public const TYPE_SWITCH            = 5;
    public const TYPE_UNKNOWN           = 7;

    protected int                $level            = -1;
    protected ?Branch            $parentBranch     = null;
    protected ?StatementAbstract $relatedStatement = null;
    protected int                $type             = self::TYPE_ROOT;
    protected ?bool              $unreachable      = null;
    /**
     * @var mixed
     */
    private $uniqueId;

    /**
     * @return int
     */
    public function getLevel(): int
    {
        return $this->level;
    }

    /**
     * Returns the branch's parent.
     *
     * @return Branch|null
     */
    public function getParentBranch(): ?self
    {
        return $this->parentBranch;
    }

    /**
     * @return StatementAbstract|null
     */
    public function getRelatedStatement(): ?StatementAbstract
    {
        return $this->relatedStatement;
    }

    /**
     * Returns the branch type.
     *
     * @return int
     */
    public function getType(): int
    {
        return $this->type;
    }

    /**
     * @return mixed
     */
    public function getUniqueId()
    {
        return $this->uniqueId;
    }

    /**
     * @return bool|null
     */
    public function isUnreachable(): ?bool
    {
        return $this->unreachable;
    }

    /**
     * @param int $level
     */
    public function setLevel(int $level): void
    {
        $this->level = $level;
    }

    /**
     * Set the branch's parent.
     *
     * @param Branch $parentBranch
     */
    public function setParentBranch(self $parentBranch): void
    {
        $this->parentBranch = $parentBranch;
    }

    /**
     * @param StatementAbstract $relatedStatement
     */
    public function setRelatedStatement(StatementAbstract $relatedStatement): void
    {
        $this->relatedStatement = $relatedStatement;
    }

    /**
     * Set the type of branch. One of the TYPE_* constants.
     *
     * @param int $type
     */
    public function setType(int $type): void
    {
        $this->type = $type;
    }

    /**
     * @param $uniqueId
     */
    public function setUniqueId($uniqueId): void
    {
        $this->uniqueId = $uniqueId;
    }

    /**
     * Sets if the branch is unreachable.
     *
     * @param bool|null $unreachable
     */
    public function setUnreachable(?bool $unreachable): void
    {
        $this->unreachable = $unreachable;
    }
}
