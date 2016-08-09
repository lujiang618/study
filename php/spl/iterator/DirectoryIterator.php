<?php
foreach (new DirectoryIterator('../../function') as $fileInfo) {
    if($fileInfo->isDot()) continue;
    echo $fileInfo->getFilename() . "\n";
}
