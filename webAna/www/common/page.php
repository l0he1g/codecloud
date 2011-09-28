<?php
   //header的上部分,主要是设置页面的标题
   function page_header_top($title="未设置标题") {
     echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">';
     echo '<html xmlns="http://www.w3.org/1999/xhtml">';
     echo '<head>';
     echo '<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>';
     echo '<title>'.$title.'</title>';
   }
   
  //header的下部分,主要是设置页面的背景颜色
   //function page_header_down($bgcolor="#dfe8f6") {
   //function page_header_down($bgcolor="url(images/bgstripe.gif)") {
   function page_header_down($bgcolor="#ffffff") {
   	 echo '</head>';
     echo '<body style="background:'.$bgcolor.';">';
   }
   
  //footer部分
   function page_footer(){
   	echo '</body>';
    echo '</html>';
   }
?>
