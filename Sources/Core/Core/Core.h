#ifndef CORE_H
#define CORE_H

#include <Core/Interface/Interface.h>
#include <Core/Structure/Structure.h>
#include <Core/Placement/Placement.h>
#include <Core/Objects/Args.h>
#include <thread>

namespace gfn {
    class Core {
    private:
        int echo = 2;
        int echoPrev = 2;
        int bgState = 0; // 0: STOPPING  1: RUNNING  2: TERMINATED
        void bgFunction(int& state);

    public:
        gfn::Interface itf; // the input/output stream to the gui/console
        gfn::Structure structure; // stores graph data in specialized format
        gfn::Placement placement; // containes graph drawing algorithm functions
        std::string file; // the gfn file core opens and saves to, or use -f to override

        explicit Core();

        void startBackground();

        // this function will block the calling thread while core is terminating
        // to stop a batch of core background loops, you can set doNotWait to true.
        // then, do checkTermination for each instance
        void terminateBackground(bool doNotWait = false);

        /// @returns true if terminated, will return false if terminateBackground is called with wait
        bool checkTermination();

        gfn::Interface* getInterface();

        void update();

        void parse(Args command, gfn::Args& output);

        void parseCommands();

        void openFile(Args command, gfn::Args& output);

        void saveFile(Args command, gfn::Args& output);
    };
}

#endif