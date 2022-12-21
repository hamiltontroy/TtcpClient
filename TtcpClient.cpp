//  Made by Troy A. Hamilton (12/20/2022)
//  No guarantees offered. Constructive comments to wmmmwd@gmail.com

bool TtcpClient::ipConnect(const char *presentational, unsigned short portNumber)
{
    struct sockaddr_in 
        serverAddr;

    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNumber);
    inet_pton(AF_INET, presentational, &serverAddr.sin_addr);
    
    pvclientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(connect(pvclientSocket, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr_in)) < 0)
    {
        close(pvclientSocket);
        return false;
    }
    
    return true;
}

bool TtcpClient::sendData(struct hbs &hbuf)
{
    if(send(pvclientSocket, hbuf.start, hbuf.length, 0) < hbuf.length)
    {
        return false;
    }
    
    return true;
}

bool TtcpClient::dynamicReceive(struct hbs &hbuf, int maxBytes)
{
    //sets up the struct to a safe condition in case of failure
    hbuf.start = NULL;
    hbuf.length = -1;
    
    int
        mallocIncrement = 4096; //this can be changed based on implementation
    char
        *bufferStart, *temporaryPointer;
    int
        memoryAllocationCount = 0, totalCharsReceived = 0, temporaryCharsReceivedCount;

    if(maxBytes < mallocIncrement)
        mallocIncrement = maxBytes;

    //allocate an initial buffer
    bufferStart = (char*)malloc(mallocIncrement);
    if(bufferStart == NULL)
    {
        return false;
    }

    memoryAllocationCount++;

    while(totalCharsReceived < maxBytes)
    {
        //recv into buffer
        temporaryCharsReceivedCount = recv(pvclientSocket, bufferStart + totalCharsReceived, mallocIncrement, 0);
        
        //check for recv() errors
        if(temporaryCharsReceivedCount < 1)
        {
            free(bufferStart);
            
            return false;
        }
        
        totalCharsReceived += temporaryCharsReceivedCount;
        temporaryCharsReceivedCount = 0;
        
        //check if buffer needs to be resized
        if(totalCharsReceived == mallocIncrement * memoryAllocationCount)
        {
            memoryAllocationCount++;
            
            //resize buffer to a larger size to read more chars
            temporaryPointer = (char*)realloc(bufferStart, mallocIncrement * memoryAllocationCount);
            if(temporaryPointer == NULL)
            {
                free(bufferStart);
                
                return false;
            }
            
            bufferStart = temporaryPointer;
            temporaryPointer = NULL;
        }
        else
        {   //resize buffer to a smaller size
            temporaryPointer = (char*)realloc(bufferStart, totalCharsReceived);
            if(temporaryPointer == NULL)
            {
                free(bufferStart);
                
                return false;
            }
            //successfully exit
            bufferStart = temporaryPointer;
            temporaryPointer = NULL;
            break;
        }
    }
    
    hbuf.start = bufferStart;
    hbuf.length = totalCharsReceived;
    
    return true;
}

void TtcpClient::clean()
{
    close(pvclientSocket);
}
