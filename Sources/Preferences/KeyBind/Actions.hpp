#pragma once

#include <vector>

namespace gfn::keybind {
    std::vector<std::string> actions = {
            // 0
            "Camera pan (no hovered item)",
            "Camera pan (hovered on unselected item)",
            "Camera pan (hovered on selected item)",
            "", "",
            // 5
            "Zoom in (no hovered item)",
            "Zoom in (hovered on unselected item)",
            "Zoom in (hovered on selected item)",
            "Zoom out (no hovered item)",
            "Zoom out (hovered on unselected item)",
            // 10
            "Zoom out (hovered on selected item)",
            "", "", "", "",
            // 15
            "", "", "", "", "",
            // 20
            "", "", "", "", "",
            // 25
            "", "", "", "", "",
            // 30
            "", "", "", "", "",
            // 35
            "", "", "", "", "",
            // 40
            "", "", "", "", "",
            // 45
            "", "", "", "", "",
            // 50
            "Add single item to selection (hovered on unselected item)",
            "Add item to selection with rectangular selection tool (no hovered item)",
            "Add item to selection with rectangular selection tool (hovered on unselected item)",
            "Add item to selection with rectangular selection tool (hovered on selected item)",
            "Add item to selection with lasso selection tool (no hovered item)",
            // 55
            "Add item to selection with lasso selection tool (hovered on unselected item)",
            "Add item to selection with lasso selection tool (hovered on selected item)",
            "Add all items to selection (no hovered item)",
            "Add all items to selection (hovered on unselected item)",
            "Add all items to selection (hovered on selected item)",
            // 60
            "", "", "", "", "",
            // 65
            "Subtract single item from selection (hovered on selected item)",
            "Subtract item from selection with rectangular selection tool (no hovered item)",
            "Subtract item from selection with rectangular selection tool (hovered on unselected item)",
            "Subtract item from selection with rectangular selection tool (hovered on selected item)",
            "Subtract item from selection with lasso selection tool (no hovered item)",
            // 70
            "Subtract item from selection with lasso selection tool (hovered on unselected item)",
            "Subtract item from selection with lasso selection tool (hovered on selected item)",
            "Subtract all items from selection (no hovered item)",
            "Subtract all items from selection (hovered on unselected item)",
            "Subtract all items from selection (hovered on selected item)",
            // 75
            "", "", "", "", "",
            // 80
            "Invert selection of single item (hovered on unselected item)",
            "Invert selection of single item (hovered on selected item)",
            "Invert selection with rectangular selection tool (no hovered item)",
            "Invert selection with rectangular selection tool (hovered on unselected item)",
            "Invert selection with rectangular selection tool (hovered on selected item)",
            // 85
            "Invert selection with lasso selection tool (no hovered item)",
            "Invert selection with lasso selection tool (hovered on unselected item)",
            "Invert selection with lasso selection tool (hovered on selected item)",
            "Invert selection of all items (no hovered item)",
            "Invert selection of all items (hovered on unselected item)",
            // 90
            "Invert selection of all items (hovered on selected item)",
            "", "", "", "",
            // 95
            "", "", "", "", "",
            // 100
            "Preview add vertex (no hovered item)",
            "Preview add vertex (hovered on unselected item)",
            "Preview add vertex (hovered on selected item)",
            "", "",
            // 105
            "Add vertex (no hovered item)",
            "Add vertex (hovered on unselected item)",
            "Add vertex (hovered on selected item)",
            "", "",
            // 110
            "Preview add edge (no hovered item)",
            "Preview add edge (hovered on unselected item)",
            "Preview add edge (hovered on selected item)",
            "", "",
            // 115
            "Add edge (no hovered item)",
            "Add edge (hovered on unselected item)",
            "Add edge (hovered on selected item)",
            "", "",
            // 120

    };

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