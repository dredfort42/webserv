<html>
    <head>
        <link rel='stylesheet' href='style.css' />
        <script>
            function getFile() {
                document.getElementById("upfile").click();
            }

            function sub(obj) {
                var file = obj.value;
                var fileName = file.split("\\");
                document.getElementById("uploadButton").innerHTML = fileName[fileName.length - 1];
                document.getElementById("uploadfile").click();
                event.preventDefault();
            }
        </script>
    </head>
    <body>

        <div class="heading">
            <h1 style="text-align:center">Memes gallery</h1>
        </div>
        <div class="tile-layout">

            <?php
                $filelist = glob("./pics/*");
                usort($filelist, function($a, $b) {
                    return filemtime($b) - filemtime($a);
                });
                foreach($filelist as $filename){
                    echo "<a href='galleryScript.php?picture=".$filename."'><img class='picture' src='".$filename."'></a>";
                }
            ?>

        </div>

        <div class="center heading" id="wrapper">
            <form method="POST" action="" enctype="multipart/form-data" >
                <div id="uploadButton" onclick="getFile()" class="input-button" name="uploadfile" >UPLOAD FILE</div>
                <div class="hidden">
                    <input id="upfile" type="file" value="upload" onchange="sub(this)" name="choosefile" />
                    <button hidden id="uploadfile" type="submit" name="uploadfile" >UPLOAD</button>
                </div>
            </form>

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
                        header("Refresh:0");
                    }else{
                        $msg = "Failed to upload image";
                    }
                    echo '<h1 style="text-align:center">'.$msg.'</h1>';
                }
            ?>

        </div>
    </body>
</html>