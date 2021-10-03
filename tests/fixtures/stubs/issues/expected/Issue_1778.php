<?php

namespace Stubs;

use Psr\Http\Message\RequestInterface;

/**
 * Test Class Example Description
 *
 * <code>
 * $container = new Di();
 * $config = [
 *    'param' => 42
 * ];
 *
 * $example = new Issue_1778($request, $config);
 * </code>
 */
class Issue_1778
{
    const PROPERTY_EXAMPLE = 'test_property';

    /**
     * @var <RequestInterface>
     */
    private $request;

    /**
     * @var array
     */
    protected $config;

    /**
     * @param \Psr\Http\Message\RequestInterface $request
     * @param array $config
     */
    public function __construct(\Psr\Http\Message\RequestInterface $request, array $config)
    {
    }

    /**
     * Some useful description
     *
     * @return mixed|null
     * @param string $key
     */
    public function getVar(string $key)
    {
    }
}
