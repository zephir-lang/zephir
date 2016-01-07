<?php
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/
namespace Zephir\Expression\Builder\Statements;

use Zephir\Expression\Builder\AbstractBuilder;

/**
 * StatementsBlock
 *
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
        if ($statements !== null) {
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
     * @return $this
     */
    public function setStatements($statements)
    {
        $this->statements = $statements;
        return $this;
    }

    /**
     * @param $statement
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
