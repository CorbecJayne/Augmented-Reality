
#include "Player.h"

void Player::add_points(int amount) {
    points+=amount;
}

void Player::set_time(time_t t) {
    time=t;
}

time_t Player::getTime() const {
    return time;
}

int Player::getArea() const {
    return area;
}
