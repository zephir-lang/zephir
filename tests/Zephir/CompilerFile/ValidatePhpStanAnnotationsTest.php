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

namespace Zephir\Test\CompilerFile;

use PHPUnit\Framework\TestCase;
use Psr\Log\AbstractLogger;
use Zephir\AliasManager;
use Zephir\Class\Constant;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Property;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\FileSystem\FileSystemInterface;

final class ValidatePhpStanAnnotationsTest extends TestCase
{
    public function testEmitsWarningForInvalidShapeOnMethodDocBlock(): void
    {
        $logger = $this->makeCapturingLogger();
        $file   = $this->makeCompilerFile($logger);

        $classDefinition = new Definition('Stub\\Test', 'PhpStanCheck');
        $classDefinition->setAliasManager(new AliasManager());

        $method = new Method(
            $classDefinition,
            ['public'],
            'find',
            null,
            null,
            "/**\n * @phpstan-return array<int string>\n */"
        );
        $classDefinition->setMethod('find', $method);

        $this->setClassDefinition($file, $classDefinition);
        $file->validatePhpStanAnnotations();

        $this->assertCount(1, $logger->records);
        $this->assertStringContainsString('phpstan-return', $logger->records[0]['message']);
    }

    public function testEmitsNoWarningForValidShape(): void
    {
        $logger = $this->makeCapturingLogger();
        $file   = $this->makeCompilerFile($logger);

        $classDefinition = new Definition('Stub\\Test', 'PhpStanCheck');
        $classDefinition->setAliasManager(new AliasManager());

        $method = new Method(
            $classDefinition,
            ['public'],
            'find',
            null,
            null,
            "/**\n * @phpstan-return array<int, string>\n */"
        );
        $classDefinition->setMethod('find', $method);

        $this->setClassDefinition($file, $classDefinition);
        $file->validatePhpStanAnnotations();

        $this->assertSame([], $logger->records);
    }

    public function testWalksClassPropertyConstantAndMethodDocBlocks(): void
    {
        $logger = $this->makeCapturingLogger();
        $file   = $this->makeCompilerFile($logger);

        $classDefinition = new Definition('Stub\\Test', 'PhpStanCheck');
        $classDefinition->setAliasManager(new AliasManager());
        $classDefinition->setDocBlock(
            "/**\n * Class header\n * @phpstan-type Foo array<int string>\n */"
        );

        $property = new Property(
            $classDefinition,
            ['protected'],
            'data',
            ['type' => 'null', 'value' => null],
            "/**\n * @phpstan-var array<int string>\n */"
        );
        $classDefinition->addProperty($property);

        $constant = new Constant(
            'LIMIT',
            ['type' => 'int', 'value' => 10],
            "/**\n * @phpstan-var array<int string>\n */"
        );
        $classDefinition->addConstant($constant);

        $method = new Method(
            $classDefinition,
            ['public'],
            'find',
            null,
            null,
            "/**\n * @phpstan-return array<int string>\n */"
        );
        $classDefinition->setMethod('find', $method);

        $this->setClassDefinition($file, $classDefinition);
        $file->validatePhpStanAnnotations();

        $this->assertCount(4, $logger->records);
    }

    public function testIgnoresNonWhitelistedTags(): void
    {
        $logger = $this->makeCapturingLogger();
        $file   = $this->makeCompilerFile($logger);

        $classDefinition = new Definition('Stub\\Test', 'PhpStanCheck');
        $classDefinition->setAliasManager(new AliasManager());

        $method = new Method(
            $classDefinition,
            ['public'],
            'find',
            null,
            null,
            "/**\n * @phpstan-template T of object\n */"
        );
        $classDefinition->setMethod('find', $method);

        $this->setClassDefinition($file, $classDefinition);
        $file->validatePhpStanAnnotations();

        $this->assertSame([], $logger->records);
    }

    public function testIgnoresPlainPhpDocTags(): void
    {
        $logger = $this->makeCapturingLogger();
        $file   = $this->makeCompilerFile($logger);

        $classDefinition = new Definition('Stub\\Test', 'PhpStanCheck');
        $classDefinition->setAliasManager(new AliasManager());

        $method = new Method(
            $classDefinition,
            ['public'],
            'find',
            null,
            null,
            "/**\n * @param array<int string> \$x\n */"
        );
        $classDefinition->setMethod('find', $method);

        $this->setClassDefinition($file, $classDefinition);
        $file->validatePhpStanAnnotations();

        $this->assertSame([], $logger->records);
    }

    public function testNoOpWhenNoClassDefinition(): void
    {
        $logger = $this->makeCapturingLogger();
        $file   = $this->makeCompilerFile($logger);

        // Do not set a class definition.
        $file->validatePhpStanAnnotations();

        $this->assertSame([], $logger->records);
    }

    private function makeCompilerFile(AbstractLogger $logger): CompilerFile
    {
        $file = new CompilerFile(
            $this->createMock(Config::class),
            new AliasManager(),
            $this->createMock(FileSystemInterface::class),
        );
        $file->setLogger($logger);

        return $file;
    }

    private function makeCapturingLogger(): AbstractLogger
    {
        return new class extends AbstractLogger {
            public array $records = [];

            public function log($level, $message, array $context = []): void
            {
                $this->records[] = ['level' => $level, 'message' => (string) $message];
            }
        };
    }

    private function setClassDefinition(CompilerFile $file, Definition $definition): void
    {
        $reflection = new \ReflectionClass(CompilerFile::class);
        $property   = $reflection->getProperty('classDefinition');
        $property->setAccessible(true);
        $property->setValue($file, $definition);
    }
}
