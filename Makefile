
# FR :  Fichier Makefile racine

# Cette variable contient la liste des sous répertoires
# Pour lesquels il faudra constuire un programme.
# Pour l'instant seul le serveur WEB est dans la liste
FOLDERS=webserver

# Indique à make les règles qui ne correspondent pas à la création
# d'un fichier
# make lancera toujours la construction de cette règle si elle est
# demandée, même si un fichier/répertoire du nom de la cible existe
# On indique ici la règle all ainsi que les répertoires
.PHONY: all $(FOLDERS)

# La règle qui sera executée si on lance make sans arguments
all: $(FOLDERS)

# Cette règle va lancer make dans le répertoire webserver
# option -C de make
webserver: 
	make -C webserver
