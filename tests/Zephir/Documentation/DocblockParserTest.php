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

namespace Zephir\Test\Documentation;

use PHPUnit\Framework\TestCase;
use Zephir\Documentation\Annotation;
use Zephir\Documentation\Annotation\Link;
use Zephir\Documentation\Annotation\ReturnAnnotation;
use Zephir\Documentation\Annotation\See;
use Zephir\Documentation\Docblock;
use Zephir\Documentation\DocblockParser;
use Zephir\Os;

final class DocblockParserTest extends TestCase
{
    public function testShouldParseDocblockFromString(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * This file is part of the Zephir.
             *
             * Helper to parse raw docblocks to structured object.
             *
             * @link https://github.com/zephir-lang/zephir - Zephir
             * @see  https://github.com/zephir-lang/zephir/README.md - Zephir readme
             *
             * @param array \$annotation - custom annotation
             *
             * @return Docblock the parsed docblock
             */
            DOC;

        $link = new Link('link', 'https://github.com/zephir-lang/zephir - Zephir');
        $link->getLinkText();

        $see = new See('see', 'https://github.com/zephir-lang/zephir/README.md - Zephir readme');
        $see->getResource();

        $return = new ReturnAnnotation('return', 'Docblock the parsed docblock');
        $return->getReturnType();

        $param = new Annotation('param', 'array $annotation - custom annotation');

        $expected = new Docblock();
        $expected->setSummary('This file is part of the Zephir.');
        $expected->setDescription('Helper to parse raw docblocks to structured object.');
        $expected->addAnnotation($link);
        $expected->addAnnotation($see);
        $expected->addAnnotation($param);
        $expected->addAnnotation($return);

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $this->assertSame($expected->getSummary(), $actual->getSummary());
        $this->assertSame($expected->getDescription(), $actual->getDescription());

        $this->assertEquals($expected->getAnnotationsByType('link'), $actual->getAnnotationsByType('link'));
        $this->assertEquals($expected->getAnnotationsByType('see'), $actual->getAnnotationsByType('see'));
        $this->assertEquals($expected->getAnnotationsByType('param'), $actual->getAnnotationsByType('param'));
        $this->assertEquals($expected->getAnnotationsByType('return'), $actual->getAnnotationsByType('return'));
    }

    /**
     * A PHPStan type-alias whose body is a multi-line array shape must be
     * captured as a single annotation; the newline between `array{` and the
     * following members is inside an unclosed brace and therefore should
     * not terminate annotation parsing.
     */
    public function testShouldParseMultiLinePhpStanTypeAliasAcrossNewlines(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * Component description.
             *
             * @phpstan-type TTemplate array{
             *      main: string,
             *      line: string,
             *      last: string
             *  }
             */
            DOC;

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $annotations = $actual->getAnnotationsByType('phpstan-type');
        $this->assertCount(1, $annotations);
        // Indentation from continuation lines is preserved verbatim; the
        // newline itself collapses to a single space. Assert via
        // whitespace-normalized comparison so the test stays robust against
        // future indentation tweaks in the heredoc above.
        $this->assertSame(
            'TTemplate array{ main: string, line: string, last: string }',
            preg_replace('/\s+/', ' ', $annotations[0]->getString())
        );
    }

    /**
     * Two consecutive multi-line PHPStan type aliases must register as two
     * independent annotations; closing the first `array{...}` must reset
     * bracket-depth tracking so the second alias is not absorbed into the
     * first.
     */
    public function testShouldParseTwoConsecutiveMultiLinePhpStanTypeAliases(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * @phpstan-type TTemplate array{
             *      main: string,
             *      line: string
             *  }
             * @phpstan-type TElement array{
             *      attributes: array<string, string>,
             *      icon: string,
             *      link: string
             *  }
             */
            DOC;

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $annotations = $actual->getAnnotationsByType('phpstan-type');
        $this->assertCount(2, $annotations);
        $this->assertSame(
            'TTemplate array{ main: string, line: string }',
            preg_replace('/\s+/', ' ', $annotations[0]->getString())
        );
        $this->assertSame(
            'TElement array{ attributes: array<string, string>, icon: string, link: string }',
            preg_replace('/\s+/', ' ', $annotations[1]->getString())
        );
    }

    /**
     * Multi-line `@method` signature whose parameter list spans several
     * lines must remain a single annotation; the open `(` keeps the
     * annotation alive across newlines until the matching `)` is seen.
     */
    public function testShouldParseMultiLineMethodSignatureAcrossNewlines(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * @method Result handle(
             *      string \$name,
             *      array \$options,
             *      ?int \$flags = null
             *  )
             */
            DOC;

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $annotations = $actual->getAnnotationsByType('method');
        $this->assertCount(1, $annotations);
        $this->assertSame(
            'Result handle( string $name, array $options, ?int $flags = null )',
            preg_replace('/\s+/', ' ', $annotations[0]->getString())
        );
    }

    /**
     * Multi-line generic type wrapped across several lines must keep the
     * annotation open until the closing `>` brings the depth counter back
     * to zero.
     */
    public function testShouldParseMultiLineGenericTypeAcrossNewlines(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * @param array<
             *      int,
             *      string
             *  > \$rows
             */
            DOC;

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $annotations = $actual->getAnnotationsByType('param');
        $this->assertCount(1, $annotations);
        $this->assertSame(
            'array< int, string > $rows',
            preg_replace('/\s+/', ' ', $annotations[0]->getString())
        );
    }

    /**
     * Regression guard: a plain single-line annotation must still terminate
     * on the first newline even though bracket-depth tracking is now in
     * place. The new logic only suppresses termination when depth > 0.
     */
    public function testShouldStillTerminateSingleLineAnnotationOnNewline(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * @param array \$rows the dataset
             * @return void
             */
            DOC;

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $params = $actual->getAnnotationsByType('param');
        $returns = $actual->getAnnotationsByType('return');

        $this->assertCount(1, $params);
        $this->assertCount(1, $returns);
        $this->assertSame('array $rows the dataset', $params[0]->getString());
        $this->assertSame('void', $returns[0]->getString());
    }

    /**
     * Stray closing brackets (no matching opener) must not push
     * `annotationBracketDepth` below zero. The depth counter is floored
     * at zero, so a `}` outside any open structure leaves termination
     * behavior unchanged for the next annotation.
     */
    public function testShouldHandleStrayClosingBracketsWithoutLockingAnnotation(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Warning: Strings contain different line endings!');
        }

        $test = <<<DOC
            /**
             * @param string \$json closing brace inside text: }
             * @return void
             */
            DOC;

        $parser = new DocblockParser($test);
        $parser->parse();
        $actual = $parser->getParsedDocblock();

        $this->assertCount(1, $actual->getAnnotationsByType('param'));
        $this->assertCount(1, $actual->getAnnotationsByType('return'));
    }
}
