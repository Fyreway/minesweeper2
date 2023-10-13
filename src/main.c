#include <stdlib.h>
#include <time.h>

#include "ui.h"

int main() {
    srand(time(NULL));
    struct State state;
    struct Map map = create_map(30, 16, 99);
    init(&state);

    bool running = true;
    uint8_t x, y;
    enum EventStatus events[8] = {};

    while (running) {
        update_button_pos(&state.play_again_btn, state.win, -1, -1);
        handle_events(state, &running, &map);
        render(state, map);
    }

    cleanup(state);
}
