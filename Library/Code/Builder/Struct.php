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

namespace Zephir\Code\Builder;

use Zephir\Exception\InvalidArgumentException;
use Zephir\Exception\RuntimeException;

use function is_string;

/**
 * Represents an internal extension global structure
 */
class Struct
{
    /**
     * Struct name
     *
     * @var string
     */
    protected string $name;

    /**
     * Struct simple name
     *
     * @var string
     */
    protected string $simpleName;

    /**
     * Struct members definition
     *
     * ```c
     * struct Name {
     *     key value;
     * }
     * ```
     *
     * @var array
     */
    protected array $properties = [];

    /**
     * @param string $name
     * @param string $simpleName
     *
     * @throws InvalidArgumentException
     */
    public function __construct(string $name, string $simpleName)
    {
        if (empty($name)) {
            throw new InvalidArgumentException('Struct name must not be empty');
        }

        $this->name = $name;
        $this->simpleName = $simpleName;
    }

    /**
     * @return string
     */
    public function __toString(): string
    {
        $code = 'typedef struct '.$this->name.' { '.PHP_EOL;

        foreach ($this->properties as $name => $type) {
            $code .= sprintf("\t%s %s;%s", $type, $name, PHP_EOL);
        }

        return $code.'} '.substr($this->name, 1).';'.PHP_EOL;
    }

    /**
     * @param string $field
     * @param array  $global
     *
     * @throws InvalidArgumentException
     */
    public function addProperty(string $field, array $global)
    {
        if (!isset($global['type']) || !is_string($global['type'])) {
            throw new InvalidArgumentException('Property type must be string');
        }

        if (isset($this->properties[$field])) {
            throw new InvalidArgumentException('Property was defined more than once');
        }

        $this->properties[$field] = $this->convertToCType($global['type']);
    }

    /**
     * Returns the C code that initializes the extension global.
     *
     * @param string $name
     * @param array  $global
     * @param string $namespace
     *
     * @throws RuntimeException
     * @throws InvalidArgumentException
     *
     * @return string
     */
    public function getCDefault(string $name, array $global, string $namespace): string
    {
        if (!isset($global['default'])) {
            throw new RuntimeException('Field "'.$name.'" does not have a default value');
        }

        switch ($global['type']) {
            case 'boolean':
            case 'bool':
                return '';

            case 'int':
            case 'uint':
            case 'long':
            case 'double':
            case 'hash':
                return "\t".$namespace.'_globals->'.$this->simpleName.'.'.$name.' = '.$global['default'].';';

            default:
                throw new InvalidArgumentException('Unknown global type: '.$global['type']);
        }
    }

    /**
     * Process Globals for phpinfo() page.
     *
     * @see https://docs.zephir-lang.com/latest/en/globals
     *
     * @param string $name      - global-name
     * @param array $global    - global structure (type, default...)
     * @param string $namespace - global namespace
     *
     * @return string
     */
    public function getInitEntry(string $name, array $global, string $namespace): string
    {
        $structName = $this->simpleName.'.'.$name;
        $iniEntry = $global['ini-entry'] ?? [];
        $iniName = $iniEntry['name'] ?? $namespace.'.'.$structName;
        $scope = $iniEntry['scope'] ?? 'PHP_INI_ALL';

        switch ($global['type']) {
            case 'boolean':
            case 'bool':
                return
                    'STD_PHP_INI_BOOLEAN("'.
                    $iniName.
                    '", "'.
                    (int) (true === $global['default']).
                    '", '.
                    $scope.
                    ', OnUpdateBool, '.
                    $structName.
                    ', zend_'.
                    $namespace.
                    '_globals, '.
                    $namespace.'_globals)';
        }

        return '';
    }

    /**
     * Generates the internal c-type according to the php's type.
     *
     * @param string $type
     *
     * @throws InvalidArgumentException
     *
     * @return string
     */
    protected function convertToCType(string $type): string
    {
        switch ($type) {
            case 'boolean':
            case 'bool':
                return 'zend_bool';

            case 'hash':
                return 'HashTable* ';

            case 'int':
            case 'uint':
            case 'long':
            case 'char':
            case 'uchar':
            case 'double':
                return $type;

            default:
                throw new InvalidArgumentException('Unknown global type: '.$type);
        }
    }
}
