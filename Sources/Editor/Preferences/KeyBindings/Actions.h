#ifndef GRAPHENE_ACTIONS_H
#define GRAPHENE_ACTIONS_H

namespace gfn {
    enum Actions {
        /// VIEW CONTROL
        CAMERA_PAN_E = 0,
        CAMERA_PAN_U = 1,
        CAMERA_PAN_S = 2,
        ZOOM_IN_E = 5,
        ZOOM_IN_U = 6,
        ZOOM_IN_S = 7,
        ZOOM_OUT_E = 8,
        ZOOM_OUT_U = 9,
        ZOOM_OUT_S = 10,

        /// SELECTION
        ADD_SELECT_SINGLE_U = 50,//
        ADD_SELECT_RECT_E = 51,
        ADD_SELECT_RECT_U = 52,
        ADD_SELECT_RECT_S = 53,
        ADD_SELECT_LASSO_E = 54,//
        ADD_SELECT_LASSO_U = 55,//
        ADD_SELECT_LASSO_S = 56,//
        ADD_SELECT_ALL_E = 57,//
        ADD_SELECT_ALL_U = 58,//
        ADD_SELECT_ALL_S = 59,//
        SUBTRACT_SELECT_SINGLE_S = 65,//
        SUBTRACT_SELECT_RECT_E = 66,
        SUBTRACT_SELECT_RECT_U = 67,
        SUBTRACT_SELECT_RECT_S = 68,
        SUBTRACT_SELECT_LASSO_E = 69,//
        SUBTRACT_SELECT_LASSO_U = 70,//
        SUBTRACT_SELECT_LASSO_S = 71,//
        SUBTRACT_SELECT_ALL_E = 72,//
        SUBTRACT_SELECT_ALL_U = 73,//
        SUBTRACT_SELECT_ALL_S = 74,//
        INV_SELECT_SINGLE_U = 80,//
        INV_SELECT_SINGLE_S = 81,//
        INV_SELECT_RECT_E = 82,
        INV_SELECT_RECT_U = 83,
        INV_SELECT_RECT_S = 84,
        INV_SELECT_LASSO_E = 85,//
        INV_SELECT_LASSO_U = 86,//
        INV_SELECT_LASSO_S = 87,//
        INV_SELECT_ALL_E = 88,//
        INV_SELECT_ALL_U = 89,//
        INV_SELECT_ALL_S = 90,//

        /// GRAPH MANIPULATION (STRUCTURE)
        ADD_VERTEX_PREV_E = 100,
        ADD_VERTEX_PREV_U = 101,
        ADD_VERTEX_PREV_S = 102,
        ADD_VERTEX_E = 105,
        ADD_VERTEX_U = 106,
        ADD_VERTEX_S = 107,
        ADD_EDGE_PREV_E = 110,
        ADD_EDGE_PREV_U = 111,
        ADD_EDGE_PREV_S = 112,
        ADD_EDGE_E = 115,
        ADD_EDGE_U = 116,
        ADD_EDGE_S = 117,
    };
}

#endif
