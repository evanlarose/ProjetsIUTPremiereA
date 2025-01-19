Titres
4 niveaux de titres : 
# Un titre de niveau 1 est centré
## les autres sont cadré à gauche
### suivi d’un espace et d’un texte sur la même ligne,qui constitue le titre.
#### Vous jouerez sur la taille et la graisse de la police de caractères.

Listes
Toute suite de lignes commençant par un tiret (-) suivi d’un espace.
Si une ligne est trouvée sans tiret au départ, elle est ajoutée à la suite de la ligne précédente et ainsi de suite jusqu’à rencontrer une autre ligne débutant par un tiret+espace, ce qui constitue une nouvelle ligne de la liste, ou une ligne totalement vide (ou contenant uniquement des espaces), ce qui constitue alors la fin de la liste.

- Cillum ea enim do exercitation proident sint aute ex.
- Ullamco aute mollit enim cillum enim ex pariatur ex excepteur enim tempor id minim ex.

Tableaux
Une ligne commençant par un | (pipe) et suivi d’un certain nombre quelconque de tiret (-) et d’autres | (pipes). Le nombre d'intervalles |---|... indique le nombre de colonnes de détail. Des lignes de détail dont les cellules sont aussi encadrées par des | (pipes). Les lignes dont le nombre de cellules est inférieur aux colonnes de l’entête sont complétées par des champs vides. Les cellules surnuméraires aux colonnes de l’entête sont simplement ignorées. Le tableau se termine quand une ligne vide (ou avec uniquement des espaces) est rencontrée.

Exemple :
|Titre col1|Titre col2|Titre col3|
|-|-|-|
| 1 | Toto | toto@gmail.com |
| 2 | Lulu | lulu@hotmail.com |

Code
Un bloc de texte encadré par des ` (backticks ou accents graves) doit être écrit dans une fonte à chasse fixe. Ce bloc peut être au milieu d’une ligne normale. Exemple : Utilisez la commande `./gendoc` pour générer la documentation.Doit afficher ceci : Utilisez la commande ./gendoc pour lancer la génération de la documentation. Un bloc de texte encadré par des ``` (3 backticks) doit être écrit dans une fonte à chasse fixe et dans un paragraphe isolé. Les espaces doivent obligatoirement être préservés ! En HTML, plusieurs espaces consécutifs sont considérés comme un seul espace par le navigateur. Vous devez donc convertir les espaces consécutifs en "&nbsp;" pour préserver leur nombre dans ces blocs de code. Ces blocs sont généralement utilisés pour afficher du code ou des commandes à taper.
Exemple :
```
int main() {
printf("Hello World!\n");
return EXIT_SUCCESS;
}
```

URL
Une URL est constituée d’un texte affiché et d’un lien vers un site Web. Les deux peuvent être identiques mais on pourra aussi préférer présenter un nom en clair dans le texte plutôt qu’un lien long et incompréhensible. 
Le format est le suivant : [Texte à afficher](Lien)
Les liens doivent donc être cliquables.

Textes
Tout ce qui n’entre pas dans une des autres catégories de tags. Le texte doit être formaté dans des paragraphes. Un paragraphe se termine par une ligne vide (ou remplie uniquement d’espaces).

Formatages spéciaux
Dans les sections de texte et les tableaux, vous autoriserez le formatage des valeurs en gras (<b>...</b>) et/ou en italique (<i>...</i>) à l’exclusion de tout autre type de formatage qui devra être ignoré. Cela signifie que toute autre ouverture ou fermeture detag HTML (<xxx> ou </xxx>) sera simplement ignorée. Dans les autres tags, aucun formatage n’est autorisé, vous devez donc opérer ce même nettoyage