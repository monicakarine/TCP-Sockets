/************************************************
Redes de Computadores - Trabalho Pratico 1
Aluno: Mônica Karine Caridade Pereira
************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Bibliotecas para uso da tecnologia Sockets
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
//Biblioteca para medir o tempo gasto
#include <sys/time.h>

int main(){
  int buffer = 1;
  //Criando o socket para o servidor
  int server_socket;
  server_socket = socket(AF_INET,SOCK_STREAM,0);
  char ip_address[]="127.0.0.1";
  //Definindo o endereço do servidor
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  // Especificando a porta:
  server_address.sin_port = htons(9001); //Função que converte o dado para que a struct entenda o número da porta
  server_address.sin_addr.s_addr = inet_addr(ip_address);

  /* Variáveis para a medição do tempo gasto nvitimaa execução*/
  double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
  ti = tf = tempo = 0;
  struct timeval tempo_inicio,tempo_fim;
  gettimeofday(&tempo_inicio,NULL);  //Começa a contagem do tempo inicial
  /* ---------------------------------------------------*/

  //Ligando o socket ao endereço IP e a porta especificadas
  bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));
  //Depois de ligado, o servidor pode esperar para "ouvir" as conexões
  listen(server_socket, 5);

  int client_socket;
  client_socket = accept(server_socket,NULL, NULL); //aceitando a conexão com o socket do cliente
  char resposta_cliente[128];
  read(client_socket , &resposta_cliente, sizeof(resposta_cliente));

  printf("O cliente enviou: %s\n", resposta_cliente);

  // Abrindo o arquivo
  FILE *arq;
  arq = fopen(resposta_cliente, "r");

  if(arq == NULL){
    printf("Erro, nao foi possivel abrir o arquivo\n");
    close(server_socket);
    return;
  }
  char armazena[buffer];
  char teste[buffer];
  int pacotes=0;
  while(!feof(arq)) /*Enquanto não encontrar o fim do arquivo..*/
  {
    fread(teste, sizeof(char), sizeof(teste),arq);
    pacotes++;
  }
  printf("Número de pacotes enviados: %d\n",pacotes);
  send(client_socket, &pacotes,sizeof(pacotes),0);
  rewind(arq);
  int cont=0;

  while(!feof(arq) && cont!=pacotes) /*Enquanto não encontrar o fim do arquivo..*/
  {
    fread(armazena, sizeof(char), sizeof(armazena),arq);
    send(client_socket, armazena, sizeof(armazena),0);
    cont++;
  }

  fclose(arq);
  close(server_socket);

  gettimeofday(&tempo_fim,NULL);
  tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
  ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
  tempo = (tf - ti)/1000000;
  printf("Tempo gasto em segundos %.3f\n",tempo);

  return 0;
}
