#include "../header/client.h"



void* lecture(void* socket){
  int* socketLecture = (int*)socket;
  char buffer[64];
  ssize_t taille=recv(*socketLecture, buffer, 64, 0);

  do{
    printf("taille(%ld) // Recu : %s\n", taille, buffer);
    taille=recv(*socketLecture, buffer, 64, 0);
  }while(taille > 0);


  printf("Deconnexion du serveur\n");
  shutdown(*socketLecture, 2);
  close(*socketLecture);
  return NULL;
}

int main(){
  sockaddr_in_t sin;
  socklen_t taille = sizeof(sin);
  pthread_t thread;

  //On crée notre socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(clientSocket==-1){
    printf("Sortie à cause d'un bug de création Socket\n");
    return 1;
  }
  printf("La socket %d est maintenant ouverte en mode TCP/IP\n", clientSocket);


  //Configuration
  sin.sin_addr.s_addr = inet_addr("172.18.41.172");    //inet_addr("172.18.41.172") afin de connaitre l'adresse ip via ifconfig
  sin.sin_family = AF_INET;                           //Protocole ici (IP)
  sin.sin_port = htons(PORT);                         //Port

  int* testConnect= malloc(sizeof(int));
  *testConnect = -1;
  int i;
  *testConnect = connect(clientSocket, (struct sockaddr*)&sin, sizeof(sin));
  for(i=1;i<=NB_TENTATIVE && *testConnect==-1;i++){
    printf("Nouvelle tentative de connexion (%i)\n",i);
    sleep(DELAI);
    *testConnect = connect(clientSocket, (struct sockaddr*)&sin, sizeof(sin));
  }
  if(i>=NB_TENTATIVE){
    printf("Nombre de tentative max atteinte (%i)\n\nEXIT\n",i);
    return 1;
  }

  printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));

  pthread_create(&thread, NULL, lecture, (void*)&clientSocket);

  int choix = -1;
  do{
    printf("---MENU TEST ---\n");
    printf("1) QUITTER\n");
    printf("\n");
    scanf("%i",&choix);
    switch (choix){
      case 1: //OPTN: QUITTER
        printf("On quitte le client. CODE DE SORTIE: (%i: attendu -1 / %i: attendu 0)\n",shutdown(clientSocket, 2),close(clientSocket));
        choix=-1;
        return 1;
      default:
      printf("Mais fréro, tu es cringe\n\n");
      choix=-1;
      break;
    }
  }while(choix!=1);
  free(testConnect);
  return 0;
}
