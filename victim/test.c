#include <unistd.h>

#include "connection.h"

int main(int argc, char const *argv[])
{
    /* code */
    get_encryption_key("test");
    get_decryption_key("test");
    return 0;
}
