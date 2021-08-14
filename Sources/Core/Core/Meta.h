#ifndef META_H
#define META_H

#define BUILD_MAJOR 0
#define BUILD_MINOR 4
#define BUILD_PATCH 1

const char buildVersion[] = {
        'G', 'r', 'a', 'p', 'h', 'e', 'n', 'e', ' ',
        '[', 'V', 'e', 'r', 's', 'i', 'o', 'n', ':', ' ',
        '0' + BUILD_MAJOR, '.', '0' + BUILD_MINOR, '.', '0' + BUILD_PATCH, ' ',
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
// fixed multiple selection problems with window focusing
// added pause
// added pause ui, added edge label, fixed properties panel
// prototype multiedge rendering
// added tikz picture export
// added edge arrows support
// added preferences window
// added zoom to fit
// added export window
// added very bad self cycle rendering
// added svg export
// added png export
// added copy and paste in string format

#endif