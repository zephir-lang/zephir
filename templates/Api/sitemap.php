<?php echo <<<EOF
<?xml version="1.0" encoding="ISO-8859-1"?>
EOF;
?>

<?php $date = date("Y-m-d") ?>
<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
    <url>
        <loc><?= $baseUrl ?>index.html</loc>
        <changefreq>daily</changefreq>
        <priority>1</priority>
        <lastmod><?= $date ?></lastmod>
    </url>

    <url>
        <loc><?= $baseUrl ?>namespaces.html</loc>
        <changefreq>daily</changefreq>
        <priority>1</priority>
        <lastmod><?= $date ?></lastmod>
    </url>

    <url>
        <loc><?= $baseUrl ?>classes.html</loc>
        <changefreq>daily</changefreq>
        <priority>1</priority>
        <lastmod><?= $date ?></lastmod>
    </url>

    <?php foreach($classes as $class){ ?>
        <url>
            <loc><?= $baseUrl ?>class/<?= str_replace("\\", "/", $class->getClassDefinition()->getCompleteName()) ?>.html</loc>
            <changefreq>daily</changefreq>
            <priority>1</priority>
            <lastmod><?= $date ?></lastmod>
        </url>

        <url>
            <loc><?= $baseUrl ?>source/<?= str_replace("\\", "/", $class->getClassDefinition()->getCompleteName()) ?>.html</loc>
            <changefreq>daily</changefreq>
            <priority>1</priority>
            <lastmod><?= $date ?></lastmod>
        </url>

    <?php } ?>

    <?php foreach($namespaces as $ns=>$def){ ?>
        <url>
            <loc><?= $baseUrl ?>namespace/<?= str_replace("\\", "/", $ns) ?>.html</loc>
            <changefreq>daily</changefreq>
            <priority>1</priority>
            <lastmod><?= $date ?></lastmod>
        </url>
    <?php } ?>

</urlset>