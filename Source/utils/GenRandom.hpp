#pragma  once
#include <random>
#include <sstream>

namespace Utils {
    namespace UID {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        std::uniform_int_distribution<> dis16(0, 15);
        std::uniform_int_distribution<> dis4(8, 11);
        bool launch = true;

        float genRandom() {
            if (launch) {
                launch = false;
                gen.seed(time(nullptr));
            }
            return dis(gen);
        }

        int genRandom16() {
            if (launch) {
                launch = false;
                gen.seed(time(nullptr));
            }
            return dis16(gen);
        }

        int genRandom4() {
            if (launch) {
                launch = false;
                gen.seed(time(nullptr));
            }
            return dis4(gen);
        }

        static std::string generate_uuid_v4() {
            std::stringstream ss;
            int i;
            ss << std::hex;
            for (i = 0; i < 8; i++) {
                ss << genRandom16();
            }
            ss << "-";
            for (i = 0; i < 4; i++) {
                ss << genRandom16();
            }
            ss << "-4";
            for (i = 0; i < 3; i++) {
                ss << genRandom16();
            }
            ss << "-";
            ss << genRandom4();
            for (i = 0; i < 3; i++) {
                ss << genRandom16();
            }
            ss << "-";
            for (i = 0; i < 12; i++) {
                ss << genRandom16();
            };
            return ss.str();
        }

        static std::string generate_64() {
            std::stringstream ss;
            int i;
            ss << std::hex;
            for (i = 0; i < 8; i++) {
                ss << genRandom16();
            }
            return ss.str();
        }

    }
}