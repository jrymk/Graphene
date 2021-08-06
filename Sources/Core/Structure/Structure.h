#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <Core/Graph/Graph.h>
#include <Core/Structure/Component.h>
#include <Core/Interface/Interface.h>

namespace gfn {
/// This is the class what the Graphene core actually touches.
/// Everything is stored in Vertex and Edge pointers rather than Uuids.
/// This is for faster, more versatile, and lock-free access to vertices.
    class Structure {
    private:
        gfn::Interface* itf;
        gfn::Graph* graph;

    public:
        std::unordered_set<Vertex*> vertices;
        std::unordered_set<Edge*> edges;
        std::unordered_set<Component*> components;

        Structure(gfn::Interface* _itf);

        ~Structure();

        std::unordered_set<Component*>& getComponents();

        /// @brief Constructs all components from UserGraph (must run in sync with core cycle)
        /// @param usergraph the source graph where the components are constructed from
        void componentify();

        ///@param c the component object for this whole dfs session
        ///@param uId the uuid for the currently traversed vertex
        ///@param mapping the mapping from uuid to vertex* for reading the uuid based usergraph and find the correct vertex
        /// to construct the object based component adjacency list
        ///@param pending the set of vertices who does not have a component. here it is used as a "visited" tag
        void _componentifyDfs(Component* c,
                              const gfn::Uuid& uId,
                              std::unordered_map<gfn::Uuid, Vertex*>& mapping,
                              std::unordered_set<Vertex*>& pending);
    };
}

#endif