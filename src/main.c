#include <stdlib.h>
#include <time.h>

#include "ui.h"

int main() {
    srand(time(NULL));
    struct SDLState state;
    struct Map map = create_map(30, 16, 99);
    init(&state);

    bool running = true;
    bool flag = false;
    uint8_t x, y;
    enum EventStatus events[8] = {0};

    while (running) {
        for (uint8_t i = 0; i < handle_events(flag, events, &x, &y); i++) {
            switch (events[i]) {
            case E_NONE: break;
            case E_QUIT: running = false; break;
            case E_FLAG_OFF: flag = false; break;
            case E_FLAG_ON: flag = true; break;
            case E_FLAG:  // TODO: flag
                break;
            case E_MINE:  // TODO: mine
                break;
            }
        }

        render(state, map);
    }

    cleanup(state);
}
