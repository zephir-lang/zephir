namespace Test\Globals;

class ServerRequestFactory
{
    /**
     * @link https://github.com/phalcon/zephir/issues/1934
     */
    public function load(
        array server = null,
        array get = null,
        array post = null,
        array cookies = null,
        array files = null
    ) -> array {
        array globalCookies = [],
              globalFiles   = [],
              globalGet     = [],
              globalPost    = [],
              globalServer  = [];

        /**
         * Ensure that superglobals are defined if not
         */
        if !empty _COOKIE {
            let globalCookies = _COOKIE;
        }

        if !empty _FILES  {
            let globalFiles = _FILES;
        }

        if !empty _GET  {
            let globalGet = _GET;
        }

        if !empty _POST  {
            let globalPost = _POST;
        }

        if !empty _SERVER  {
            let globalServer = _SERVER;
        }

        let server  = this->checkNullArray(server, globalServer),
            files   = this->checkNullArray(files, globalFiles),
            cookies = this->checkNullArray(cookies, globalCookies),
            get     = this->checkNullArray(get, globalGet),
            post    = this->checkNullArray(post, globalPost);

        return [
            "server":  server,
            "files":   files,
            "cookies": cookies,
            "get":     get,
            "post":    post
        ];
    }

    /**
     * Checks the source if it null and returns the super, otherwise the source
     * array
     */
    private function checkNullArray(var source, array super) -> array
    {
        if unlikely null === source {
            return super;
        }

        return source;
    }
}
