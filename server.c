#include"file_handler.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include <string.h>
#include<pthread.h>
#include "user.h"
#include "account.h"
extern int urlist[];
extern int uolist[];
extern int accrlist[];
extern int accolist[];
void itoa(int x, char *text)
{
    for(int i=0;i<20;i++)
    {
        text[i]=(x%10+'0');
        x/=10;
        text[i+1]='\0';
    }
}
void* thread_func(void *arg)
{
        int sd= *(int *)(arg);
    write(sd, "Login as?\n(1)User\n(2)Admin\n", sizeof("Login as?\n(1)User\n(2)Admin\n"));
    char buf[80];
    read(sd, buf, 80);
    int type = atoi(buf);
    strcpy(buf, "Enter ID:\n");
    write(sd, buf, sizeof(buf));
    read(sd, buf, 80);
    
    int uid=atoi(buf);

    struct user *main_user=(struct user *)(malloc(sizeof(struct user)));
    int status=r_fetch(uid, main_user, sizeof(struct user), urlist, uolist, 1000, "users.dat");
    
    if(status==1)
        strcpy(buf, "Id found\n");
    else
        strcpy(buf, "Id not found\n");
    write(sd, buf, sizeof(buf));
    if(status)
    {
        strcpy(buf, "Enter password:\n");
        write(sd, buf, sizeof(buf));
        read(sd, buf, 80);
        printf("%s %s\n", main_user->password, buf);
        if(strcmp(main_user->password, buf)!=0)
        {
            strcpy(buf, "Wrong password\n");
            write(sd, buf, sizeof(buf));
            return NULL;
        }
        else if(main_user->type!=type)
        {
            strcpy(buf, "You don't have permissions\n");
            write(sd, buf, sizeof(buf));
            return NULL;
        }
        else
        {
            strcpy(buf, "Login Success\n");
            write(sd, buf, sizeof(buf));
        }
    }
    else
    {
        return NULL;
    }
    if(type==1)
    {
        strcpy(buf, "Select the Operation\n(1)Deposit\n(2)Withdraw\nGet Account Details\n");
        write(sd, buf, sizeof(buf));
        read(sd, buf, 80);
        int otype = atoi(buf);
        if(otype==1)
        {
            strcpy(buf, "Enter Amount\n");
            write(sd, buf, sizeof(buf));
            read(sd, buf, 80);
            int amt=atoi(buf);
            struct account *user_account=(struct account *)(malloc(sizeof(struct account)));
            int stat1=r_fetch(main_user->account_number, user_account, sizeof(struct account), accrlist, accolist, 1000, "accounts.dat");
            user_account->balance+=amt;
            int stat2=r_delete(user_account->id, accrlist, 1000, "accounts.dat");
            int stat3=r_add(user_account->id, user_account, sizeof(user_account), accrlist, accolist, 1000, "accounts.dat");
            if(stat1&&stat2&&stat3)
            {
                strcpy(buf, "Amount deposited: \n");
                write(sd, buf, sizeof(buf));
            }
            else
            {
                strcpy(buf, "Transaction failed\n");
                write(sd, buf, sizeof(buf));
            }
        }
        else if(otype==2)
        {
            strcpy(buf, "Enter Amount\n");
            write(sd, buf, sizeof(buf));
            read(sd, buf, 80);
            int amt=atoi(buf);
            struct account *user_account=(struct account *)(malloc(sizeof(struct account)));
            int stat1=r_fetch(main_user->account_number, user_account, sizeof(struct account), accrlist, accolist, 1000, "accounts.dat");
            user_account->balance-=amt;
            int stat2=r_delete(user_account->id, accrlist, 1000, "accounts.dat");
            int stat3=r_add(user_account->id, user_account, sizeof(user_account), accrlist, accolist, 1000, "accounts.dat");
            if(stat1&&stat2&&stat3)
            {
                strcpy(buf, "Amount deposited: \n");
                write(sd, buf, sizeof(buf));
            }
            else
            {
                strcpy(buf, "Transaction failed\n");
                write(sd, buf, sizeof(buf));
            }
        }
        //*/
        else
        {
            struct account *user_account=(struct account *)(malloc(sizeof(struct account)));
            int stat1=r_fetch(main_user->account_number, user_account, sizeof(struct account), accrlist, accolist, 1000, "accounts.dat");
            if(stat1==0)
            {
                strcpy(buf, "Query failed\n");
            }
            else
            {
                strcpy(buf, "Account Number:");
                char num[20];
                char bal[20];
                itoa(user_account->id, num);
                itoa(user_account->balance, bal);
                strcat(buf, num);
                strcat(buf, "\n Balance:");
                strcat(buf, bal);
                strcat(buf, "\n");
            }
            write(sd, buf, sizeof(buf));
        }
    }
    else
    {
        strcpy(buf, "Select the database you want to access\n(1)Customer\n(2)Admin\n");
        write(sd, buf, sizeof(buf));
        read(sd, buf, 80);
        int rtype = atoi(buf);
        
        strcpy(buf, "Select Operation\n(1)add\n(2)delete\n(3)retrieve\n");
        write(sd, buf, sizeof(buf));
        read(sd, buf, 80);
        int optype=atoi(buf);
        if(optype==1)
        {
            strcpy(buf, "enter Id\n");
            write(sd, buf, sizeof(buf));
            read(sd, buf, 80);
            int id=atoi(buf);//get id
            int status=0;
            if(rtype==1)
            {
                struct user *rec=(struct user *)(malloc(sizeof(struct user)));
                rec->id=id;
                strcpy(buf, "enter passwored\n");
                write(sd, buf, sizeof(buf));
                read(sd, buf, 80);
                strcpy(rec->password, buf);
                strcpy(buf, "enter type\n");
                write(sd, buf, sizeof(buf));
                read(sd, buf, 80);
                rec->type=atoi(buf);
                strcpy(buf, "enter account_number\n");
                write(sd, buf, sizeof(buf));
                read(sd, buf, 80);
                rec->account_number=atoi(buf);
                status=r_add(id, rec, sizeof(struct user), urlist, uolist, 1000, "users.dat");
            }
            else
            {
                struct account *rec=(struct account *)(malloc(sizeof(struct account)));
                rec->id=id;
                strcpy(buf, "enter balance\n");
                write(sd, buf, sizeof(buf));
                read(sd, buf, 80);
                rec->balance=atoi(buf);
                status=r_add(id, rec, sizeof(struct account), accrlist, accolist, 1000, "accounts.dat");
            }
            if(status)
            {
                strcpy(buf, "add successful\n");
            }
            else
            {
                strcpy(buf, "add failed\n");
            }
            write(sd, buf, sizeof(buf));
        }
        else if(optype==2)
        {
            strcpy(buf, "enter Id\n");
            write(sd, buf, sizeof(buf));
            read(sd, buf, 80);
            int id=atoi(buf);//get id
            int status=0;
            if(rtype==1)
            {
                status=r_delete(id, urlist, 1000, "users.dat");
            }
            else
            {
                status=r_delete(id, accrlist, 1000, "accounts.dat");
            }
            if(status)
            {
                strcpy(buf, "Delete successful\n");
            }
            else
            {
                strcpy(buf, "Delete failed\n");
            }
            write(sd, buf, sizeof(buf));
        }
        else if(optype==3)
        {
            strcpy(buf, "enter Id\n");
            write(sd, buf, sizeof(buf));
            read(sd, buf, 80);
            int id=atoi(buf);//get id
            int status=0;
            if(rtype==1)
            {
                struct user *rec=(struct user *)(malloc(sizeof(struct user)));
                status=r_fetch(id, rec, sizeof(struct user), urlist, uolist, 1000, "users.dat");
                if(status) 
                {
                    strcpy(buf, "Password:");
                    strcat(buf, rec->password);
                    strcat(buf, "\n");
                    write(sd, buf, sizeof(buf));
                }
            }
            else
            {
                struct account *rec=(struct account *)(malloc(sizeof(struct account)));
                status=r_fetch(id, rec, sizeof(struct account), accrlist, accolist, 1000, "accounts.dat");
                if(status) 
                {
                    strcpy(buf, "Balance:");
                    int bal=rec->balance;
                    char ba[22];
                    itoa(bal, ba);
                    strcat(buf, ba);
                    strcat(buf, "\n");
                    write(sd, buf, sizeof(buf));
                }
 
            }
            if(!status)
            {
                strcpy(buf, "fetch failed\n");
            }
            write(sd, buf, sizeof(buf));
        }
    }
}
int main(int argc, char *argv[])
{
    struct user* admin=(struct user*)(malloc(sizeof(struct user)));
    admin->id = 0;
    admin->password;
    strcpy(admin->password, "hi");
    printf("%s\n", admin->password);
    admin->type = 2;
    r_add(0, admin, sizeof(struct user), urlist, uolist, 1000, "./users.dat");
 

//    printf("%d\n", argc);
    printf("hello\n");
    struct sockaddr_in server, client;
    unsigned int sd,sz,nsd; //server file descriptor, size, nsd
    char buf[80]; //buffer 

    sd = socket(AF_UNIX,SOCK_STREAM,0);  //  or AF_INET for internet, TCP 

    int portno=atoi(argv[1]);
//    printf("%d\n", portno);
    server.sin_family = AF_UNIX;
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons(portno); //big endian (host to socket)
    bind(sd,(struct sockaddr *)&server,sizeof(server));

    listen(sd,2); //number of clients at a time;
    
    while(1){
        sz = sizeof(client);
        //sleep(5);
        if((nsd = accept(sd,(struct sockaddr *)(&client),&sz)) <0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_t tid;
        if(pthread_create(&tid,NULL,thread_func, (void*)&nsd) < 0){
            perror("Thread Failed");
            exit(EXIT_FAILURE);
        }

    }
    //*/
}