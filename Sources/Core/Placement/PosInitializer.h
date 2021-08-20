#ifndef GRAPHENE_POSINITIALIZER_H
#define GRAPHENE_POSINITIALIZER_H

#include <unordered_map>
#include <Core/Structure/Structure.h>

namespace gfn {

    class ComponentPosition {
    public:
        Component* component;
        double diameter;
        std::unordered_map<Vertex*, Vec2> pos;
    };

    class ComponentInitializer {

    private:

        Component* component;
        ComponentPosition* position;

        std::unordered_map<BiconnectedComponent*, double> fromAngle;
        std::unordered_map<BiconnectedComponent*, double> angleSize;
        std::unordered_map<BiconnectedComponent*, double> width;
        std::vector<std::vector<BiconnectedComponent*>> depth;

        std::unordered_map<BiconnectedComponent*, double> subtreeDepth;
        std::unordered_map<BiconnectedComponent*, double> upDepth;
        std::unordered_map<BiconnectedComponent*, double> rootDepth;
        BiconnectedComponent* root = nullptr;

        double getWidth(BiconnectedComponent* b);

        void dfs1(BiconnectedComponent* now, BiconnectedComponent* parent);

        void dfs2(BiconnectedComponent* now, BiconnectedComponent* parent, int d);

        double initDepth(int d, double radius);

        void findRoot1(BiconnectedComponent* now, BiconnectedComponent* parent);

        void findRoot2(BiconnectedComponent* now, BiconnectedComponent* parent);

    public:

        explicit ComponentInitializer(Component* c);

        ComponentPosition* init();

    };

    class PosInitializer {
    private:
        Structure* structure;
        bool maintainCentroid;
        void putComponent(ComponentPosition* c, Vec2 pos);

    public:
        PosInitializer(Structure* structure, bool maintainCentroid = true);

        void init();

    };

}

#endif //GRAPHENE_POSINITIALIZER_H
