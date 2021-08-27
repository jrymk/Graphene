#include <string>
#include <Editor/Editor.h>
#include <fstream>
#include <set>
#include <Core/Objects/Random.h>

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
        std::string line;ㄊㄛ
        while (std::getline(ctFile, line)) {
            editor.execute(line);
        }*/

        auto docId = editor.newDocument();
        /*int bctSize = 1000;
        int maxBlkSize = 4;

        std::vector<int> blkSizes;
        for (int i = 0; i < bctSize; i++) {
            std::uniform_int_distribution<int> dis(1, maxBlkSize);
            int mySize = dis(gfn::getRng());
            blkSizes.push_back(mySize);
            std::uniform_int_distribution<int> hueDis(0, 359);
            int myHue = hueDis(gfn::getRng());
            for (int j = 0; j < mySize; j++) {
                editor.getDoc(docId)->execute("mkvertex -name=" + std::to_string(i) + ":" + std::to_string(j));
                editor.getDoc(docId)->execute("setvertexprops -name=" + std::to_string(i) + ":" + std::to_string(j) +
                                              " -key=fillColor -value=hsv(" + std::to_string(myHue) + ",100,100)");
                if (j == 0)
                    continue;
                std::uniform_int_distribution<int> blkDis(0, j - 1);
                editor.getDoc(docId)->execute(
                        "mkedge -uname=" + std::to_string(i) + ":" + std::to_string(j) + " -vname=" + std::to_string(i) + ":" +
                        std::to_string(blkDis(gfn::getRng())));
            }
            std::set<std::pair<int, int>> edges;
            std::uniform_int_distribution<int> edgeDis((mySize - 1), mySize * (mySize - 1) - (mySize - 1));
            int edgeCnt = edgeDis(gfn::getRng()) / 2;
            std::cerr << edgeCnt << "\n";
            std::uniform_int_distribution<int> vDis(0, mySize - 1);
            for (int e = 0; e < edgeCnt;) {
                int u = vDis(gfn::getRng()), v = vDis(gfn::getRng());
                if (u == v)
                    continue;
                if (u > v)
                    std::swap(u, v);
                if (edges.find({u, v}) != edges.end())
                    continue;
                edges.insert({u, v});
                editor.getDoc(docId)->execute(
                        "mkedge -uname=" + std::to_string(i) + ":" + std::to_string(u) + " -vname=" + std::to_string(i) + ":" + std::to_string(v));
                e++;
            }
            if (i == 0)
                continue;
            std::uniform_int_distribution<int> bctUDis(0, i - 1);
            int bctU = bctUDis(gfn::getRng());
            std::uniform_int_distribution<int> bctUvDis(0, blkSizes[bctU] - 1);
            int bctUv = bctUvDis(gfn::getRng());
            int bctVv = vDis(gfn::getRng());
            editor.getDoc(docId)->execute(
                    "mkedge -uname=" + std::to_string(bctU) + ":" + std::to_string(bctUv) + " -vname=" + std::to_string(i) + ":" + std::to_string(bctVv));
        }*/




        /*{
            std::string path = "./small tree.gfn";
            auto docId = editor.newDocument();
            editor.getDoc(docId)->setFile(path);
            editor.getDoc(docId)->execute("open");
            editor.getDoc(docId)->fileSaved = true;
        }*/
        //auto docId = editor.newDocument();

        //editor.getDoc(docId)->execute("echo -off");
        //editor.getDoc(docId)->execute("configs -c4=5.0");
//        int w = 80;
//        int h = 66;
//        for (int i = 0; i < w; i++) {
//            for (int j = 0; j < h; j++) {
//                editor.getDoc(docId)->execute("mkvertex -name=" + std::to_string(i) + "*" + std::to_string(j));
//            }
//        }
//        for (int i = 0; i < w; i++) {
//            for (int j = 0; j < h; j++) {
//                editor.getDoc(docId)->execute(
//                        "mkedge -uname=" + std::to_string((i + 1) % w) + "*" + std::to_string(j) + " -vname=" + std::to_string(i) + "*" + std::to_string(j));
//                editor.getDoc(docId)->execute(
//                        "mkedge -uname=" + std::to_string(i) + "*" + std::to_string((j + 1) % h) + " -vname=" + std::to_string(i) + "*" + std::to_string(j));
//            }
//        }
//        int v = 32768 / 8;
//        editor.getDoc(docId)->execute("mkvertex -name=0");
//        for (int i = 0; i < v - 1; i++) {
//            editor.getDoc(docId)->execute("mkvertex -name=" + std::to_string(i + 1));
//            std::uniform_int_distribution dis(0, i);
//            editor.getDoc(docId)->execute(
//                    "mkedge -uname=" + std::to_string(i + 1) + " -vname=" + std::to_string(dis(gfn::getRng())));
//        }

        while (!editor.gfx.shouldClose()) {
            editor.update();
        }
        editor.terminate();
    }

    return 0;
}
