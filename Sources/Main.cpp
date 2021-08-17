#include <string>
#include <Editor/Editor.h>
#include <fstream>

int main(int argc, char* argv[]) {
    bool enableEditorGui = true;

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-no-gui")
            enableEditorGui = false;
    }

//    MTR_META_PROCESS_NAME("graphene");
//    MTR_META_THREAD_NAME("main thread");

    if (enableEditorGui) {

        gfn::Editor editor;

        /*std::ifstream ctFile("CT-Output.txt");
        std::string line;
        while (std::getline(ctFile, line)) {
            editor.execute(line);
        }*/

        /*{
            std::string path = "./small tree.gfn";
            auto docId = editor.newDocument();
            editor.getDoc(docId)->setFile(path);
            editor.getDoc(docId)->execute("open");
            editor.getDoc(docId)->fileSaved = true;
        }*/
        //auto docId = editor.newDocument();

        /*editor.getDoc(docId)->execute("echo -off");
        editor.getDoc(docId)->execute("configs -c4=5.0");
        int w = 12;
        int h = 20;
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                editor.getDoc(docId)->execute("mkvertex -name=" + std::to_string(i) + "*" + std::to_string(j));
            }
        }
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                editor.getDoc(docId)->execute(
                        "mkedge -uname=" + std::to_string((i + 1) % w) + "*" + std::to_string(j) + " -vname=" + std::to_string(i) + "*" + std::to_string(j));
                editor.getDoc(docId)->execute(
                        "mkedge -uname=" + std::to_string(i) + "*" + std::to_string((j + 1) % h) + " -vname=" + std::to_string(i) + "*" + std::to_string(j));
            }
        }*/

        while (!editor.gfx.shouldClose()) {
            editor.update();
        }
        editor.terminate();
    }
    return 0;
}
