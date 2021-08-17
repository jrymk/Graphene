#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <cmath>
#include <queue>
#include <Core/Interface/Interface.h>
#include <Core/Structure/Structure.h>
#include <Core/Objects/Timer.h>
#include <ThreadPool.h>
#include <thread>

// updater is such a bad name, I need a new one
namespace gfn {
    class Placement {

        // optimize performance by measuring four multithreading modes every 30 seconds
        int multiThreadingMode = 2;
        gfn::Timer performanceCheckTimer;
        ThreadPool pool;
        bool stablized = false;
        //nThread::CThreadPool tp{std::thread::hardware_concurrency()};

    public:
        Placement();

        void update(Interface* itf, gfn::Structure* structure);

        static gfn::Vec2 attractForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v);

        static gfn::Vec2 repelForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v);

        static gfn::Vec2 edgeAttractForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v);

        static gfn::Vec2 edgeRepelEdgeForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v);

        static gfn::Vec2 edgeRepelVertexForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v);

        static void updateVertex(gfn::Interface* itf, gfn::Component* c, gfn::Vertex* u);
        static void updateEdge(gfn::Interface* itf, gfn::Component* c, gfn::Edge* e);

        void updateComponentSingleThreaded(gfn::Interface* itf, gfn::Component* c);

        void updateComponentMultiThreaded(gfn::Interface* itf, gfn::Component* c);
    };
}

#endif