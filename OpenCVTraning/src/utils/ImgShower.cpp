#include "OpenCVTraning/include/utils/ImgShower.h"

#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>

namespace CVTraning {
namespace utils {

void ImgShower::ShowImg(const std::string& file_path,
                        const std::string& window_name, const int wait_key) {
    // create ImgShower and then call show()
    ImgShower img_shower(file_path);

    if (img_shower.Show(window_name, wait_key) == RES::FAIL) {
        // show img fail, most probably because of read image file failed
        std::cout << "show image from path " << file_path << " failed"
                  << std::endl;
    }

    return;
}

ImgShower::RES ImgShower::Show(const std::string& window_name,
                               const int wait_key) {
    cv::Mat img = cv::imread(file_path_);
    if (img.empty()) return RES::FAIL;

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(window_name, img);
    cv::waitKey(wait_key);

    cv::destroyWindow(window_name);

    return RES::SUCCESS;
};

}  // namespace utils
}  // namespace CVTraning