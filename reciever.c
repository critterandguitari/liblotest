
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "lo/lo.h"

int done = 0;

void error(int num, const char *m, const char *path);

int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data);

int foo_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data);

int blobtest_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data);

int quit_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data);

int main()
{

    // start server
    lo_server s = lo_server_new("7770", error);

    // match anything
    lo_server_add_method(s, NULL, NULL, generic_handler, NULL);

    // match other stuff
    lo_server_add_method(s, "/foo/bar", "fi", foo_handler, NULL);

    lo_server_add_method(s, "/blobtest", "b", blobtest_handler, NULL);

    lo_server_add_method(s, "/quit", "", quit_handler, NULL);
	
    // poll at 10ms
    do {
        lo_server_recv_noblock(s, 10); // 10ms timeout
    } while (!done);

    return 0;
}

void error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
}

/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    int i;
    printf("path: <%s>\n", path);
    for (i = 0; i < argc; i++) {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp((lo_type)types[i], argv[i]);
        printf("\n");
    }
    printf("\n");
    fflush(stdout);

    return 1;
}

int foo_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("%s <- f:%f, i:%d\n\n", path, argv[0]->f, argv[1]->i);
    fflush(stdout);

    return 0;
}

int blobtest_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    /* example showing how to get data for a blob */
    int i, size = argv[0]->blob.size;
    char mydata[6];

    unsigned char *blobdata = (unsigned char*)lo_blob_dataptr((lo_blob)argv[0]);
    int blobsize = lo_blob_datasize((lo_blob)argv[0]);

    /* Alternatively:
     * blobdata = &argv[0]->blob.data;
     * blobsize = argv[0]->blob.size;
     */

    /* Don't trust network input! Blob can be anything, so check if
       each character is a letter A-Z. */
    for (i=0; i<6 && i<blobsize; i++)
        if (blobdata[i] >= 'A' && blobdata[i] <= 'Z')
            mydata[i] = blobdata[i];
        else
            mydata[i] = '.';
    mydata[5] = 0;

    printf("%s <- length:%d '%s'\n", path, size, mydata);
    fflush(stdout);
    
    printf("blob \n");
    fflush(stdout);

    return 0;
}


int quit_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    done = 1;
    printf("quiting\n\n");

    return 0;
}

/* vi:set ts=8 sts=4 sw=4: */
