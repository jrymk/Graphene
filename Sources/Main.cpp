#include <string>
#include <Editor/Editor.h>

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

        {
            std::string path = "./bagel.gfn";
            auto docId = editor.newDocument();
            editor.getDoc(docId)->setFile(path);
            editor.getDoc(docId)->execute("open");
            editor.getDoc(docId)->fileSaved = true;
        }

        while (!editor.gfx.shouldClose()) {
            editor.update();
        }
        editor.terminate();
    }
    return 0;
}
