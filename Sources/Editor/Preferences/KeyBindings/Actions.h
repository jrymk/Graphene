#ifndef GRAPHENE_ACTIONS_H
#define GRAPHENE_ACTIONS_H

namespace gfn {
    enum Actions {
        /// VIEW CONTROL
        CAMERA_PAN = 0,
        ZOOM_IN = 5,
        ZOOM_OUT = 6,

        /// SELECTION
        ADD_SELECT_SINGLE_VERTEX = 50,//
        ADD_SELECT_SINGLE_EDGE = 51,//
        ADD_SELECT_RECT_VERTEX = 52,
        ADD_SELECT_RECT_EDGE = 53,
        ADD_SELECT_LASSO_VERTEX = 54,//
        ADD_SELECT_LASSO_EDGE = 55,//
        ADD_SELECT_ALL_VERTEX = 56,//
        ADD_SELECT_ALL_EDGE = 57,//

        SUBTRACT_SELECT_SINGLE_VERTEX = 65,//
        SUBTRACT_SELECT_SINGLE_EDGE = 66,//
        SUBTRACT_SELECT_RECT_VERTEX = 67,
        SUBTRACT_SELECT_RECT_EDGE = 68,
        SUBTRACT_SELECT_LASSO_VERTEX = 69,//
        SUBTRACT_SELECT_LASSO_EDGE = 70,//
        SUBTRACT_SELECT_ALL_VERTEX = 71,//
        SUBTRACT_SELECT_ALL_EDGE = 72,//

        INV_SELECT_SINGLE_VERTEX = 80,//
        INV_SELECT_SINGLE_EDGE = 81,//
        INV_SELECT_RECT_VERTEX = 82,
        INV_SELECT_RECT_EDGE = 83,
        INV_SELECT_LASSO_VERTEX = 84,//
        INV_SELECT_LASSO_EDGE = 85,//
        INV_SELECT_ALL_VERTEX = 86,//
        INV_SELECT_ALL_EDGE = 87,//

        /// GRAPH MANIPULATION (STRUCTURE)
        ADD_VERTEX_PREVIEW = 100,
        ADD_VERTEX = 105,

        ADD_EDGE_PREVIEW = 110,
        ADD_EDGE = 115,
    };
}

#endif
