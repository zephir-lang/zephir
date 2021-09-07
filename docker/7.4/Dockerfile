FROM composer:latest as composer
FROM php:7.4-fpm

RUN CPU_CORES="$(getconf _NPROCESSORS_ONLN)";
ENV MAKEFLAGS="-j${CPU_CORES}"

RUN apt update -y && apt install -y \
    wget \
    zip \
    git \
    apt-utils \
    sudo \
    libicu-dev \
    libgmp-dev \
    libzip-dev && \
    pecl install psr

RUN docker-php-ext-install zip gmp intl mysqli && \
    docker-php-ext-enable psr

# Install Zephir parser
RUN PHP_INI_DIR="$(dirname "$(php -i | grep /.+/conf.d/.+.ini -oE | head -n 1)")" && \
    cd /opt && \
    git clone -b "development" \
            --depth 1 \
            -q https://github.com/phalcon/php-zephir-parser \
            php-zephir-parser && \
    cd php-zephir-parser || exit 1 && \
    phpize && \
    ./configure --with-php-config="$(command -v php-config)" --enable-zephir_parser && \
    make -j"$(getconf _NPROCESSORS_ONLN)" && \
    sudo make install && \
    echo 'extension="zephir_parser.so"' |\
      sudo tee "$PHP_INI_DIR/zephir_parser.ini" && \
    php --ri "Zephir Parser"

COPY --from=composer /usr/bin/composer /usr/local/bin/composer

CMD ["php-fpm"]
