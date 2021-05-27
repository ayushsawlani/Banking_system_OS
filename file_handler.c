#include "file_handler.h"

int r_add(int id, void *record, size_t dat_size, int *rlist, int *olist, int size_rlist, char *filepath)
{
    if(r_check(id, rlist, size_rlist, filepath)!=-1)
    {
        return 0;
    }
    key_t k1=ftok(filepath, 'a');
    int semid=semget(k1, 1, IPC_CREAT);
    struct sembuf buf = {0,-1,0};
    //Locking the list before access
    semop(semid,&buf,1);    
    for(int i=0;i<size_rlist;i++)
    {
        if(rlist[i]==-1)
        {
            rlist[i]=id;
            int fd=open(filepath, O_RDONLY);
            olist[i]=lseek(fd, 0, SEEK_END);
            close(fd);
            break;
        }
    }
    buf.sem_op=1;
    semop(semid, &buf, 1);
    //lock released
    struct flock lck;
    lck.l_type = F_WRLCK;	/* setting a write lock */
 	lck.l_whence = SEEK_END;	/* offset l_start from beginning of file */
 	lck.l_start = (off_t)0;	
 	lck.l_len = (off_t)(dat_size);	/* until the end of the file */
    int fd=open(filepath, O_WRONLY);
    fcntl(fd, F_SETLK, &lck);
    lseek(fd, 0, SEEK_END);
    write(fd, record, dat_size);
    lck.l_type=F_UNLCK;
    fcntl(fd, F_SETLK, &lck);
    close(fd);
    return 1;
}
int r_check(int id, int *rlist, int size_rlist, char *filepath)
{
    key_t k1=ftok(filepath, 'a');
    int semid=semget(k1, 1, IPC_CREAT);
    struct sembuf buf = {0,-1,0};
    //Locking the list before access
    semop(semid,&buf,1);    
    
    for(int i=0;i<size_rlist;i++)
    {
        //printf("%d\n", rlist[i]);
        if(rlist[i]==id)
            return i;
    }
    buf.sem_op=1;
    semop(semid, &buf, 1);
    return -1;
 
}
int r_delete(int id, int *rlist, int size_rlist, char *filepath)
{
    key_t k1=ftok(filepath, 'a');
    int semid=semget(k1, 1, IPC_CREAT);
    struct sembuf buf = {0,-1,0};
    //Locking the list before access
    semop(semid,&buf,1);    
 
    for(int i=0;i<size_rlist;i++)
    {
        if(rlist[i]==id)
        {
            rlist[i]=-1;
            return 1;
        }
    }
    buf.sem_op=1;
    semop(semid, &buf, 1);
    return 0;
}
int r_fetch(int id, void *rec, size_t dat_size, int *rlist, int *olist, int size_rlist, char* filepath)
{
    int offset=r_check(id, rlist, size_rlist, filepath);
    if(offset==-1)
    {
        return 0;
    }
    offset=olist[offset];
    printf("%d\n", offset);
    struct flock lck;
    lck.l_type = F_RDLCK;	/* setting a write lock */
 	lck.l_whence = SEEK_SET;	/* offset l_start from beginning of file */
 	lck.l_start = (off_t)offset;	
 	lck.l_len = (off_t)(dat_size);	/* until the end of the file */
    int fd=open(filepath, O_RDONLY);
    fcntl(fd, F_SETLK, &lck);
    
    lseek(fd, offset, SEEK_SET);
    //printf("%lu\n", sizeof(dat_size));
    read(fd, rec, dat_size);
    lck.l_type=F_UNLCK;
    fcntl(fd, F_SETLK, &lck);
    close(fd);
    return 1;
}

int main()
{
    int *x=(int *)malloc(sizeof(int));
    (*x)=934;
    int rlist[3];
    int olist[3];
    for(int i=0;i<3;i++)
        rlist[i]=-1;
    
    int *y=(int *)malloc(sizeof(int));
    
    printf("%d\n", r_add(3, x, sizeof(*x), rlist, olist, 3, "users.dat"));
    //printf("%d\n", rlist[0]);
    //printf("%d\n", olist[0]);
    //printf("%d\n", sizeof(*y));
    printf("%d\n", r_fetch(3, y, sizeof(*y), rlist, olist, 3, "users.dat"));
    printf("%d\n", (*y));
    printf("%d\n", r_delete(3, rlist, 3, "users.dat"));
    printf("%d\n", r_fetch(3, y, sizeof(*y), rlist, olist, 3, "users.dat"));
    (*x)=808;
    printf("%d\n", r_add(4, x, sizeof(*x), rlist, olist, 3, "users.dat"));
    printf("%d\n", r_fetch(4, y, sizeof(*y), rlist, olist, 3, "users.dat"));
    printf("%d\n", (*y));
    //printf("%d\n", r_delete(3, rlist, 3));
//    printf("%d\n", r_add(3, x, rlist, olist, 3, "users.dat"));

}
//*/