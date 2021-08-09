#ifndef META_H
#define META_H

#define BUILD_MAJOR 0
#define BUILD_MIDDLE 2
#define BUILD_MINOR 4

const char buildVersion[] = {
        'G', 'r', 'a', 'p', 'h', 'e', 'n', 'e', ' ',
        '[', 'V', 'e', 'r', 's', 'i', 'o', 'n', ':', ' ',
        '0' + BUILD_MAJOR, '.', '0' + BUILD_MIDDLE, '.', '0' + BUILD_MINOR, ' ',
        __DATE__[0], __DATE__[1], __DATE__[2], __DATE__[3],
        (__DATE__[4] == ' ' ? '0' : __DATE__[4]), __DATE__[5], __DATE__[6], __DATE__[7],
        __DATE__[8], __DATE__[9], __DATE__[10],
        ' ',
        __TIME__[0], __TIME__[1], __TIME__[2], __TIME__[3],
        __TIME__[4], __TIME__[5], __TIME__[6], __TIME__[7], ']', '\0'
};

// more responsive move selection and config changing with echo -off
// fully working properties panel (functional)
// added delete (finally)
// added temporary reset and aggressive c4
// added file hotkeys
// fix octet nonsense
// Updated selection, refreshed properties

#endif