<?php
namespace Zephir;

abstract class BaseBackend
{
    /**
     * The name of the backend (e.g. ZendEngine2)
     * @var string
     */
    protected $name;

    /**
     * Resolves the path to the source kernel files of the backend
     * @return string Absolute path to kernel files
     */
    public function getInternalKernelPath()
    {
        return realpath(__DIR__ . '/../kernels/' . $this->name);
    }
    
    /**
     * Resolves the path to the source template files of the backend
     * @return string Absolute path to template files
     */
    public function getInternalTemplatePath()
    {
        return realpath(__DIR__ . '/../templates/' . $this->name);
    }
    
    public static function getActiveBackend()
    {
        if (version_compare(phpversion(), '7.0', '>=')) {
            return 'ZendEngine3';
        }
        return 'ZendEngine2';
    }
}
