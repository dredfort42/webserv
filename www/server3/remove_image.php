<html>
    <head>
        <link rel="stylesheet" href="style.css" />
    </head>
    <body>

        <div class="container">
            <div class="hero">
                <h2 style="text-align:center"><a href="./index.php"><- Back to the gallery</a></h2>
                <?php
                    $image = $_GET['picture'];
                    if (unlink($image)) {
                        echo '<h2 style="text-align:center">The image was deleted successfully!</h2>';
                        header("Refresh:2; url=index.php");
                    } else {
                        echo 'There was an error deleting the file ' . $image;
                    }
                ?>
            </div>
		</div>
    </body>
</html>