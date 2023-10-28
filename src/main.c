#include <stdlib.h>
#include <time.h>

#include "ui.h"

int main() {
    srand(time(NULL));
    struct UIState ui_state;
    struct Map map;
    init(&ui_state);

    bool running = true;
    uint8_t x, y;
    enum EventStatus events[8] = {};

    while (running) {
        update_button_pos(&ui_state.play_again_btn, ui_state.win, -1, -1);
        handle_events(&ui_state, &running, &map);
        render(ui_state, map);
    }

    cleanup(ui_state);
}
