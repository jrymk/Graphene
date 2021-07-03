#pragma once

namespace Constants {

    // attract force = c1 * log(dis / c2)
    // repel force = -c3 / dis / dis
    // force move = c4 * f
    // torque move = c5 * t (rad)
    // attract force from origin = min(c6, dis)

    float c1 = 2;
    float c2 = 1;
    float c3 = 1;
    float c4 = 0.1;

    float c5 = 0.1;
    float c6 = 0.001;

    bool forceBetweenComponents = false;

}
