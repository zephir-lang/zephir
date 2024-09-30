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
}
