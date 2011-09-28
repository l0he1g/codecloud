<?php
   function loading($loading_src="images/loading.gif") {
   	echo '<div id="loading-mask" style="">';
    echo '<div id="loading">';
    echo '<div style="text-align:center;padding-top:5%"><img src="'.$loading_src.'" width="32" height="32" style="margin-right:8px;" align="middle"/>Loading...</div>';
    echo '</div>';
    echo '</div>';
   }
   
?>