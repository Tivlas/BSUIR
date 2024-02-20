#include "SS.h"
#include "ports.h"

int main() {
    io_context io_context;
    SS s(io_context, ports::SS_PORT);
    io_context.run();
    return 0;
}