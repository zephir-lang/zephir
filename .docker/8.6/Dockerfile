FROM composer:latest AS composer
FROM php:8.6-rc-fpm

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
    libzip-dev

# The PHP 8.6 images ship neither PEAR (so no `pecl`) nor mbstring.
RUN for pkg in psr-1.2.0 zephir_parser-2.11.0; do \
        curl -fsSL "https://pecl.php.net/get/${pkg}.tgz" | tar xz -C /tmp && \
        (cd "/tmp/${pkg}" && phpize && ./configure && make && make install) && \
        rm -rf "/tmp/${pkg}" || exit 1; \
    done

RUN docker-php-ext-configure mbstring --disable-mbregex && \
    docker-php-ext-install zip gmp intl mysqli mbstring && \
    docker-php-ext-enable psr zephir_parser

COPY --from=composer /usr/bin/composer /usr/local/bin/composer
# Bash script with helper aliases
COPY ./.bashrc /root/.bashrc

CMD ["php-fpm"]
