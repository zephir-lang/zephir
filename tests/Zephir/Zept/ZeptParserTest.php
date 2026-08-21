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

namespace Zephir\Test\Zept;

use PHPUnit\Framework\TestCase;
use Zephir\Zept\ZeptParseException;
use Zephir\Zept\ZeptParser;

final class ZeptParserTest extends TestCase
{
    private ZeptParser $parser;

    protected function setUp(): void
    {
        $this->parser = new ZeptParser();
    }

    public function testParsesMinimalValidFile(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            Greeter returns a greeting
            --FILE--
            namespace Zept;

            class Greeter
            {
                public function hi() -> string
                {
                    return "hi";
                }
            }
            --USAGE--
            echo (new Zept\Greeter())->hi();
            --EXPECT--
            hi
            ZEPT;

        $zept = $this->parser->parse($raw, 'greeter.zept');

        $this->assertSame('Greeter returns a greeting', $zept->test);
        $this->assertCount(1, $zept->files);
        $this->assertStringContainsString('class Greeter', $zept->files[0]);
        $this->assertStringStartsWith('namespace Zept;', $zept->files[0]);
        $this->assertSame('echo (new Zept\Greeter())->hi();', $zept->usage);
        $this->assertSame('hi', $zept->expect);
        $this->assertFalse($zept->expectIsFormat);
        $this->assertNull($zept->skipif);
    }

    public function testThrowsWhenUsageSectionMissing(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            no usage section
            --FILE--
            namespace Zept; class A {}
            --EXPECT--
            x
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('USAGE');

        $this->parser->parse($raw, 'broken.zept');
    }

    public function testThrowsWhenNoExpectSection(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            no expectation
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('--EXPECT-- or --EXPECTF--');

        $this->parser->parse($raw, 'broken.zept');
    }

    public function testThrowsWhenBothExpectSectionsPresent(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            conflicting expectations
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            --EXPECTF--
            %s
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('both --EXPECT-- and --EXPECTF--');

        $this->parser->parse($raw, 'broken.zept');
    }

    public function testThrowsOnUnknownSection(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            unknown section present
            --FILE--
            namespace Zept; class A {}
            --BOGUS--
            whatever
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('BOGUS');

        $this->parser->parse($raw, 'broken.zept');
    }

    public function testThrowsOnDuplicateNonRepeatableSection(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            first description
            --TEST--
            second description
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('TEST');

        $this->parser->parse($raw, 'broken.zept');
    }

    public function testCollectsMultipleFileSectionsInOrder(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            two classes
            --FILE--
            namespace Zept; class First {}
            --FILE--
            namespace Zept; class Second {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $zept = $this->parser->parse($raw, 'multi.zept');

        $this->assertCount(2, $zept->files);
        $this->assertStringContainsString('class First', $zept->files[0]);
        $this->assertStringContainsString('class Second', $zept->files[1]);
    }

    public function testExpectFSetsFormatFlag(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            format expectation
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECTF--
            object(%s)#%d
            ZEPT;

        $zept = $this->parser->parse($raw, 'fmt.zept');

        $this->assertTrue($zept->expectIsFormat);
        $this->assertSame('object(%s)#%d', $zept->expect);
    }

    public function testToleratesTrailingWhitespaceAfterMarkers(): void
    {
        $raw = "--TEST--\ntrailing ws\n--FILE--  \nnamespace Zept; class A {}\n--USAGE--\t\necho 1;\n--EXPECT--\n1\n";

        $zept = $this->parser->parse($raw, 'ws.zept');

        $this->assertCount(1, $zept->files);
        $this->assertSame('echo 1;', $zept->usage);
    }

    public function testCapturesSkipifSection(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            skippable
            --SKIPIF--
            <?php if (PHP_INT_SIZE < 8) echo "skip 64-bit only";
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $zept = $this->parser->parse($raw, 'skip.zept');

        $this->assertSame('<?php if (PHP_INT_SIZE < 8) echo "skip 64-bit only";', $zept->skipif);
    }

    public function testConfigSectionIsDecodedAsAnArray(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            non-default compiler configuration
            --CONFIG--
            {"optimizations": {"internal-call-transformation": true}}
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $zept = $this->parser->parse($raw, 'config.zept');

        $this->assertSame(
            ['optimizations' => ['internal-call-transformation' => true]],
            $zept->config
        );
    }

    public function testConfigIsNullWhenSectionAbsent(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            no config section
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $this->assertNull($this->parser->parse($raw, 'noconfig.zept')->config);
    }

    public function testThrowsOnMalformedConfigJson(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            broken config
            --CONFIG--
            {"optimizations": }
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('invalid JSON in --CONFIG-- section');

        $this->parser->parse($raw, 'broken.zept');
    }

    public function testThrowsWhenConfigIsNotAJsonObject(): void
    {
        $raw = <<<'ZEPT'
            --TEST--
            config is a list
            --CONFIG--
            ["internal-call-transformation"]
            --FILE--
            namespace Zept; class A {}
            --USAGE--
            echo "x";
            --EXPECT--
            x
            ZEPT;

        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('--CONFIG-- section must be a JSON object');

        $this->parser->parse($raw, 'list.zept');
    }
}
