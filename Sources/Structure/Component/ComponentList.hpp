#pragma once

#include <unordered_map>
#include <unordered_set>
#include <UserGraph/UserGraph.hpp>
#include <Structure/Component/Component.hpp>
#include <Structure/BlockCutTree/BlockCutTree.hpp>
#include <Structure/BlockCutTree/BlockCutTreeBuilder.hpp>
#include <Structure/Vertex.hpp>
#include <Structure/Edge.hpp>
#include <Interface/Interface.hpp>
#include <Logging/Logging.hpp>

namespace gfn::structure {
/**
 * This is the class what the Graphene core actually touches.
 * Everything is stored in Vertex and Edge pointers rather than Uuids.
 * This is for faster, more versatile, and lock-free access to vertices.
 */
    class ComponentList {
        // the componentifier will build a component list based on the usergraph and properties given
        gfn::usergraph::UserGraph* usergraph;
        gfn::properties::Properties* properties;

    public:
        std::unordered_set<Vertex*> vertices;
        std::unordered_set<Edge*> edges;
        std::unordered_set<Component*> components;

        ~ComponentList() {
            for (auto& v : vertices)
                delete v;
            for (auto& e : edges)
                delete e;
            for (auto& c : components)
                delete c;
        }

        void bindSource(gfn::usergraph::UserGraph* _usergraph, gfn::properties::Properties* _properties) {
            usergraph = _usergraph;
            properties = _properties;
        }

        std::unordered_set<Component*>& getComponents() { return components; }

