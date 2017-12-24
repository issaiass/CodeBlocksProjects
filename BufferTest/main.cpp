#include <iostream>
#include <stdlib.h>
#include "FIFO.h"

using namespace std;

FIFO accBuf;

int main()
{
for(int i = 0; i < 10; i++) {
    accBuf.RxPutChar(rand()%65535);
    accBuf.RxPutChar(rand()%65535);
    accBuf.RxPutChar(rand()%65535);
}

    while (!accBuf.RxBufEmpty()) {
        INT16S data = accBuf.RxGetChar();
        cout << data << endl;
    }

    cout << "Hello world!" << endl;
    return 0;
}
