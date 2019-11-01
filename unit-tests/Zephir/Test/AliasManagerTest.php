<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\AliasManager;

class AliasManagerTest extends TestCase
{
    /** @var \Zephir\AliasManager */
    private $testAliasMgr;

    public function setUp()
    {
        $this->testAliasMgr = new AliasManager();
    }

    public function aliasDataProvider(): array
    {
        return [
            'with alias' => [
                // Actual
                [
                    'name' => 'Bug\\Events\\ManagerInterface',
                    'alias' => 'EventsManagerInterface',
                ],
                // Expected
                [
                    'EventsManagerInterface' => 'Bug\\Events\\ManagerInterface',
                ],
            ],
            'with alias and namespace from root' => [
                // Actual
                [
                    'name' => '\\Bug\\Events\\ManagerInterface',
                    'alias' => 'EventsManagerInterface',
                ],
                // Expected
                [
                    'EventsManagerInterface' => '\\Bug\\Events\\ManagerInterface',
                ],
            ],
            'without explicit alias' => [
                // Actual
                [
                    'name' => 'Throwable',
                    'alias' => 'Throwable',
                ],
                // Expected
                [
                    'Throwable' => 'Throwable',
                ],
            ],
        ];
    }

    /**
     * @test
     * @dataProvider aliasDataProvider
     */
    public function shouldProperAddStatements(array $useStatements, array $expected)
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $alias = $useStatements['alias'];
        $namespace = $useStatements['name'];

        $this->assertTrue($this->testAliasMgr->isAlias($alias));
        $this->assertSame($this->testAliasMgr->getAliases(), $expected);
        $this->assertSame($this->testAliasMgr->getAlias($alias), $namespace);
    }

    public function statementDataProvider(): array
    {
        return [
            'with aliased statement' => [
                [
                    'name' => 'Bug\\Events\\ManagerInterface',
                    'alias' => 'EventsManagerInterface',
                ], true,
            ],
            'without aliased statement' => [
                [
                    'name' => 'Bug\\Events\\ManagerInterface',
                    'alias' => 'ManagerInterface',
                ], false,
            ],
        ];
    }

    /**
     * @test
     * @dataProvider statementDataProvider
     */
    public function shouldCheckAliasedStatement(array $useStatements, bool $expected)
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $alias = $useStatements['alias'];
        $namespace = $useStatements['name'];

        $this->assertSame($this->testAliasMgr->isUseStatementAliased($alias), $expected);
        $this->assertSame($this->testAliasMgr->isNamespaceAliased($namespace), $expected);
    }
}
