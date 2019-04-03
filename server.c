
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#define porta      6666
#define sizeBuffer 1024

char buffer[sizeBuffer], bufferAux[sizeBuffer], *locMem;
char commands[] = "/x /mem /disc /time /pros /port /help";
int file, sizeBufferInt, skt, tasks, choose;
struct sockaddr_in server;


int main()
{    
    skt = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons (porta);
    memset(&(server.sin_zero),0x00,sizeof(server.sin_zero));
    tasks = sizeof(struct sockaddr_in);
    
    printf("\n Bem Vindo ao WebSocket - Server\n\n");
    bind(skt,(struct sockaddr *)&server,sizeof(struct sockaddr));
    listen(skt,1);
    printf(">> Servidor esta escutando na porta %d\n\n",porta);
    
    skt = accept(skt,(struct sockaddr *)&server,&tasks);
    printf(">> A Conexao com o endereco %s foi estabelecida\n\n",inet_ntoa(server.sin_addr));

    // Envia ack p/ cli
    strcpy(buffer,"Comunicacao com Server OK!!!");
    strcpy(bufferAux,buffer);
    send(skt,buffer,strlen(buffer), 0);

    // Recebe ack do cli
    sizeBufferInt = recv(skt, buffer,sizeBuffer, 0);
    buffer[sizeBufferInt]=0x00;
    printf(">: %s\n",buffer);
    
    do
    {
    sizeBufferInt = recv(skt,buffer,sizeBuffer,0);
    buffer[sizeBufferInt]=0x00;
    
      if (strncmp(buffer,"/",1) == 0)
      {

        locMem = strstr(commands,buffer);
        choose = locMem - commands;

        switch(choose)
        {
          case 0: break;
          case 14:
            puts(">> Tempo Logado Requisitado");
            system("uptime > temp");
            file = open("temp", O_RDONLY, 0666);
              
              if(file < 0)
              {
                puts("Erro no arquivo temporario!");
              }

            sizeBufferInt = read(file,buffer,sizeof(buffer));
            close(file);
            send(skt,buffer,sizeBufferInt,0);
            system("rm -r temp");
          break;
          case 20:
            puts(">> Processos Rodando no serveridor");
            system("ps -a > temp");
            file = open("temp", O_RDONLY, 0666);
              
              if(file < 0)
              {
                puts("Erro no arquivo temporario!");
              }      
            
            sizeBufferInt = read(file,buffer,sizeof(buffer));
            close(file);
            send(skt,buffer,sizeBufferInt,0);
            system("rm -r temp");
          break;
          case 26:
            puts(">> Portas abertas no serveridor");
            system("netstat -ant > temp");
            file = open("temp", O_RDONLY, 0666);
              
              if(file < 0)
              {
                puts("Erro no arquivo temporario!");
              }      
                
            sizeBufferInt = read(file,buffer,sizeof(buffer));
            close(file);
            send(skt,buffer,sizeBufferInt,0);
            system("rm -r temp");
          break;
          case 32:
            puts(">> Help Solicitado");
            file = open("help", O_RDONLY, 0666);
            
              if(file < 0)
              {
                puts("Erro no arquivo temporario!");
              }      
                
            sizeBufferInt = read(file,buffer,sizeof(buffer));
            close(file);
            send(skt,buffer,sizeBufferInt,0);
          break;
            
        }
      }
      else
        while(strcmp(bufferAux,buffer) != 0)
        {
          printf(">: %s\n",buffer);     
          strcpy(bufferAux,buffer);   
         }   

    }while(strcmp(buffer,"/x") != 0);
    
    close(skt); 
    printf(">> A Conexao com o Client %s foi encerrada!!!\n\n",inet_ntoa(server.sin_addr));
    exit(0);
}