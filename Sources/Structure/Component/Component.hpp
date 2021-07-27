#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Structure/Vertex.hpp>
#include <Structure/Edge.hpp>
#include <Structure/BlockCutTree/BlockCutTree.hpp>
#include <Properties/Properties.hpp>

namespace gfn::structure {
/// @brief The Component class is the biggest unit for graph calculations, stores both directional-edge-based adj list
/// (same as usergraph) and connectivity-based adj list
    class Component {
    private:
        std::unordered_map<Vertex*,
                std::pair<std::unordered_set<Vertex*>,                            // connectivity-based
                        std::unordered_map<Vertex*, std::unordered_set<Edge*>> // directional-edge-based
                >>
                adjList;
        gfn::structure::BlockCutTree blockCutTree;

    public:
        gfn::Uuid uuid;

        Vertex* root;

        Component() { uuid = gfn::uuid::createNil(); }

        std::unordered_map<Vertex*,
                std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>>&
        getAdjList() {
            return adjList;
        }

        /// @brief get all vertices that are adjacent to the given vertex the component (non directional)
        /// @returns a set of all the adjacent vertices
        std::unordered_set<Vertex*> getAdjacentVertices(Vertex* v) {
            auto vIt = adjList.find(v);
            if (vIt == adjList.end()) {
                logMessage << "Component: Request to get adjacent vertices for vertex {" << v->props->uuid
                           << "} in component {"
                           << uuid << "} failed (vertex does not exist in component)";
                logWarning;
                return std::unordered_set<Vertex*>();
            }
            return vIt->second.first;
        }

        /// @brief check if vertices are adjacent to each other in the component (non directional)
        bool isAdjacent(Vertex* u, Vertex* v) {
            auto uIt = adjList.find(u);
            if (uIt == adjList.end()) {
                logMessage << "Component: Request to check adjacentivity for vertex {" << u->props->uuid << "} and {"
                           << v->props->uuid
                           << "} in component {" << uuid << "} failed (start vertex does not exist in component)";
                logWarning;
                return false;
            }
            return uIt->second.first.find(v) != uIt->second.first.end();
        }

        gfn::structure::BlockCutTree& getBlockCutTree() { return blockCutTree; }
    };
} // namespace gfn::structure