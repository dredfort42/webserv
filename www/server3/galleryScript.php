<html>
    <head>
        <link rel="stylesheet" href="style.css" />
        <script type="text/javascript">

            function remove(filename) {
                window.location.href = "remove_image.php?picture=" + filename;
            }

        </script>
    </head>
    <body>

        <div class="container">
            <div class="hero">
                <h2 style="text-align:center"><a href="./index.php"><- Back to the gallery</a></h2>
                <?php
                    $image = $_GET['picture'];
                    if (file_exists($image)) {
                        echo "<img class='bigPic' src='".$image."'>";
                        echo "<button type='submit' name='deletebutton' class='red' onClick='remove(\"" . $image . "\")'>DELETE</button>";
                    } else {
                        echo '<h2 style="text-align:center">No such file!</h2>';
                        header("Refresh:2; url=index.php");
                    }
                    if(isset($_POST['deletebutton'])) {
                        echo "<h1> TEST________ </h1>";
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
