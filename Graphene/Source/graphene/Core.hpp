#pragma once
#include <windows.h>
#include <cmath>
#include <iostream>
#include <random>
#include <map>
#include <iomanip>
#include "Graph.hpp"
#include "Structure.hpp"
#include "GraphIter.hpp"

namespace Graphene {

	class Core {

	private:
		Graph* m_graph;

	public:

		Core(Graph& graph) {
			m_graph = &graph;
		}

		float m_c1 = 2;
		float m_c2 = 1;
		float m_c3 = 1;
		float m_c4 = 0.1;

		bool pendingInputUpdate = false;

		Vertex* grabbingVertex = nullptr;

		Graph* boundGraph() {
			return m_graph;
		}


		float distance(Vertex* u, Vertex* v) {
			float dx = u->getCoord().x - v->getCoord().x;
			float dy = u->getCoord().y - v->getCoord().y;
			return sqrt(dx * dx + dy * dy);
		}

		float length(Vec2f a) {
			return sqrt(a.x * a.x + a.y * a.y);
		}

		Vec2f attractForce(Vertex* u, Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Vec2f(0, 0);
			float coeff = m_c1 * log(dis / m_c2);
			return (v->getCoord() - u->getCoord()).normalize() * coeff;
		}

		Vec2f repelForce(Vertex* u, Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Vec2f(0, 0);
			float coeff = -m_c3 / dis / dis;
			return (v->getCoord() - u->getCoord()).normalize() * coeff;
		}

        Vec2f originAttractForce(Vertex* v){
		    float dis = v->getCoord().length();
		    if(dis == 0.0){
		        return Vec2f(0, 0);
		    }
            float coeff = std::min(1.0f, dis);
		    return (Vec2f(0, 0) - v->getCoord()).normalize() * coeff;
		}


		void updatePos() {

			{
				VertexIter uIt(boundGraph());
				while (uIt.next()) {
					VertexIter vIt(boundGraph());
					while (vIt.next()) {
                        uIt.v->move(repelForce(uIt.v, vIt.v));
                        vIt.v->move(repelForce(vIt.v, uIt.v));
                    }
				}
			}
			{
				EdgeIter it(boundGraph());
				while (it.next()) {
					float dis = distance(it.u, it.v);
					it.u->move(attractForce(it.u, it.v));
                    it.v->move(attractForce(it.v, it.u));
				}
			}
			{
				VertexIter it(boundGraph());
				while (it.next()) {
				    it.v->move(originAttractForce(it.v));
                    if (it.v != grabbingVertex)
                        it.v->flushMove(m_c4);
                }
			}

		}

	};

};
