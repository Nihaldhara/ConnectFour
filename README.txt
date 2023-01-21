Bien le bonjour Messieurs, 
Dans ce magnifique Puissance 4 designé à la main par nous-même, vous trouverez les fonctionnalités suivantes :

Sur le menu, nous avons utilisé la librairie SDL mixer pour rajouter une musique d'ambiance sur le menu et une autre sur les parties, ainsi qu'un crépitement de feu, tout cela sur trois channels différents. 
Nous avons animé le feu en utilisant 4 images différentes, et en les faisant alterner en fonction du temps par frame.
Vous pouvez choisir entre les 3 difficultés d'IA suivantes : (ez : random; normal : profondeur 4; wow : profondeur 8)
On peut ici sélectionner une partie parmi 3 modes de jeu : 
    - IA versus IA : deux IA jouent l'une contre l'autre avec une difficulté variable (choisi dans le menu options)
    - IA versus Player : une IA (a difficulté variable) joue contre un joueur (le joueur commençant la partie)
    - Player versus Player : Deux joueurs jouent l'un contre l'autre 
Cette IA est un minmax avec élagage alpha beta (sauf le random), elle va a la profondeur indiquée pour faire une évaluation du noeud, cette évaluation est faite grâce à un tableau fixe de nombre de connexion de chaque cellule. 
L'IA parcourt les différentes colonnes du board selon un ordre bien défini (3;2;4;1;5;0;6).
Le random fait un random modulo 6 tant qu'il ne peut pas poser son jeton. 

Affichage en jeu :
	Deux icônes de joueur (team potoo et el kakapo) indiquent à qui est le tour actuel.
Il est possible d'appuyer sur la touche "H" du clavier pour afficher une aide qui ne disparaitra que quand le joueur aura joué son coup. 
Cette aide fonctionne dans les deux modes de jeu impliquant des joueurs humains. Cette aide de l'IA s'adapte à l'aide choisie dans les options (dans le menu).
Un bouton pour mettre en pause permettant de partir puis de revenir à volonté pendant un spectaculaire combat d'IA profondeur 8, après avoir fait une pause pour reprendre ses esprits face à ce trop-plein de génie.
La pause est utilisable dans tous les modes de jeu.
L'IA a un timer pour éviter de jouer trop vite.
- A la fin d'une partie un écran vous permet de voir qui a gagné et de regarder la grille finale en même temps.
On clique ensuite sur le bouton "continuer, qui nous redirige vers le menu, et reset la partie.
Tous les boutons sont fonctionnels on les a tous testés promis.