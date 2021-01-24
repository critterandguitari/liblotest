#include <stdio.h>
#include <stdlib.h>
#include "lo/lo.h"

const char testdata[6] = "ABCDE";

int main(int argc, char *argv[])
{
    // setup a blob
    lo_blob btest = lo_blob_new(sizeof(testdata), testdata);

    lo_address t = lo_address_new(NULL, "7770");

    // send and report errors
    if (lo_send(t, "/foo/bar", "ff", 0.12345678f, 23.0f) == -1) {
	printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    }

    // send some other stuff
    lo_send(t, "/a/b/c/d", "sfsff", "one", 0.12345678f, "three", -0.00000023001f, 1.0);

    lo_send(t, "/a/b/c/d", "b", btest);

    lo_send(t, "/blobtest", "b", btest);

    lo_send(t, "/jamin/scene", "i", 2);

    // message with no arguments
    lo_send(t, "/quit", NULL);
    
    return 0;
}

/* vi:set ts=8 sts=4 sw=4: */
