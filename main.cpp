#include <iostream>

#include "controller.h"

using namespace spel;

int main() {
    Controller ctrl;
    ctrl.create_default_world();
//     ctrl.save_game("default.game");
    ctrl.load_game("default.game");
    return ctrl.run_game();
}
