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

/**
 * Represents an internal extension global structure
 */
class Struct
{
    /**
     * Struct members definition
     *
     * ```c
     * struct Name {
     *     key value;
     * }
     * ```
     */
    protected array $properties = [];

    public function __construct(protected string $name, protected string $simpleName)
    {
        if (empty($name)) {
            throw new InvalidArgumentException('Struct name must not be empty');
        }
    }

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
     * @param string $type
     *
     * @throws InvalidArgumentException
     */
    public function addProperty(string $field, string $type): void
    {
        if (isset($this->properties[$field])) {
            throw new InvalidArgumentException('Property was defined more than once');
        }

        $this->properties[$field] = $this->convertToCType($type);
    }

    /**
     * Returns the C code that initializes the extension global.
     *
     * @param string $name
     * @param array  $global
     * @param string $namespace
     *
     * @return string
     *
     * @throws RuntimeException
     * @throws InvalidArgumentException
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

            case 'string':
                return "\t".$namespace.'_globals->'.$this->simpleName.'.'.$name.' = ZSTR_VAL(zend_string_init(ZEND_STRL("'.$global['default'].'"), 0));';

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
     * @param array  $global    - global structure (type, default...)
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

            case 'string':
                return sprintf(
                    'STD_PHP_INI_ENTRY(%s, %s, %s, NULL, %s, %s, %s)',
                    '"'.$iniName.'"',
                    '"'.$global['default'].'"',
                    $scope,
                    $structName,
                    'zend_'.$namespace.'_globals',
                    $namespace.'_globals',
                );
        }

        return '';
    }

    /**
     * Generates the internal c-type according to the php's type.
     *
     * @throws InvalidArgumentException
     */
    protected function convertToCType(string $type): string
    {
        switch ($type) {
            case 'boolean':
            case 'bool':
                return 'zend_bool';

            case 'hash':
                return 'HashTable* ';

            case 'string':
                return 'zend_string* ';

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
