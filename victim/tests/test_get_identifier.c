#include <stdio.h>
#include <stdlib.h>

#include "../benchmark.h"

int main(int argc, char const *argv[])
{
    unsigned char* p_macAddress = get_identifier();

    printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
        p_macAddress[0], p_macAddress[1], p_macAddress[2], p_macAddress[3], p_macAddress[4], p_macAddress[5]);

    free(p_macAddress);

    return 0;
}
