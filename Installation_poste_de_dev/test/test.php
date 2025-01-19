<?php
$jours = ['Lundi', 'Mardi', 'Mercredi', 'Jeudi', 'Vendredi',
'Samedi', 'Dimanche'];
?>
<!DOCTYPE html>
<html lang="fr">
    <head>
        <title>Document</title>
    </head>
    <body>
        <h1>Semaine</h1>
        <ul>
            <?php
            foreach($jours as $un_jour) {
            ?>
            <li><?php echo $un_jour ?></li>
            <?php
            }
            ?>
        </ul>
    </body>
</html>