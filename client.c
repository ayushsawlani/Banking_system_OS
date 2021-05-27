#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]){
    struct sockaddr_in server;
    int sd;
    //char buf[80];
    
    sd = socket(AF_UNIX,SOCK_STREAM,0);
    if(sd == 0){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    int portno=atoi(argv[1]);
    printf("%d\n", portno);
    server.sin_family = AF_UNIX;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portno);

    
    if(connect(sd,(struct sockaddr *)(&server),sizeof(server)) == -1){
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    char buf[80];
    read(sd, buf, 80);//login status (user or admin)
    printf("%s\n", buf);
    fflush(stdout);
    scanf("%s", buf);
    fflush(stdin);
    int type=atoi(buf);
    write(sd, buf, sizeof(buf));//login type inputted

    read(sd, buf, 80);//enter ID:
    printf("%s\n", buf);
    fflush(stdout);
    scanf("%s", buf);
    fflush(stdin);
    int uid=atoi(buf);
    write(sd, buf, sizeof(buf));//ID type inputted

    read(sd, buf, 80);//ID status;
    printf("%s\n", buf);
    fflush(stdout);

    if(strcmp(buf, "Id found\n")!=0)
    {
        return 0;
    }

    read(sd, buf, 80);//enter password;
    printf("%s\n", buf);
    fflush(stdout);
    scanf("%s", buf);
    fflush(stdin);
    write(sd, buf, sizeof(buf));

    read(sd, buf, 80);//authentication status
    printf("%s\n", buf);
    fflush(stdout);
 
    if(strcmp(buf, "Login Successful\n")!=0)
    {
        return 0;
    }


    if(type ==1)
    {
        read(sd, buf, 80);//type of query
        printf("%s\n", buf);
        fflush(stdout);
        scanf("%s", buf);
        fflush(stdin);
        int otype=atoi(buf);
        write(sd, buf, sizeof(buf));//type of query outputted
        if((otype==1)||(otype==2))
        {
            read(sd, buf, 80);//enter amount
            printf("%s\n", buf);
            fflush(stdout);
            scanf("%s", buf);
            fflush(stdin);
            int otype=atoi(buf);
            write(sd, buf, sizeof(buf));// amount entered
        
            read(sd, buf, 80);//feedback
            printf("%s\n", buf);
            fflush(stdout);
 
        }
        else
        {
            read(sd, buf, 80);//feedback
            printf("%s\n", buf);
            fflush(stdout);
            
        }
    
    }
    if(type==2)
    {
        read(sd, buf, 80);//type of database
        printf("%s\n", buf);
        fflush(stdout);
        scanf("%s", buf);
        fflush(stdin);
        int rtype=atoi(buf);
        write(sd, buf, sizeof(buf));//type of database outputted

        read(sd, buf, 80); //type of operation
        printf("%s\n", buf);
        fflush(stdout);
        scanf("%s", buf);
        fflush(stdin);
        int otype=atoi(buf);
        write(sd, buf, sizeof(buf));//type of operation outputted
        if(otype==1)
        {
            read(sd, buf, 80);
            printf("%s\n", buf);
            fflush(stdout);
            scanf("%s", buf);
            fflush(stdin);//id
            write(sd, buf, sizeof(buf));


            if(rtype==1)
            {
                read(sd, buf, 80);
                printf("%s\n", buf);
                fflush(stdout);
                scanf("%s", buf);
                fflush(stdin);//password
                write(sd, buf, sizeof(buf));

                read(sd, buf, 80);
                printf("%s\n", buf);
                fflush(stdout);
                scanf("%s", buf);
                fflush(stdin);//type
                write(sd, buf, sizeof(buf));

                read(sd, buf, 80);
                printf("%s\n", buf);
                fflush(stdout);
                scanf("%s", buf);
                fflush(stdin);//account_number
                write(sd, buf, sizeof(buf));
            }
            else
            {
                read(sd, buf, 80);
                printf("%s\n", buf);
                fflush(stdout);
                scanf("%s", buf);
                fflush(stdin);//balance
                write(sd, buf, sizeof(buf));
            }
            read(sd, buf, 80);//add status
            printf("%s\n", buf);
            fflush(stdout); 
        }
        else if(otype==2)
        {
            read(sd, buf, 80);//id asked
            printf("%s\n", buf);
            fflush(stdout);
            scanf("%s", buf);
            fflush(stdin);
            write(sd, buf, sizeof(buf));//id sent
            
            read(sd, buf, 80);//read status
            printf("%s\n", buf);
            fflush(stdout);
        }
        else if(otype==3)
        {
            read(sd, buf, 80);//id asked
            printf("%s\n", buf);
            fflush(stdout);
            scanf("%s", buf);
            fflush(stdin);
            write(sd, buf, sizeof(buf));//id sent
            
            read(sd, buf, 80);//read status
            printf("%s\n", buf);
            fflush(stdout);
        }
        
    }
}
