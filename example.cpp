//  Made by Troy A. Hamilton (12/20/2022)
//  No guarantees offered. Constructive comments to wmmmwd@gmail.com

#include "TtcpClient.hpp" // should be included before TtcpClient.cpp
#include "TtcpClient.cpp"

#include <cstdio> //putchar
#include <iostream> //std::cout std::endl
#include <cstdlib> //free

void bytePrint(const char* start, int length)
{
    if(start == NULL || length < 1)
        return;
    
    for(int i = 0; i < length; i++)
    {
        putchar(*(start + i));
    }
    putchar('\n');
}
/* hbs reference
struct hbs //header'd byte sequence
{
    int length;
    char *start;
}; 
*/

int main()
{
    struct hbs
        hbuf, hbuf2;
    char
        msg[] = "GET / HTTP/1.0\r\n\r\n"; // the simplest http request
    TtcpClient
        instance;
    
    hbuf.start = msg;
    hbuf.length = strlen(msg);

    std::cout << "commencing ipConnect test" << std::endl;
    if(instance.ipConnect("34.223.124.45", 80) == false) // connect to neverssl.com (a non https website for testing purposes)
    {
        std::cout << "ipConnect error" << std::endl;
        return 0;
    }
    std::cout << "ipConnect success" << std::endl;
    
    std::cout << "commencing sendData test" << std::endl;
    if(instance.sendData(hbuf) == false)
    {
        std::cout << "sendData error" << std::endl;
        return 0;
    }
    std::cout << "sendData success" << std::endl;
    
    std::cout << "commencing dynamicReceive test" << std::endl;
    if(instance.dynamicReceive(hbuf2, 8192) == false)
    {
        std::cout << "dynamicReceive error" << std::endl;
    }
    else
        bytePrint(hbuf2.start, hbuf2.length);

    free(hbuf2.start); // to illustrate that dynamically received messages should be freed
    
    instance.clean(); // cleans the internals, required to prevent resource leaks
}
