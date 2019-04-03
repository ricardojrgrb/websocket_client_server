#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*#define ip "127.0.0.1"*/
#define sizeBuffer 1024
#define port 6666

int main ()
{

    char ip[16], buffer[sizeBuffer], bufferAux[sizeBuffer], *locMem;
    char commands[]="/x /mem /disc /time /pros /port /help";
    int sizeBufferInt, sck, choose;
    struct sockaddr_in serv;
    
    printf("\n Bem Vindo ao Websck - Client\n\n");
    
    printf("\n Insira o Endereco IP do Server(127.0.0.1): ");
    gets(ip);
    
    sck = socket(AF_INET, SOCK_STREAM, 0);
    
    serv.sin_family = AF_INET; 
    serv.sin_addr.s_addr = inet_addr(ip); 
    serv.sin_port = htons (port); 

    memset (&(serv.sin_zero), 0x00, sizeof (serv.sin_zero));

    if(connect (sck, (struct sockaddr *)&serv, sizeof (struct sockaddr)) != 0) 
    {
        puts("\n Server Invalido!!!\n");
        exit(0);
    }

    printf(">> A conexao com o %s foi estabelecida na porta: %d \n\n",ip,port);

    // Recebe ack do serv
    sizeBufferInt = recv (sck, buffer, sizeBuffer, 0);
    buffer[sizeBufferInt] = 0x00;
    printf (">: %s\n",buffer);

    // Envia ack p/ serv
    strcpy(buffer, "Comunicacao com Client OK!!!");
    send(sck, buffer, strlen(buffer), 0 );    
    
    
    while(strcmp(buffer,"/x") != 0)
    {
       printf("> ");
       gets(buffer);
       strcpy(bufferAux,buffer);
       while(strncmp(buffer,"/",1) == 0) 
        if ((strcmp(buffer,"/mem") != 0) && (strcmp(buffer,"/disc") != 0) && (strcmp(buffer,"/time") != 0) && (strcmp(buffer,"/pros") != 0) 
              && (strcmp(buffer,"/port") != 0) && (strcmp(buffer,"/help") != 0) && (strcmp(buffer,"/x") != 0))
        {
            puts("> Comando Invalido ");
            printf("> ");
            gets(buffer);
        }
        else 
          break;    
            
        send(sck, buffer, strlen(buffer), 0);
       
       if ((strncmp(buffer,"/",1) == 0) && (strcmp(buffer,"/x") != 0))
       {
         sizeBufferInt = recv(sck, buffer, sizeBuffer, 0);
         buffer[sizeBufferInt] = 0x00;
         locMem = strstr(commands,bufferAux);
         choose = locMem - commands;
       
         switch (choose)
         {
            case 3: printf ("\n>: Memoria Disponivel:\n\n %s\n",buffer);
            break;
            case 8: printf ("\n>: Particoes do Disco:\n\n %s\n",buffer);
            break;
            case 14: printf ("\n>: Tempo Total Rodando:\n\n %s\n",buffer);
            break;
            case 20: printf ("\n>: Processos Rodando no Server:\n\n %s\n",buffer);
            break;
            case 26: printf ("\n>: Portas Abertas nos Server:\n\n %s\n",buffer);
            break;
            case 32: printf ("\n\n %s\n",buffer);
            break;     
        }        
      }
    } 
    
    close(sck);
    printf (">>A conexao com o Server foi finalizada!!!\n\n");
    exit(0);    
}