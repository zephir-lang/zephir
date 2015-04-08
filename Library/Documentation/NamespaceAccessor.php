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

namespace Zephir\Documentation;

use Zephir\Documentation\NamespaceHelper;

class NamespaceAccessor
{

    /**
     *
     * @var \Zephir\CompilerFile[]
     */
    protected $classes;

    protected $namespaceTree;
    
    protected $byNamespace;

    /**
     *
     * @param \Zephir\CompilerFile[] $files
     */
    public function __construct($files)
    {
        $this->classes = $files;
    }

    /**
     *
     * @return NamespaceHelper[]
     */
    public function getByNamespace()
    {
        return $this->byNamespace;
    }

    public function getNamespaceTree()
    {
        return $this->namespaceTree;
    }

    public function build()
    {
        $byNamespace = array();
        $tree = array();

        foreach ($this->classes as $className => $class) {
            $ns = explode("\\", $class->getClassDefinition()->getNamespace());
            $actualStr = "";
            foreach ($ns as $n) {
                if (strlen($actualStr) > 0) {
                    $previous = $byNamespace[$actualStr];
                    $actualStr.= "\\";
                    $isRoot = false;
                } else {
                    $previous = null;
                    $isRoot = false;
                    $isRoot = true;
                }
                $actualStr.= $n;

                if (!isset($byNamespace[$actualStr])) {
                    $byNamespace[$actualStr] = new NamespaceHelper($actualStr);
                }

                $nh = $byNamespace[$actualStr];

                if ($previous) {
                    $previous->addNamespace($nh);
                }

                if ($isRoot) {
                    if (!isset($tree[$n])) {
                        $tree[$n] = $nh;
                    }
                }
            }

            $nh->addClass($class);
        }

        $this->byNamespace = $byNamespace;
        $this->namespaceTree = $tree;
    }
}
