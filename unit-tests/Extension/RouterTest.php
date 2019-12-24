<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;

class RouterTest extends TestCase
{
    public function testRouter()
    {
        $tests = [
            [
                'uri' => '',
                'controller' => 'index',
                'action' => 'index',
                'params' => [],
            ],
            [
                'uri' => '/',
                'controller' => 'index',
                'action' => 'index',
                'params' => [],
            ],
            [
                'uri' => '/documentation/index/hellao/aaadpqñda/bbbAdld/cc-ccc',
                'controller' => 'documentation',
                'action' => 'index',
                'params' => ['hellao', 'aaadpqñda', 'bbbAdld', 'cc-ccc'],
            ],
            [
                'uri' => '/documentation/index/',
                'controller' => 'documentation',
                'action' => 'index',
                'params' => [],
            ],
            [
                'uri' => '/documentation/index',
                'controller' => 'documentation',
                'action' => 'index',
                'params' => [],
            ],
            [
                'uri' => '/documentation/',
                'controller' => 'documentation',
                'action' => null,
                'params' => [],
            ],
            [
                'uri' => '/documentation',
                'controller' => 'documentation',
                'action' => null,
                'params' => [],
            ],
            [
                'uri' => '/system/admin/a/edit/hellao/aaadp',
                'controller' => 'admin',
                'action' => 'edit',
                'params' => ['hellao', 'aaadp'],
            ],
            [
                'uri' => '/es/news',
                'controller' => 'news',
                'action' => 'index',
                'params' => ['language' => 'es'],
            ],
            [
                'uri' => '/admin/posts/edit/100',
                'controller' => 'posts',
                'action' => 'edit',
                'params' => ['id' => 100],
            ],
            [
                'uri' => '/posts/2010/02/10/title/content',
                'controller' => 'posts',
                'action' => 'show',
                'params' => ['year' => '2010', 'month' => '02', 'day' => '10', 0 => 'title', 1 => 'content'],
            ],
            [
                'uri' => '/manual/en/translate.adapter.html',
                'controller' => 'manual',
                'action' => 'show',
                'params' => ['language' => 'en', 'file' => 'translate.adapter'],
            ],
            [
                'uri' => '/named-manual/en/translate.adapter.html',
                'controller' => 'manual',
                'action' => 'show',
                'params' => ['language' => 'en', 'file' => 'translate.adapter'],
            ],
            [
                'uri' => '/posts/1999/s/le-nice-title',
                'controller' => 'posts',
                'action' => 'show',
                'params' => ['year' => '1999', 'title' => 'le-nice-title'],
            ],
            [
                'uri' => '/feed/fr/blog/diaporema.json',
                'controller' => 'feed',
                'action' => 'get',
                'params' => ['lang' => 'fr', 'blog' => 'diaporema', 'type' => 'json'],
            ],
            [
                'uri' => '/posts/delete/150',
                'controller' => 'posts',
                'action' => 'delete',
                'params' => ['id' => '150'],
            ],
            [
                'uri' => '/very/static/route',
                'controller' => 'static',
                'action' => 'route',
                'params' => [],
            ],
        ];

        $router = new \Test\Router();

        $router->add('/', [
            'controller' => 'index',
            'action' => 'index',
        ]);

        $router->add('/system/:controller/a/:action/:params', [
            'controller' => 1,
            'action' => 2,
            'params' => 3,
        ]);

        $router->add('/([a-z]{2})/:controller', [
            'controller' => 2,
            'action' => 'index',
            'language' => 1,
        ]);

        $router->add('/admin/:controller/:action/:int', [
            'controller' => 1,
            'action' => 2,
            'id' => 3,
        ]);

        $router->add('/posts/([0-9]{4})/([0-9]{2})/([0-9]{2})/:params', [
            'controller' => 'posts',
            'action' => 'show',
            'year' => 1,
            'month' => 2,
            'day' => 3,
            'params' => 4,
        ]);

        $router->add('/manual/([a-z]{2})/([a-z\.]+)\.html', [
            'controller' => 'manual',
            'action' => 'show',
            'language' => 1,
            'file' => 2,
        ]);

        $router->add('/named-manual/{language:([a-z]{2})}/{file:[a-z\.]+}\.html', [
            'controller' => 'manual',
            'action' => 'show',
        ]);

        $router->add('/very/static/route', [
            'controller' => 'static',
            'action' => 'route',
        ]);

        $router->add("/feed/{lang:[a-z]+}/blog/{blog:[a-z\-]+}\.{type:[a-z\-]+}", 'Feed::get');

        $router->add("/posts/{year:[0-9]+}/s/{title:[a-z\-]+}", 'Posts::show');

        $router->add('/posts/delete/{id}', 'Posts::delete');

        $router->add("/show/{id:video([0-9]+)}/{title:[a-z\-]+}", 'Videos::show');

        foreach ($tests as $n => $test) {
            $router->handle($test['uri']);
            $this->assertSame($router->getControllerName(), $test['controller']);
            $this->assertSame($router->getActionName(), $test['action']);
        }
    }
}
