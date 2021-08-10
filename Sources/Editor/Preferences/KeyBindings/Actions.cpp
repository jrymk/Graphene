#include "KeyBindings.h"
#include <vector>

namespace gfn {
    int Bindings::nonCondActionStartsAt = 100;

    std::vector<std::pair<std::string, int>> Bindings::actions = {
            // 0
            {"New document",                                                     0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 5
            {"Open file",                                                        0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 10
            {"Save file",                                                        0},
            {"Save as file",                                                     0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 15
            {"Close document",                                                   0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 20
            {"Key bindings",                                                     0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 25
            {"Quit",                                                             0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 30
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 35
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 40
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 45
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 50
            {"Copy selection",                                                   0},
            {"Cut selection",                                                    0},
            {"Paste selection",                                                  0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 55
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 60
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 65
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 70
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 75
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 80
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 85
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 90
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 95
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},


            // 100
            {"Camera pan",                                                       0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 105
            {"Zoom in",                                                          0},
            {"Zoom out",                                                         0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 110
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 115
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 120
            {"Hold to pause graph updates",                                      0},
            {"Toggle graph updates",                                             0},
            {"Hold to pause graph streaming",                                    0},
            {"Toggle graph streaming",                                           0},
            {"",                                                                 0},
            // 125
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 130
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 135
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 140
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 145
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 150
            {"Add vertex to selection",                                          0},
            {"Add edge to selection",                                            0},
            {"Add vertices to selection with rectangular selection tool",        0},
            {"Add edges to selection with rectangular selection tool",           0},
            {"Add vertices to selection with lasso selection tool",              0},
            // 155
            {"Add edges to selection with lasso selection tool",                 0},
            {"Add all vertices to selection",                                    0},
            {"Add all edges to selection",                                       0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 160
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 165
            {"Subtract vertex from selection",                                   0},
            {"Subtract edge from selection",                                     0},
            {"Subtract vertices from selection with rectangular selection tool", 0},
            {"Subtract edges from selection with rectangular selection tool",    0},
            {"Subtract vertices from selection with lasso selection tool",       0},
            // 170
            {"Subtract edges from selection with lasso selection tool",          0},
            {"Subtract all vertices from selection",                             0},
            {"Subtract all edges from selection",                                0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 175
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 180
            {"Invert selection of vertex",                                       0},
            {"Invert selection of edge",                                         0},
            {"Invert selection of vertices with rectangular selection tool",     0},
            {"Invert selection of edges with rectangular selection tool",        0},
            {"Invert selection of vertices with lasso selection tool",           0},
            // 185
            {"Invert selection of edges with lasso selection tool",              0},
            {"Invert selection of all vertices",                                 0},
            {"Invert selection of all edges",                                    0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 190
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 195
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 200
            {"Preview add vertex",                                               0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 205
            {"Add vertex",                                                       0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 210
            {"Preview add edge",                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 215
            {"Add edge",                                                         0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 220
            {"Delete hovered vertex",                                            0},
            {"Delete selected vertices",                                         0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 225
            {"Delete hovered edge",                                              0},
            {"Delete selected edges",                                            0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 230
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 235
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 240
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 245
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},
            // 250
            {"Move hovered vertex",                                              0},
            {"Move selected vertices",                                           0},
            {"",                                                                 0},
            {"",                                                                 0},
            {"",                                                                 0},

    };
}