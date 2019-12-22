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
use Zephir\Documentation\Annotation;
use Zephir\Documentation\Annotation\Link;
use Zephir\Documentation\Annotation\ReturnAnnotation;
use Zephir\Documentation\Annotation\See;
use Zephir\Documentation\Docblock;
use Zephir\Documentation\DocblockParser;

class DocblockParserTest extends TestCase
{
    /** @test */
    public function shouldParseDocblockFromString()
    {
        $test = <<<DOC
/**
 * This file is part of the Zephir.
 *
 * Helper to parse raw docblocks to structured object.
 *
 * @link https://github.com/phalcon/zephir - Zephir
 * @see  https://github.com/phalcon/zephir/README.md - Zephir readme
 *
 * @param array \$annotation - custom annotation
 *
 * @return Docblock the parsed docblock
 */
DOC;

        $link = new Link('link', 'https://github.com/phalcon/zephir - Zephir');
        $link->getLinkText();

        $see = new See('see', 'https://github.com/phalcon/zephir/README.md - Zephir readme');
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
        $this->assertArraySubset($expected->getAnnotationsByType('link'), $actual->getAnnotationsByType('link'));
        $this->assertArraySubset($expected->getAnnotationsByType('see'), $actual->getAnnotationsByType('see'));
        $this->assertArraySubset($expected->getAnnotationsByType('param'), $actual->getAnnotationsByType('param'));
        $this->assertArraySubset($expected->getAnnotationsByType('return'), $actual->getAnnotationsByType('return'));
    }
}
