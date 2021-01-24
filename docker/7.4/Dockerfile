FROM php:7.4-fpm

RUN apt update -y && apt install -y \
    wget \
    zip \
    git \
    apt-utils \
    sudo \
    libgmp-dev \
    libzip-dev && \
    pecl install psr

RUN docker-php-ext-install zip gmp && \
    docker-php-ext-enable psr

COPY --from=composer:latest /usr/bin/composer /usr/local/bin/composer

CMD ["php-fpm"]
