#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Objects/Uuid.hpp>
#include <Structure/Vertex.hpp>
#include <Structure/Edge.hpp>
#include <Properties/Properties.hpp>

namespace gfn::structure {
/**
 * A biconnected component consists of several vertices including articulation, but articulation will be an
 * independent object here, and the remaining parts will be in the same object with the type BLOCK.
 *
 * By definition, adjacent articulations share a biconnected component without non-articulation vertices, and the block
 * will have no vertices, so note that 'vertices' may be empty.
 */
    class BiconnectedComponent {
    public:
        gfn::Uuid uuid;

        enum Type {
            BLOCK,
            ARTICULATION,
        };

        BiconnectedComponent(Type type) : type(type) {}

    private:
        Type type;
        // the vertices within this biconnected component (the vertices in the biconnected component defined above)
        std::unordered_set<Vertex*> vertices;
        // the vertices within this biconnected component AND adjacent ARTICULATIONs (actual biconnected component)
        std::unordered_set<Vertex*> blockVerticesIncludingArticulation;

    public:
        Type getType() { return type; }

        std::unordered_set<Vertex*>& getVertices() { return vertices; }

        std::unordered_set<Vertex*>&
        getBlockVerticesIncludingArticulation() { return blockVerticesIncludingArticulation; }
    };
} // namespace gfn::core::structure