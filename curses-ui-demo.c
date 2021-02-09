#include <ncurses.h>
#include <locale.h>
#include <unistd.h>

#define N_BANKS 7
// banks is 4, 7 cells per bank
#define BAT_W 7
#define BAT_H 7

#define CHARGE_COLOR_PAIR 1
#define DISCHARGE_COLOR_PAIR 2

void init_curses() {
    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);
    noecho();
    cbreak();

    start_color();
    init_pair(CHARGE_COLOR_PAIR, COLOR_GREEN, 0);
    init_pair(DISCHARGE_COLOR_PAIR, COLOR_RED, 0);

    refresh();
}

float fake_status[N_BANKS] = { 0 };
float charge_status(int bank) {
    fake_status[bank] += (2 * bank) + bank / (N_BANKS + 0.1);
    if (fake_status[bank] > 99) {
        fake_status[bank] -= 200;
    }
    return fake_status[bank];
}

void update_banks(WINDOW** banks) {
    for (int i = 0; i < N_BANKS; i++) {
        banks[i] = newwin(BAT_H, BAT_W, 1, 1 + i * (BAT_W + 3));
        box(banks[i], 0, 0);
        mvwprintw(banks[i], (BAT_H / 2), (BAT_W / 2), "%i", i + 1);

        float status = charge_status(i);
        if (status > 0) { // Charging
            wattron(banks[i], COLOR_PAIR(CHARGE_COLOR_PAIR));
            mvwprintw(banks[i], (BAT_H / 2) - 1, (BAT_W / 2), "▲");
            mvwprintw(banks[i], (BAT_H / 2) - 2, 1, "%05.2f", status);
            wattroff(banks[i], COLOR_PAIR(CHARGE_COLOR_PAIR));
        }
        else if (status < 0) { // Discharging
            wattron(banks[i], COLOR_PAIR(DISCHARGE_COLOR_PAIR));
            mvwprintw(banks[i], (BAT_H / 2) + 1, (BAT_W / 2), "▼");
            mvwprintw(banks[i], (BAT_H / 2) + 2, 1, "%05.2f", status * -1);
            wattroff(banks[i], COLOR_PAIR(DISCHARGE_COLOR_PAIR));
        }

        wrefresh(banks[i]);
    }
}

int main(void) {
    init_curses();
    WINDOW* banks[N_BANKS];

    while (true) {
        update_banks(banks);
        usleep(500000);
    }

    getch();
    endwin();
    return 0;
}
