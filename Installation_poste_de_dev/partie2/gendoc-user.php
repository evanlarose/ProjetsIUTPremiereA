#!/usr/bin/php
<?php
    $version = $argv[1];
    $linetab = 0;
    $mot = '';
    $i = 0;
    $tabl = false;
    $nomDest = "doc-user-".$version.".html";
    $dest = fopen($nomDest, 'w');
    $file = fopen("doc.md", 'r');
    if ($file) {
        fwrite($dest, "<html>\n<head>\n<title>Markdown en HTML</title>\n</head>\n<body>\n<p> version : $version </p>\n");

        while (!feof($file)) {
            $line = fgets($file);

            if (!preg_match('/^\|(.*)/', $line) && $tabl) {
                fwrite($dest, "</table>");
                $tabl = false;
            }
            
            if (!preg_match('/^\|(.*)/', $line)) {
                if(preg_match("/\[(.*)\]/", $line, $cliquable)){
                    preg_match("(\((.*)\))", $line, $lien);
                    $texte = preg_replace("/\[(.*)\]*/", "<a href='$lien[1]'> $cliquable[1] </a>", $line);
                    fwrite($dest, $texte);
                }
                else if (preg_match('/^####(.*)/', $line, $match)) {
                    fwrite($dest, "<h4> $match[1] </h4>\n");
                }
    
                else if (preg_match('/^###(.*)/', $line, $match)) {
                    fwrite($dest, "<h3> $match[1] </h3>\n");
                }
    
                else if (preg_match('/^##(.*)/', $line, $match)) {
                    fwrite($dest, "<h2> $match[1] </h2>\n");
                }
    
                else if (preg_match('/^#(.*)/', $line, $match)) {
                    fwrite($dest, "<h1> $match[1] </h1>\n");
                }
    
                else if (preg_match('/}/', $line, $match)){
                    fwrite($dest, "<p> $match[0] </p>\n");
                }
    
                else if (preg_match('/^[A-Z](.*)/', $line, $match)){
                    fwrite($dest, "<p> $match[0] </p>\n");
                }
    
                else if (preg_match('/^[a-z](.*)/', $line, $match)){
                    fwrite($dest, "<p> $match[0] </p>\n");
                }
    
                else if (preg_match('/^[0-9](.*)/', $line, $match)){
                    fwrite($dest, "<p> $match[0] </p>\n");
                }
                
                else if (preg_match('/^ (.*)/', $line, $match)){
                    fwrite($dest, "<p> $match[0] </p>");
                }
    
                else if (preg_match('/^---(.*)/', $line, $match)){
                    fwrite($dest, "<hr>");
                }
    
                else if (preg_match('/^-(.*)/', $line, $match)){
                    fwrite($dest, "<ul>\n\t<li> $match[1] </li>\n</ul>\n");
                }    
            }

            else if (preg_match('/^\|(.*)/', $line, $match)){
                $tabl = true;
                if ($linetab == 0){
                    fwrite($dest, "<table>");
                    fwrite($dest, "\n\t<tr>\n\t\t<th>");
                    while($i < strlen($match[1])){
                        if($match[1][$i] != '|' && $match[1][$i] != ' '){
                            fwrite($dest, $match[1][$i]);
                        }
                        else if($i != 0 && $match[1][$i] != ' '){
                            fwrite($dest, "\n</th>");
                            if($i != strlen($match[1])-2){
                                fwrite($dest, "\n<th>");
                            }
                        }
                        $i = $i + 1;
                    }
                    $linetab =$linetab +1;
                    fwrite($dest, "\n</tr>");
                    $i = 0;
                }
                else{
                    if($match[1][$i+1] == "-" || $match[1][$i+2] == "-"){
                        fwrite($dest, "<tr><td colspan='50'><hr></td></tr>");
                    }
                    else{
                        fwrite($dest, "\n\t<tr>\n\t\t<td>");
                        while($i < strlen($match[1])){
                            if($match[1][$i] != '|' && $match[1][$i] != ' '){
                                if($match[1][$i] == '['){
                                    preg_match("(\((.*?)\))", $line, $lien);
                                    fwrite($dest, "<a href='$lien[1]'>");
                                }
                                else if($match[1][$i] == ']'){
                                    fwrite($dest, "</a>");
                                }
                                else if($match[1][$i] == '('){
                                    fwrite($dest, "<!--");
                                }
                                else if($match[1][$i] == ')'){
                                    fwrite($dest, "-->");
                                }
                                else{
                                    fwrite($dest, $match[1][$i]);
                                }
                            }
                            else if($i != 0 && $match[1][$i] != ' '){
                                fwrite($dest, "\n</td>");
                                if($i != strlen($match[1])-2){
                                    fwrite($dest, "\n<td>");
                                }
                            }
                            $i = $i + 1;
                        }
                        fwrite($dest, "\n</tr>");
                        $i = 0;
                    }
                }
            }
        }
        fwrite($dest, "</body>\n</html>");

        fclose($file);
    } else {
        echo "Erreur sur le fichier.";
    }
?>