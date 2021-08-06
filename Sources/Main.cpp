#include <string>
#include <Editor/Editor.h>
#include <minitrace.h>

int main(int argc, char* argv[]) {
    bool enableEditorGui = true;

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-no-gui")
            enableEditorGui = false;
    }

    mtr_init("trace.json");
//    MTR_META_PROCESS_NAME("graphene");
//    MTR_META_THREAD_NAME("main thread");
//    MTR_SCOPE("main", "main");

    if (enableEditorGui) {
        MTR_SCOPE("main", "editor");

        gfn::Editor editor;

        {
            std::string path = "./bagel.gfn";
            auto docId = editor.newDocument();
            editor.getDoc(docId)->setFile(path);
            editor.getDoc(docId)->execute("open");
            editor.getDoc(docId)->fileSaved = true;
        }

        while (!editor.gfx.shouldClose()) {
            MTR_SCOPE("main", "frame");
            editor.update();
        }
        editor.terminate();
    }
    mtr_shutdown();
    return 0;
}
