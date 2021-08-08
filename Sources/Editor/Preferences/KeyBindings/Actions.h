#ifndef GRAPHENE_ACTIONS_H
#define GRAPHENE_ACTIONS_H

namespace gfn {
    enum Actions {
        NEW_FILE = 0,
        OPEN_FILE = 5,
        SAVE_FILE = 10,
        SAVE_AS_FILE = 15,
        KEY_BINDINGS = 20,
        QUIT = 25,

        /// VIEW CONTROL
        CAMERA_PAN = 100,
        ZOOM_IN = 105,
        ZOOM_OUT = 106,

        /// SELECTION
        ADD_SELECT_SINGLE_VERTEX = 150,//
        ADD_SELECT_SINGLE_EDGE = 151,//
        ADD_SELECT_RECT_VERTEX = 152,
        ADD_SELECT_RECT_EDGE = 153,
        ADD_SELECT_LASSO_VERTEX = 154,//
        ADD_SELECT_LASSO_EDGE = 155,//
        ADD_SELECT_ALL_VERTEX = 156,//
        ADD_SELECT_ALL_EDGE = 157,//

        SUBTRACT_SELECT_SINGLE_VERTEX = 165,//
        SUBTRACT_SELECT_SINGLE_EDGE = 166,//
        SUBTRACT_SELECT_RECT_VERTEX = 167,
        SUBTRACT_SELECT_RECT_EDGE = 168,
        SUBTRACT_SELECT_LASSO_VERTEX = 169,//
        SUBTRACT_SELECT_LASSO_EDGE = 170,//
        SUBTRACT_SELECT_ALL_VERTEX = 171,//
        SUBTRACT_SELECT_ALL_EDGE = 172,//

        INV_SELECT_SINGLE_VERTEX = 180,//
        INV_SELECT_SINGLE_EDGE = 181,//
        INV_SELECT_RECT_VERTEX = 182,
        INV_SELECT_RECT_EDGE = 183,
        INV_SELECT_LASSO_VERTEX = 184,//
        INV_SELECT_LASSO_EDGE = 185,//
        INV_SELECT_ALL_VERTEX = 186,//
        INV_SELECT_ALL_EDGE = 187,//

        /// GRAPH MANIPULATION (STRUCTURE)
        ADD_VERTEX_PREVIEW = 200,
        ADD_VERTEX = 205,
        ADD_EDGE_PREVIEW = 210,
        ADD_EDGE = 215,

        DELETE_HOVERED_VERTEX = 220,
        DELETE_VERTICES = 221,
        DELETE_HOVERED_EDGE = 225,
        DELETE_EDGES = 226,

        /// GRAPH MANIPULATION (PROPERTIES)
        MOVE_VERTEX = 250,
        MOVE_SELECTION = 251,
    };
}

#endif
