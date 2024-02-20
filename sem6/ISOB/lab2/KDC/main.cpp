#include "AS.h"
#include "TGS.h"
#include "ports.h"

int main() {
    io_context io_context;
    TGS tgs(io_context, ports::TGS_PORT);
    AS as(io_context, ports::AS_PORT);
    io_context.run();
}