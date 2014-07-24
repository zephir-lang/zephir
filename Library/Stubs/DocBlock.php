<?php

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
     * @param int    $indent Indent size, 4 by default
     */
    public function __construct($source, $indent = 4)
    {
        $this->indent = str_repeat(' ', $indent);
        foreach (explode("\n", trim($source, '/')) as $line) {
            if ('' === $line = trim($line, '* ')) {
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
