#include <libdragon.h>

void function(void);

int main()
{
    debug_init_isviewer();
    while (true)
    {
        function();
        wait_ms(1);
    }
}
