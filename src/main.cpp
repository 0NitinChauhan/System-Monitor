#include "ncurses_display.h"
#include "system.h"

int main() {
    std::cout << "Entered main" << std::endl;
    System system;
    NCursesDisplay::Display(system);
}