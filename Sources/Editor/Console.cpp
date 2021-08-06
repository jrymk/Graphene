#include <queue>
#include <Editor/Document/Document.h>

/// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
/// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
/// TODO                TODO                TODO           TODO TODO                TODO
/// TODO TODO      TODO TODO      TODO      TODO      TODO      TODO      TODO      TODO
/// TODO TODO      TODO TODO      TODO      TODO      TODO      TODO      TODO      TODO
/// TODO TODO      TODO TODO      TODO      TODO      TODO      TODO      TODO      TODO
/// TODO TODO      TODO TODO      TODO      TODO      TODO      TODO      TODO      TODO
/// TODO TODO      TODO TODO      TODO      TODO      TODO      TODO      TODO      TODO
/// TODO TODO      TODO TODO      TODO      TODO      TODO      TODO      TODO      TODO
/// TODO TODO      TODO TODO                TODO           TODO TODO                TODO
/// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
/// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO

namespace gfn {
    //extern gfn::Uuid activeDoc;

    //extern gfn::Document* getActiveDocument();

    //bool stopInput = false;

    //void execute(const std::string& cmd) { processes.push(cmd); }

    // continiously obtain command input from the terminal in the background
    /*void terminalInputLoop() {
        while (true) {
            std::string input;
            if (stopInput)
                break;
            std::getline(std::cin, input);
            processes.push(input);
        }
    }*/

    /*void cmdStartup() {
        std::thread inputThread(&terminalInputLoop);
        inputThread.detach();
    }*/

    // parse select command that focuses document via command line
    /*void parseSelect(gfn::Args command, gfn::Args& output) {
        if (!command.getParamValue("-f").empty()) {
            std::string file = command.getParamValue("-f");
            int match = 0;
            gfn::Uuid matchDoc;
            for (auto& d : documents) {
                if (d.second->file == file) {
                    matchDoc = d.first;
                    break;
                }
                if (d.second->getFileName() == file) {
                    matchDoc = d.first;
                    match++;
                }
            }
            if (matchDoc.empty()) {
                output.newParam("-output", "Specified file name not found or not opened");
                output.newParam("-error", "FILE_NOT_FOUND");
            } else if (match > 1) {
                output.newParam("-output", "Multiple open files found with the same file name, "
                                           "try specify the full path");
                output.newParam("-error", "MULTIPLE_FILE_CANDADATES");
            } else
                activeDoc = matchDoc;
        } else if (!command.getParamValue("-uuid").empty()) {
            if (documents.find(command.getParamValue("-uuid")) != documents.end())
                activeDoc = command.getParamValue("-uuid");
        } else {
            output.newParam("-output", "Expected file name [-f] or document uuid [-uuid]");
            output.newParam("-error", "FILE_NOT_SPECIFIED");
        }
    }*/

    /*void parseCommandQueue() {
        while (!processes.empty()) {
            std::string fullCmd = processes.front();
            gfn::Args command(fullCmd), output;
            processes.pop();
            std::string cmd = command.getParamValue("command");
            std::cout << "parsing: " << fullCmd << "\n";
            if (cmd == "select") parseSelect(command, output);
            else if (getActiveDocument()) getActiveDocument()->execute(fullCmd);
            else output.newParam("-error", "NO_DOCUMENT_FOCUSED");

            if (output.getParamValue("successful") == "false" || output.getFlag("-error") || output.getFlag("-warning"))
                std::cerr << output.getString() << "\n";
            else
                std::cout << output.getString() << "\n";
        }
    }*/
}