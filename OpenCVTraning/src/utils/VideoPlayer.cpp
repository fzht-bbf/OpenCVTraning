#include <OpenCvTraning\include\utils\VideoPlayer.h>

#include <iostream>
#include <opencv2\imgproc\imgproc.hpp>

namespace CVTraning {
namespace utils {

std::ostream& operator<<(std::ostream& os, const VideoPlayer::VideoInfo& info) {
    os << "{frames: " << info.frames_ << ", width_: " << info.width_
       << ", height_: " << info.width_ << "};";
    return os;
}

VideoPlayer::~VideoPlayer() {
    DestroyWindow();
    return;
}

bool VideoPlayer::Play() {
    if (!Init()) return false;

    cv::Mat frame;
    while (true) {
        if (run_mode_ != 0) {
            (*cap_ptr_) >> frame;
            if (frame.empty()) break;  // end of video
            int current_pos = (int)cap_ptr_->get(cv::CAP_PROP_POS_FRAMES);
            dontset_ = 1;

            cv::setTrackbarPos(track_bar_name_, window_name_, current_pos);

            // 处理当前frame
            ProcessCurrentFrame(frame);

            cv::imshow(window_name_, frame);

            run_mode_ -= 1;  // step by step, if running_ > 0
        }

        char pressed_key = cv::waitKey(10);
        if (pressed_key == 's') {  // single step
            run_mode_ = 1;
            std::cout << "switch to SINGLE STEP mode" << std::endl;
        } else if (pressed_key == 'r') {
            run_mode_ = -1;
            std::cout << "switch RUN mode" << std::endl;
        } else if (pressed_key == 27) {
            std::cout << "END PLAY" << std::endl;
            break;
        }
    }

    return true;
}

void VideoPlayer::ProcessCurrentFrame(cv::Mat& frame) {
    //frame = std::move(BlurThisFrame(frame));
    frame = std::move(DetectEdge(frame));
    return;
}

cv::Mat VideoPlayer::BlurThisFrame(const cv::Mat& frame) {
    cv::Mat out1, out2;
    // TODO: GaussianBlur里面的sigmaX和sigmaY两个参数是什么含义？
    cv::GaussianBlur(frame, out1, cv::Size(5, 5), 3, 3);
    cv::GaussianBlur(out1, out2, cv::Size(5, 5), 3, 3);

    cv::Mat out;
    cv::hconcat(out1, out2, out);
    cv::hconcat(frame, out, out);

    return std::move(out);
}

cv::Mat VideoPlayer::DetectEdge(const cv::Mat& frame) {
    cv::Mat out;
    cv::Mat img_grey, img_cny;

    cv::cvtColor(frame, img_grey, cv::COLOR_BGR2GRAY);
    cv::Canny(img_grey, img_cny, 10, 100, 3, true);

    cv::hconcat(img_grey, img_cny, out);

    return std::move(out);
}

bool VideoPlayer::Init() {
    if (is_inited_) return true;

    CreateWindow();

    cap_ptr_ = std::make_shared<cv::VideoCapture>();
    cap_ptr_->open(file_path_);
    if (!cap_ptr_->isOpened()) {
        // open失败也有可能是ffmpeg的库出了问题，可能是因为找不到opencv_ffmpeg2410_64.dll而导致失败的
        std::cout << "open video from " << file_path_ << " failed" << std::endl;
        return false;
    }

    GetVideoInfo();

    // "this" down here shall be pass into OnTrackbarSlide as a void pointer
    cv::createTrackbar(track_bar_name_, window_name_, &slider_position_,
                       video_info_->frames_, OnTrackbarSlide, (void*)this);

    is_inited_ = true;
    return true;
}

void VideoPlayer::CreateWindow() {
    cv::namedWindow(window_name_, cv::WINDOW_AUTOSIZE);
    return;
}

void VideoPlayer::DestroyWindow() {
    cv::destroyWindow(window_name_);
    return;
}

void VideoPlayer::OnTrackbarSlide(int pos, void* ptr) {
    auto that = (VideoPlayer*)ptr;
    that->cap_ptr_->set(cv::CAP_PROP_POS_FRAMES,
                        pos);  // 设置接下来要读的帧的位置
    if (!that->dontset_) that->run_mode_ = 1;
    that->dontset_ = 0;  // 在拖动进度条后，自动变为步进模式
    return;
}

void VideoPlayer::GetVideoInfo() {
    if (cap_ptr_ == nullptr) return;

    int frames = (int)cap_ptr_->get(cv::CAP_PROP_FRAME_COUNT);
    int width = (int)cap_ptr_->get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int)cap_ptr_->get(cv::CAP_PROP_FRAME_HEIGHT);

    video_info_ = std::make_shared<VideoInfo>(frames, width, height);
    std::cout << "Video info: " << (*video_info_) << std::endl;

    return;
}

}  // namespace utils
}  // namespace CVTraning