#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
int r_add(int id, void *record, size_t dat_size, int *rlist, int *olist, int size_rlist, char *filepath);
int r_check(int id, int *rlist, int size_rlist, char *filepath);
int r_delete(int id, int *rlist, int size_rlist, char *filepath);
int r_fetch(int id, void *rec, size_t dat_size, int *rlist, int *olist, int size_rlist, char* filepath);
#endif