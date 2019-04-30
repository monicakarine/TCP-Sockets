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
  char ip_address[]="127.0.0.1";
  int buffer = 1;
  //Criando um socket
  int client_socket;
  client_socket = socket(AF_INET,SOCK_STREAM,0); // Conexão com a internet; protocolo TCP
  // Para conectar com o outro socket, devemos especificar um endereço
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  // Especificando a porta:
  server_address.sin_port = htons(9001); //Função que converte o dado para que a struct entenda o número da porta
  server_address.sin_addr.s_addr = inet_addr(ip_address);

  struct sockaddr_in client_address;
  client_address.sin_family = AF_INET;

  /* Variáveis para a medição do tempo gasto na execução*/
  double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
  ti = tf = tempo = 0;
  struct timeval tempo_inicio,tempo_fim;
  gettimeofday(&tempo_inicio,NULL);  //Começa a contagem do tempo inicial
  /* ---------------------------------------------------*/

  int status = connect(client_socket,(struct sockaddr*)&server_address, sizeof(server_address)); // Estabelecendo conexão
  //Checando se a conexão foi realizada:
  if(status == -1){
    printf("Ocorreu um erro durante a conexão\n");
  }

  //Abrindo o arquivo
  FILE *arq;
  arq = fopen("arquivo.txt", "w");
  char mensagem_cliente[]= "arquivo.txt";
  write(client_socket, &mensagem_cliente, sizeof(mensagem_cliente)); // Enviando requisição de arquivo para Servidor

  //Recebendo dados do Servidor
  char resposta_servidor[buffer];
  int pacotes,cont=0;
  float bytes=0;
  read(client_socket , &pacotes, sizeof(pacotes));
  while(cont!=pacotes)
  {
    recv(client_socket,&resposta_servidor, sizeof(resposta_servidor),0);
    fwrite(resposta_servidor , sizeof(char) , sizeof(resposta_servidor), arq );
    cont++;
    bytes+= sizeof(resposta_servidor);
  }
  printf("Numero de pacotes recebidos: %d\n", pacotes);
  close(client_socket);  //Fechando o socket

  gettimeofday(&tempo_fim,NULL);
  tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
  ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
  tempo = (tf - ti)/1000000;
  printf("Buffer: %5d bytes,%10.2f kbps, (%.3f bytes em %.3f segundos)\n",sizeof(resposta_servidor),(double)(bytes/(1000*tempo)), bytes,tempo);
  return 0;
}
