<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test\FileSystem;

use League\Flysystem\Filesystem as Flysystem;
use Symfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException;
use Symfony\Component\DependencyInjection\Exception\ServiceNotFoundException;
use Zephir\FileSystem\FileSystemInterface;
use Zephir\Test\KernelTestCase;
use Zephir\Zephir;

class HardDiskTest extends KernelTestCase
{
    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1781
     * @expectedException \Zephir\Exception\FileSystemException
     * @expectedExceptionMessage Directory not empty
     */
    public function shouldCatchErrorException()
    {
        self::bootKernel();

        $container = self::$kernel->getContainer();

        try {
            $compilerFs = $container->get(FileSystemInterface::class);
            $this->createFlysystemMock($compilerFs);

            $compilerFs->clean();
        } catch (ServiceCircularReferenceException $e) {
            $this->fail($e->getMessage());
        } catch (ServiceNotFoundException $e) {
            $this->fail($e->getMessage());
        } catch (\ReflectionException $e) {
            $this->fail($e->getMessage());
        }
    }

    /**
     * @param $compilerFs
     *
     * @throws \ReflectionException
     */
    private function createFlysystemMock($compilerFs)
    {
        $flysystem = $this->createMock(Flysystem::class);
        $exception = new \ErrorException('Directory not empty');

        $flysystem
            ->expects($this->once())
            ->method('deleteDir')
            ->with(Zephir::VERSION)
            ->willThrowException($exception);

        $reflection = new \ReflectionClass($compilerFs);

        $filesystem = $reflection->getProperty('filesystem');
        $filesystem->setAccessible(true);
        $filesystem->setValue($compilerFs, $flysystem);
    }
}
