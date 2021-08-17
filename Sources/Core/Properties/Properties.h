#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <unordered_map>
#include <unordered_set>
#include <Core/Objects/Uuid.h>
#include <Core/Properties/VertexProps.h>
#include <Core/Properties/EdgeProps.h>
#include <binn.h>

namespace gfn {
/// @brief Stores all the properties such as positions, colors and force values for internal use
    class Properties {
        // user friendly name mapping for uuids
        std::unordered_map<std::string, gfn::Uuid> accessNames;
        // this class stays ar the bottom of anything, so that usergraph, core, the api and so on can access it
        std::unordered_map<gfn::Uuid, gfn::VertexProps> vertexPropsList;
        std::unordered_map<gfn::Uuid, gfn::EdgeProps> edgePropsList;

    public:
        Properties();

        bool stablized = false;

        ///@brief serializes vertices and edges props data into binary form, remember to free the buffer after read
        void serialize(binn* document);

        void deserialize(void* document);

        /// @brief uuids can be hard to type and read, user can assign custom names to every uuid for easy access
        bool assignAccessName(const std::string& accessName, const gfn::Uuid& uuid, bool overwrite = false);

        bool revokeAccessName(const std::string& accessName);

        gfn::Uuid convertAccessName(const std::string& accessName);

        /// @brief retrieve the properties object of the given vertex
        /// @returns nullptr if the prop does not exist, else the pointer to the prop object
        gfn::VertexProps* getVertexProps(const gfn::Uuid& uuid, bool warnOnNotFound = true);

        /// @brief retrieve the properties object of the given edge
        /// @returns: returns nullptr if the prop does not exist, else the pointer to the prop object
        gfn::EdgeProps* getEdgeProps(const gfn::Uuid& edgeUuid, bool warnOnNotFound = true);

        /// @brief allocate a new vertex properties object
        /// @returns false if prop already exist but clearExisting is false
        bool newVertexProps(const gfn::Uuid& uuid, bool clearExisting = false);

        /// @brief allocate a new vertex properties object
        bool newEdgeProps(const gfn::Uuid& edgeUuid, bool clearExisting = false);

        /// @brief erase a vertex prop
        /// @returns if the erase was successful
        bool eraseVertexProps(const gfn::Uuid& edgeUuid);

        /// @brief erase an edge prop
        /// @returns if the erase was successful
        bool eraseEdgeProps(const gfn::Uuid& edgeUuid);

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, gfn::VertexProps>& getVertexPropsList();

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, gfn::EdgeProps>& getEdgePropsList();

        void setVertexProp(gfn::Args command, gfn::Args& output);

        void getVertexProp(gfn::Args command, gfn::Args& output);

        void setEdgeProp(gfn::Args command, gfn::Args& output);

        void getEdgeProp(gfn::Args command, gfn::Args& output);
    };
} // namespace gfn::properties

#endif