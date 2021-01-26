
#include <sys/time.h>
#include <iostream>
#include <atomic>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

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

int main()
{
    
    // start server	
    lo::Server st(9000);
    if (!st.is_valid()) {
        std::cout << "Nope." << std::endl;
        return 1;
    }

    st.add_method("example", "i", generic_handler, NULL);

    /*
     * Send some messages to the server we just created on localhost.
     */
    lo::Address a("localhost", "9000");

    /*
     * An individual message
     */
    a.send("example", "i", 7890987);

    /*
     * Initalizer lists and message constructors are supported, so
     * that bundles can be created easily:
     */
    a.send(lo::Bundle({{"example", lo::Message("i", 1234321)},
                       {"example", lo::Message("i", 4321234)}}));

    /*
     * Polymorphic overloads on lo::Message::add() mean you don't need
     * to specify the type explicitly.  This is intended to be useful
     * for templates.
     */
    lo::Message m;
    m.add(7654321);
    a.send("example", m);

    /*
     * Wait for messages to be received and processed.
     */
    int tries = 20;
    while (tries--) {
        lo_server_recv_noblock(st, 10); // 10ms timeout
    }
    
    std::cout << "Success!" << std::endl;
}

