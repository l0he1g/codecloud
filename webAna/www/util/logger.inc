<?php
    $glb_logger_handle = 0;
    
    function logger_enable()
    {
        global $glb_logger_handle;
        if( ! $glb_logger_handle)
        {
            $glb_logger_handle = fopen('/tmp/'.basename($_SERVER['PHP_SELF']).'.log','a');
            register_shutdown_function("logger_close");
        }
    }
    
    function logger_print($var, $prefix = '')
    {
        global $glb_logger_handle;
        if($glb_logger_handle != 0)
        {
            fwrite($glb_logger_handle, '[ ' . date('Y-m-d H:i:s').' ] ');
            if($prefix != '') fwrite($glb_logger_handle,$prefix);
            fwrite($glb_logger_handle, print_r($var, true));
            fwrite($glb_logger_handle, "\n");
            fflush($glb_logger_handle);
        }
    }
    
    function logger_close()
    {
        global $glb_logger_handle;
        if( $glb_logger_handle != 0)
        {
            fclose($glb_logger_handle);
            $glb_logger_handle = 0;
        }
    }

?>
