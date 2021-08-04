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
            "Clear selection (no hovered item)",
            "Clear selection (hovered on unselected item)",
            "Clear selection (hovered on selected item)",
            "", "",
            // 25
            "", "", "", "", "",
            // 30
            "", "", "", "", "",
            // 35
            "Begin selection single item (hovered on unselected item)",
            "Begin selection with rectangular selection tool (no hovered item)",
            "Begin selection with rectangular selection tool (hovered on unselected item)",
            "Begin selection with rectangular selection tool (hovered on selected item)",
            "Begin selection with lasso selection tool (no hovered item)",
            // 40
            "Begin selection with lasso selection tool (hovered on unselected item)",
            "Begin selection with lasso selection tool (hovered on selected item)",
            "Begin selection all items (no hovered item)",
            "Begin selection all items (hovered on unselected item)",
            "Begin selection all items (hovered on selected item)",
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
            "", "", "",
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
            "", "", "",
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
        CAMERA_PAN_E = 0,
        CAMERA_PAN_U = 1,
        CAMERA_PAN_S = 2,
        ZOOM_IN_E = 5,
        ZOOM_IN_U = 6,
        ZOOM_IN_S = 7,
        ZOOM_OUT_E = 8,
        ZOOM_OUT_U = 9,
        ZOOM_OUT_S = 10,

        CLEAR_SELECTION_E = 20,//
        CLEAR_SELECTION_U = 21,//
        CLEAR_SELECTION_S = 22,//
        SELECT_SINGLE_U = 35,
        BEGIN_SELECT_RECT_E = 36,
        BEGIN_SELECT_RECT_U = 37,
        SELECT_RECT_S = 38,
        BEGIN_SELECT_LASSO_E = 39,//
        BEGIN_SELECT_LASSO_U = 40,//
        BEGIN_SELECT_LASSO_S = 41,//
        BEGIN_SELECT_ALL_E = 42,
        BEGIN_SELECT_ALL_U = 43,
        BEGIN_SELECT_ALL_S = 44,
        ADD_SELECT_SINGLE_U = 50,
        ADD_SELECT_RECT_E = 51,
        ADD_SELECT_RECT_U = 52,
        ADD_SELECT_RECT_S = 53,
        ADD_SELECT_LASSO_E = 54,//
        ADD_SELECT_LASSO_U = 55,//
        ADD_SELECT_LASSO_S = 56,//
        SUBTRACT_SELECT_SINGLE_S = 65,
        SUBTRACT_SELECT_RECT_E = 66,
        SUBTRACT_SELECT_RECT_U = 67,
        SUBTRACT_SELECT_RECT_S = 68,
        SUBTRACT_SELECT_LASSO_E = 69,//
        SUBTRACT_SELECT_LASSO_U = 70,//
        SUBTRACT_SELECT_LASSO_S = 71,//
        INV_SELECT_SINGLE_U = 80,
        INV_SELECT_SINGLE_S = 81,
        INV_SELECT_RECT_E = 82,
        INV_SELECT_RECT_U = 83,
        INV_SELECT_RECT_S = 84,
        INV_SELECT_LASSO_E = 85,//
        INV_SELECT_LASSO_U = 86,//
        INV_SELECT_LASSO_S = 87,//
        INV_SELECT_ALL_E = 88,
        INV_SELECT_ALL_U = 89,
        INV_SELECT_ALL_S = 90,

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