        /// @brief Constructs all components from UserGraph (must run in sync with core cycle)
        /// @param usergraph the source graph where the components are constructed from
        void componentify() {
            std::cerr << "componentifying\n";
            // no props validation here, as this function is the one that actually creates props
            for (auto& v : vertices)
                delete v;
            vertices.clear();
            for (auto& e : edges)
                delete e;
            edges.clear();
            for (auto& c : components)
                delete c;
            components.clear();

            // maps from uuid to vertex objects (for edge assignment)
            std::unordered_map<gfn::Uuid, Vertex*> mapping;
            // vertices that were the component root in the previous graph update
            std::unordered_set<Vertex*> previousComponentRoot;

            // initialize new vertex and edge objects
            for (auto& u : usergraph->getAdjList()) {
                // initialize vertex objects
                auto vertex = new Vertex();
                // assign the vertex a "core prop" for fast access (can not be shared by the "user prop")
                vertex->props = properties->getVertexProps(u.first).first;
                vertex->internalProps = properties->getVertexProps(u.first).second;
                vertex->props->uuid = u.first;
                vertices.insert(vertex);
                mapping.insert({vertex->props->uuid, vertex});

                // keep a list of previous component roots to later assign them back to the fresh component objects and
                // maximize the reusability (so component properties can be kept)
                // this is a fairly bad implementation, and might be changed in the future
                if (vertex->internalProps->isComponentRoot)
                    previousComponentRoot.insert(vertex);
                for (auto& v : u.second) {
                    for (auto& e : v.second) {
                        // initialize edge objects
                        auto edge = new Edge();
                        edge->props = properties->getEdgeProps(e).first;
                        edge->internalProps = properties->getEdgeProps(e).second;
                        edge->props->edgeUuid = e;
                        edge->props->startVertexUuid = u.first;
                        edge->props->endVertexUuid = v.first;
                        edges.insert(edge);
                    }
                }
            }

            // a set of vertices that are pending to get a component assigned
            std::unordered_set<Vertex*> pendingVertices(vertices.begin(), vertices.end());

            // spawn in as many component as needed
            while (!pendingVertices.empty()) {
                auto component = new Component();
                // assign the first vertex that created the component as root by default (may be overwritten in the reuse section)
                component->root = *pendingVertices.begin();
                components.insert(component);
                // recursively recreate vertex entries in component adjacency list (no edges)
                _componentifyDfs(component, (*pendingVertices.begin())->props->uuid, mapping, pendingVertices);
            }

            /// TODO: Fix up the crappy component reuse implementation

            for (auto& v : previousComponentRoot) {
                if (v->component->uuid == gfn::uuid::createNil()) {
                    v->component->uuid = v->internalProps->component;
                    v->component->root = v;
                    logMessage << "Componentifier: Assigned component uuid {" << v->internalProps->component
                               << "} from previous root vertex {" << v->props->uuid << "}";
                    logVerbose;
                } else {
                    // A component merge event
                }
            }

            for (auto& c : components) {
                if (c->uuid == gfn::uuid::createNil()) {
                    auto uuid = gfn::uuid::createUuid();
                    c->uuid = uuid;
                    c->root = c->getAdjList().begin()->first;
                    logMessage << "Componentifier: Assigned component uuid {" << uuid
                               << "} (auto-generated) and assigned vertex {"
                               << c->getAdjList().begin()->first->props->uuid << "} as root";
                    logVerbose;
                }
            }

            // construct edges for the components
            for (auto& e : edges) {
                auto u = mapping.find(e->props->startVertexUuid)->second;
                auto v = mapping.find(e->props->endVertexUuid)->second;
                if (u->component != v->component) {
                    logMessage << "Componentifier: Edge {" << e->props->edgeUuid << "} start vertex {" << u->props->uuid
                               << "} component {" << u->component->uuid << "} is different from end vertex {"
                               << v->props->uuid << "} component {" << v->component->uuid
                               << "}, something went wrong assigning components";
                    logWarning;
                }
                auto c = u->component;
                auto uIt = c->getAdjList().find(u);
                auto vIt = c->getAdjList().find(v);
                uIt->second.first.insert(v); // the connectivity-based adj list
                vIt->second.first.insert(u);
                uIt->second.second.insert({v, std::unordered_set<Edge*>()}); // the edge-based adj list
                uIt->second.second.find(v)->second.insert(e);
                c->edges.insert(e);
            }

            // finish up everything
            logMessage << "Componentifier: Component list: (" << components.size() << " entries)";
            logVerbose;
            for (auto& c : components) {
                logMessage << "     {" << c->uuid << "}";
                logVerbose;
                c->root->internalProps->componentCentroidPosition = gfn::Vec2f(0.0, 0.0);
                for (auto& v : c->getAdjList()) {
                    logMessage << "        {" << v.first->props->uuid << "}";
                    logVerbose;
                    v.first->internalProps->isComponentRoot = false;
                    v.first->internalProps->component = c->uuid;
                    c->root->internalProps->componentCentroidPosition += v.first->props->position;
                }
                c->root->internalProps->componentCentroidPosition /= double(c->getAdjList().size());
                logMessage << "       at position " << c->root->internalProps->componentCentroidPosition;
                logVerbose;
                c->root->internalProps->isComponentRoot = true;
            }

            // build block cut trees
            for (auto& c : components) {
                gfn::structure::BlockCutTreeBuilder builder(c);
                builder.build();
            }
        }

        ///@param c the component object for this whole dfs session
        ///@param uId the uuid for the currently traversed vertex
        ///@param mapping the mapping from uuid to vertex* for reading the uuid based usergraph and find the correct vertex
        /// to construct the object based component adjacency list
        ///@param pending the set of vertices who does not have a component. here it is used as a "visited" tag
        void _componentifyDfs(Component* c, const gfn::Uuid& uId, std::unordered_map<gfn::Uuid, Vertex*>& mapping,
                              std::unordered_set<Vertex*>& pending) {
            Vertex* u = mapping.find(uId)->second;
            c->getAdjList().insert({u,
                                    std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>()});
            c->vertices.insert(u);
            pending.erase(u);
            // assign the component
            u->component = c;
            u->component = c;

            auto userIt = usergraph->getAdjList().find(uId);
            for (auto& v : userIt->second) {
                if (pending.find(mapping.find(v.first)->second) != pending.end()) { // still pending
                    _componentifyDfs(c, v.first, mapping, pending);
                }
            }
        }
    };

} // namespace gfn::structure