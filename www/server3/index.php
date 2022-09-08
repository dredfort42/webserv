<html>
    <head>
        <link rel='stylesheet' href='style.css' />
    </head>
    <body>

        <div class="heading">
            <h1 style="text-align:center">Memes gallery</h1>
        </div>
        <div class="tile-layout">

            <?php
                $filelist = glob("./pics/*");

                foreach($filelist as $filename){
                    echo "<a href='galleryScript.php?picture=".$filename."'><img class='picture' src='".$filename."'></a>";
                }
            ?>

        </div>

        <div class="center heading" id="wrapper">

        <!-- <! specify the encoding type of the form using the enctype attribute > -->

        <form method="POST" action="" enctype="multipart/form-data">        

            <input type="file" name="choosefile" value="" />

            <div>
                <button type="submit" name="uploadfile">
                UPLOAD
                </button>
            </div>

        </form>
        <?php
            error_reporting(0);
        ?>

        <?php
            $msg = ""; 

            // check if the user has clicked the button "UPLOAD" 
            if (isset($_POST['uploadfile'])) {

                $filename = $_FILES["choosefile"]["name"];
                $tempname = $_FILES["choosefile"]["tmp_name"];
                $folder = "pics/".$filename;   

                // Add the image to the "image" folder"
                if (move_uploaded_file($tempname, $folder)) {
                    $msg = "Image uploaded successfully";
                    header("Refresh:1");
                }else{
                    $msg = "Failed to upload image";
                }
            }
            echo '<h1 style="text-align:center">'.$msg.'</h1>'
        ?>

    </div>
    </body>
</html>