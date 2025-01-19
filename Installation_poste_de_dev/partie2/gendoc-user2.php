#!/usr/bin/php
<?php
    $string = 'Titre (blabla) Texte  (blabla)  Texte (Dessin).';
    preg_match_all("#(\(.+)\)#isU", $string, $output);
    echo array_pop($output);
?>