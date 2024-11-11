AVANCEMENT : 

OK : Implanter le jeu d’Awalé : représentation interne, comment jouer un coup, compter les points, sauvegarder une partie, imprimer l’état du plateau etc. 
OK : Concevez une application client/serveur. Chaque client s’inscrit avec un pseudo.
OK : Chaque client peut demander au serveur la liste des pseudos en ligne.
OK : Un client A peut défier un autre client B. B peut accepter ou refuser.
OK : Quand une partie est créée entre A et B, le serveur décide de qui commence par tirage au sort. Le serveur vérifie la légalité des coups joués (en utilisant le code créé pour 0).
OK :Si vous avez une version qui fonctionne pour une partie, Vérifiez que cela fonctionne pour plusieurs parties en jeu. Vous pouvez ajouter comme fonctionalité un listing des parties en cours, et un mode “observateur” pour lequel le serveur envoie le plateau et le score à C qui observe la partie entre A et B.
OK : Implanter une option de chat : les joueurs en plus d’envoyer des coups à peuvent échanger des messages pour discuter (dans et en dehors d’une partie).
OK : Donnez la possiblité à un joueur d’écrire une bio : disons 10 lignes en ASCII, pour se présenter. On peut demander au serveur de nous afficher la bio d’un pseudo particulier.
OK : Ajouter un mode privé: un joueur peut limiter la liste des observateurs à une liste d’amis. Implanter cette fonctionalité.
OK : Ajouter la fonctionnalité de sauvegarde de partie jouée pour pouvoir la regarder par la suite.

PAS OK / BONUS : 
- Persistence : Inscrire les utilisateurs pour qu'ils puissent se reconnecter après déconnexion
- Couleurs / IHM
- Pour l'instant, les "commandes" sont codées sur 5 caractères car parsing complexe --> Trouver une solution
- Meilleurs noms pour les commandes
- Trouver une meilleure architecture
idées : 

- Encore trop brouillon : renommage, variables, code franco-anglais...
- Réorganiser le dossier / revoir la modularité ... (pour l'instant y a trop de fichiers dispersés / peut être 4 modules Client Serveur Jeu Util ...)



Libre à vore imagination: classement des joueurs : article wikipedia sur le classement elo, organisation de tournois, adapter pour un autre jeu etc.
