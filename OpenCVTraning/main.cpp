#include <iostream>
#include <string>
#include "OpenCVTraning/include/utils/ImgShower.h"
#include "OpenCVTraning/include/utils/VideoPlayer.h"


int main(int argc, char* argv) {

    //std::string file_path = "C:/Users/z_t_h/Pictures/temp/8027853a704eac31e5d339d1cda9d362.gif";
    //CVTraning::utils::ImgShower::ShowImg(file_path, file_path);

    // 路径名必须用除号
    using Player = CVTraning::utils::VideoPlayer;
    std::string file_path = "C:/Users/z_t_h/Pictures/temp/8027853a704eac31e5d339d1cda9d362.gif";
    Player player(file_path, 0);
    if (!player.Play()) std::cout << "Play video failed" << std::endl;

    return 0;
}