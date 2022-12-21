//  Made by Troy A. Hamilton (12/20/2022)
//  No guarantees offered. Constructive comments to wmmmwd@gmail.com

#include <cstddef>      //NULL
#include <cstdlib>      //malloc free realloc
#include <cstring>      //memset
#include <unistd.h>     //close
#include <netinet/in.h> //sockaddr_in
#include <sys/socket.h> //socket bind listen accept recv
#include <arpa/inet.h>  //inet_pton

struct hbs //header'd byte sequence
{
    int length;
    char *start;
};

class TtcpClient
{
    private:
        int
            pvclientSocket;
    public:
        /*
            connects to an ip address and port number, returns true if it succeeded
        */
        bool ipConnect(const char *, unsigned short);
        
        /*
            will use a struct hbs to send data to whoever the object is connected to.
            returns true if it succeeded.
        */
        bool sendData(struct hbs &);
        
        /*
            receives data in to a dynamically allocated buffer.
            gives the info to a struct hbs parameter, receives a max number of bytes.
            returns false if failed, it is advisible to check the struct hbs
            members to make sure it actually succeeded, as this call only returns
            false on its internal failed system call checks.
        */
        bool dynamicReceive(struct hbs &, int);
        
        void clean();
};
