<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\ClassDefinition;

class SourceFile extends ClassFile
{
    /**
     * @var ClassDefinition
     */
    protected $class;

    public function getTemplateName()
    {
        return 'source.phtml';
    }

    public function getOutputFile()
    {
        return \Zephir\Documentation::sourceUrl($this->class);
    }
}
