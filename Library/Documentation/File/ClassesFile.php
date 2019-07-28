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
use Zephir\Documentation\AbstractFile;

class ClassesFile extends AbstractFile
{
    /**
     * @var ClassDefinition
     */
    protected $classes;

    public function __construct($config, $classList)
    {
        $this->classes = $classList;
    }

    public function getTemplateName()
    {
        return 'classes.phtml';
    }

    public function getData()
    {
        return [
            'classes' => $this->classes,
        ];
    }

    public function getOutputFile()
    {
        return 'classes.html';
    }
}
