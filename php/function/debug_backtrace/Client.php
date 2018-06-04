<?php
namespace debug;

include 'Second.php';
/**
 * Created by PhpStorm.
 * User: lujiang
 * Date: 2017/8/28
 * Time: 15:18
 */
class Client
{
      public static function main(){
          echo 'Class Client!';
          echo '<br/>';
          Second::execTrace();
      }
}

Client::main();