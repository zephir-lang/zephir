<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Expression\Builder\Statements;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * Allows to manually build a statements block AST node
 */
class StatementsBlock extends AbstractBuilder
{
    private $statements;

    /**
     * @param array|null $statements
     */
    public function __construct(array $statements = null)
    {
        if (null !== $statements) {
            $this->setStatements($statements);
        }
    }

    /**
     * @return array
     */
    public function getStatements()
    {
        return $this->statements;
    }

    /**
     * @param array $statements
     *
     * @return $this
     */
    public function setStatements($statements)
    {
        $this->statements = $statements;

        return $this;
    }

    /**
     * @param $statement
     *
     * @return $this
     */
    public function addStatement($statement)
    {
        $this->statements[] = $statement;

        return $this;
    }

    /**
     * @return array
     */
    public function build()
    {
        return $this->resolve($this->preBuild());
    }

    /**
     * @return array
     */
    protected function preBuild()
    {
        return $this->getStatements();
    }
}
