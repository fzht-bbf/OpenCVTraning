#pragma once

#include <string>

namespace CVTraning {
namespace utils {

class ImgShower {
   public:
    enum struct RES {
        SUCCESS,
        FAIL,
    };

   public:
    ImgShower(const std::string& file_path) : file_path_(file_path) {}

    static void ShowImg(const std::string& file_path,
                        const std::string& window_name = "img",
                        const int wait_key = 0);

    RES Show(const std::string& window_name = "img", const int wait_key = 0);

   private:
    const std::string file_path_;
};

}  // namespace utils
}  // namespace CVTraning