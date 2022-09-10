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
                    if (file_exists($image)) {
                        echo "<img class='bigPic' src='".$image."'>";
                        echo "<form method='post'>";
                        echo "<button type='submit' name='deletebutton' class='red'>DELETE</button>";
                        echo "</form>";
                    } else {
                        echo '<h2 style="text-align:center">The image was deleted successfully!</h2>';
                        header("Refresh:2; url=index.php");
                    }

                    if(isset($_POST['deletebutton'])) {
                        if (unlink($image)) {
                            header("Refresh:0");
                        } else {
                            echo 'There was an error deleting the file ' . $image;
                        }
                    }
                ?>
            </div>
		</div>
    </body>
</html>