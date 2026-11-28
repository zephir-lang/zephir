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

namespace Extension;

use ArrayObject;
use FilesystemIterator;
use PDO;
use PHPUnit\Framework\TestCase;
use RecursiveDirectoryIterator;
use ReflectionMethod;
use ReflectionProperty;
use Stub\Issue2869;
use Stub\Issue2869Connection;
use Stub\Issue2869Directory;

/**
 * Every value below must match the PHP running the test, which is the PHP
 * that compiled the extension. PHP 8.5 renumbered the `PDO::FETCH_*` flags
 * and PHP 8.1 the `FilesystemIterator` flags, so a C file generated on
 * another version used to carry the wrong numbers.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2869
 *
 * @requires extension pdo
 */
final class Issue2869Test extends TestCase
{
    public function testClassConstantsFollowTheRunningPhp(): void
    {
        $this->assertSame(PDO::FETCH_GROUP, Issue2869::GROUP);
        $this->assertSame(PDO::FETCH_ASSOC | PDO::FETCH_UNIQUE, Issue2869::UNIQUE_ASSOC);
        $this->assertSame(
            [PDO::FETCH_CLASSTYPE, 'late' => PDO::FETCH_PROPS_LATE, 'serialize' => PDO::FETCH_SERIALIZE],
            Issue2869::LIST
        );
        $this->assertSame(RecursiveDirectoryIterator::FOLLOW_SYMLINKS, Issue2869::SYMLINKS);
        $this->assertSame(FilesystemIterator::OTHER_MODE_MASK, Issue2869::MASK);
    }

    public function testPropertyDefaultsFollowTheRunningPhp(): void
    {
        $this->assertSame(PDO::FETCH_SERIALIZE, (new ReflectionProperty(Issue2869::class, 'serialize'))->getDefaultValue());
        $this->assertSame(FilesystemIterator::OTHER_MODE_MASK, (new ReflectionProperty(Issue2869::class, 'mask'))->getDefaultValue());
        $this->assertSame(PDO::FETCH_SERIALIZE, (new Issue2869())->serialize);
        $this->assertSame(FilesystemIterator::OTHER_MODE_MASK, (new Issue2869())->mask);
    }

    public function testParameterDefaultFollowsTheRunningPhp(): void
    {
        $parameter = (new ReflectionMethod(Issue2869::class, 'withDefault'))->getParameters()[0];

        $this->assertSame(PDO::FETCH_GROUP, $parameter->getDefaultValue());
        $this->assertTrue($parameter->isDefaultValueConstant());
        $this->assertSame('PDO::FETCH_GROUP', $parameter->getDefaultValueConstantName());
        $this->assertSame(PDO::FETCH_GROUP, Issue2869::withDefault());
    }

    public function testExpressionsInAMethodBodyFollowTheRunningPhp(): void
    {
        $this->assertSame([PDO::FETCH_GROUP | PDO::FETCH_ASSOC], Issue2869::grouped(PDO::FETCH_ASSOC));
        $this->assertSame(~PDO::FETCH_GROUP, Issue2869::inverted());
        $this->assertSame(PDO::FETCH_UNIQUE, Issue2869::unique());
        $this->assertTrue(Issue2869::isGroup(PDO::FETCH_GROUP | PDO::FETCH_COLUMN));
        $this->assertFalse(Issue2869::isGroup(PDO::FETCH_COLUMN));
        $this->assertSame(RecursiveDirectoryIterator::FOLLOW_SYMLINKS, Issue2869::symlinks());
    }

    public function testOtherConstantsOfPhpClassesAreReadWhenTheCodeRuns(): void
    {
        $this->assertSame(PDO::FETCH_ASSOC, Issue2869::assoc());
        $this->assertSame(ArrayObject::ARRAY_AS_PROPS, Issue2869::props());
    }

    public function testSubclassReadsTheInheritedFlags(): void
    {
        $this->assertSame(PDO::FETCH_UNIQUE, Issue2869Connection::unique());
        $this->assertSame(PDO::FETCH_GROUP, Issue2869Connection::group());
        $this->assertSame(PDO::FETCH_ASSOC, Issue2869Connection::assoc());
        $this->assertSame(FilesystemIterator::FOLLOW_SYMLINKS, Issue2869Directory::symlinks());
        $this->assertSame(FilesystemIterator::OTHER_MODE_MASK, Issue2869Directory::mask());
        $this->assertSame(FilesystemIterator::SKIP_DOTS, Issue2869Directory::skipDots());
    }

    /**
     * The issue's own symptom: PDO rejects a fetch mode built from another
     * PHP's numbers with a ValueError.
     *
     * @requires extension pdo_sqlite
     */
    public function testPdoAcceptsTheFetchModes(): void
    {
        $pdo = new PDO('sqlite::memory:');
        $pdo->exec('CREATE TABLE t (grp TEXT, name TEXT)');
        $pdo->exec("INSERT INTO t VALUES ('a', 'x'), ('a', 'y'), ('b', 'z')");

        $grouped = $pdo->query('SELECT grp, name FROM t')->fetchAll(...Issue2869::grouped(PDO::FETCH_ASSOC));

        $this->assertSame(
            ['a' => [['name' => 'x'], ['name' => 'y']], 'b' => [['name' => 'z']]],
            $grouped
        );
        $this->assertSame(
            ['a' => ['name' => 'y'], 'b' => ['name' => 'z']],
            $pdo->query('SELECT grp, name FROM t')->fetchAll(Issue2869::UNIQUE_ASSOC)
        );
    }
}
