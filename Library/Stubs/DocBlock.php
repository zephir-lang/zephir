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

namespace Zephir\Stubs;

/**
 * @author Gusakov Nikita <dev@nkt.me>
 */
class DocBlock
{
    /**
     * @var string
     */
    protected $indent;
    /**
     * @var string
     */
    protected $description;
    /**
     * @var array
     */
    protected $lines = array();

    /**
     * @param string $source Raw doc-block
     * @param string $indent Indent, 4 spaces by default
     */
    public function __construct($source, $indent = '    ')
    {
        $this->indent = $indent;

        foreach (explode("\n", trim($source, '/')) as $line) {
            $line = trim($line, "\t*\0 ");
            if ('' === $line) {
                continue;
            }

            if (strpos($line, '@') === 0) {
                $this->lines[] = $line;
            } else {
                $line = preg_replace('~([\s\t]+)[*]([\s\t]+)~', '', $line);
                $this->lines[] = array_pop($this->lines) . "\n{$this->indent} * " . $line;
            }
        }
        if (!empty($this->lines) && strpos($this->lines[0], '@') !== 0) {
            $this->description = array_shift($this->lines);
        }
    }

    /**
     * @return string
     */
    public function __toString()
    {
        $doc = '';
        $indent = $this->indent;
        if ($this->description) {
            $doc = $this->description;
        }
        if ($this->lines) {
            $lines = array_map(function ($line) use ($indent) {
                return "$indent * $line";
            }, $this->lines);
            if ($doc !== '') {
                $doc .= "\n$indent *";
            }
            $doc .= "\n" . join("\n", $lines);
        }

        return $doc === '' ? '' : "$indent/**$doc\n$indent */";
    }
}
