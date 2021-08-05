#include "Structure.h"
#include <Structure/BlockCutTree/BlockCutTreeBuilder.hpp>

using namespace gfn;

Structure::Structure(gfn::Interface* _itf) :
        itf(_itf) {
    graph = &itf->graph.getWrite();
}

Structure::~Structure() {
    for (auto& v : vertices)
        delete v;
    for (auto& e : edges)
        delete e;
    for (auto& c : components)
        delete c;
}

std::unordered_set<Component*>& Structure::getComponents() {
    return components;
}

void Structure::componentify() {
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
    for (auto& u : graph->getAdjList()) {
        // initialize vertex objects
        auto vertex = new Vertex();
        // assign the vertex a "core prop" for fast access (can not be shared by the "user prop")
        vertex->props = graph->props.getVertexProps(u.first);
        vertex->props->uuid.value = u.first;
        vertices.insert(vertex);
        mapping.insert({vertex->props->uuid.value, vertex});

        // keep a list of previous component roots to later assign them back to the fresh component objects and
        // maximize the reusability (so component properties can be kept)
        // this is a fairly bad implementation, and might be changed in the future
        if (vertex->props->isComponentRoot.value)
            previousComponentRoot.insert(vertex);
        for (auto& v : u.second) {
            for (auto& e : v.second) {
                // initialize edge objects
                auto edge = new Edge();
                edge->props = graph->props.getEdgeProps(e);
                edge->props->edgeUuid.value = e;
                edge->props->startVertexUuid.value = u.first;
                edge->props->endVertexUuid.value = v.first;
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
        _componentifyDfs(component, (*pendingVertices.begin())->props->uuid.value, mapping,
                         pendingVertices);
    }

    /// TODO: Fix up the crappy component reuse implementation

    for (auto& v : previousComponentRoot) {
        if (v->component->uuid == gfn::createNil()) {
            v->component->uuid = v->props->component.value;
            v->component->root = v;
        } else {
            // A component merge event
        }
    }

    for (auto& c : components) {
        if (c->uuid == gfn::createNil()) {
            auto uuid = gfn::createUuid();
            c->uuid = uuid;
            c->root = c->getAdjList().begin()->first;
        }
    }

    // construct edges for the components
    for (auto& e : edges) {
        auto u = mapping.find(e->props->startVertexUuid.value)->second;
        auto v = mapping.find(e->props->endVertexUuid.value)->second;
        if (u->component != v->component) {
        }
        auto c = u->component;
        auto uIt = c->getAdjList().find(u);
        auto vIt = c->getAdjList().find(v);
        uIt->second.first.insert(v); // the connectivity-based adj list
        vIt->second.first.insert(u);
        uIt->second.second.insert({v, std::unordered_set<Edge*>()}); // the edge-based adj list
        uIt->second.second.find(v)->second.insert(e);
        e->startVertex = u;
        e->endVertex = v;
        c->edges.insert(e);
    }

    // finish up everything
    for (auto& c : components) {
        for (auto& v : c->getAdjList()) {
            v.first->props->isComponentRoot.value = false;
            v.first->props->component.value = c->uuid;
        }
        c->root->props->isComponentRoot.value = true;
    }

    // build block cut trees
    for (auto& c : components) {
        gfn::structure::BlockCutTreeBuilder builder(c);
        builder.build();
    }
}

void Structure::_componentifyDfs(Component* c, const gfn::Uuid& uId, std::unordered_map<gfn::Uuid, Vertex*>& mapping,
                                 std::unordered_set<Vertex*>& pending) {
    Vertex* u = mapping.find(uId)->second;
    c->getAdjList().insert({u,
                            std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>()});
    c->vertices.insert(u);
    pending.erase(u);
    // assign the component
    u->component = c;

    auto userIt = graph->getAdjList().find(uId);
    for (auto& v : userIt->second) {
        if (pending.find(mapping.find(v.first)->second) != pending.end()) { // still pending
            _componentifyDfs(c, v.first, mapping, pending);
        }
    }
}
