#include <stdlib.h>
#include <time.h>

#include "ui.h"

int main() {
    srand(time(NULL));
    struct SDLState state;
    struct Map map = create_map(30, 16, 99);
    init(&state);

    bool running = true;
    uint8_t x, y;
    enum EventStatus events[8] = {};

    while (running) {
        handle_events(&running, &map);
        render(state, map);
    }

    cleanup(state);
}
