#pragma once

#include <iostream>
#include <memory>
#include <opencv2\highgui\highgui.hpp>
#include <string>

namespace CVTraning {
namespace utils {

/**
 * @brief �����ڲ���ffmpeg֧�ֵ���Ƶ�ļ�
 */
class VideoPlayer {
   public:
    enum struct TOOL_OPT : int {
        TRACK_BAR = 1 << 0,
    };

    struct VideoInfo {
        VideoInfo(const int frames, const int width, const int height)
            : frames_(frames), width_(width), height_(height) {}

        friend std::ostream& operator<<(std::ostream& os,
                                        const VideoInfo& info);

        const int frames_;
        const int width_;
        const int height_;
    };

   public:
    /**
     * @brief 
     * @param file_path 
     * @param tools 
     * @param window_name 
     * 
     * @warning param 'tools' is not for use yet
     * 
     * @note ����cv::createTrackbar()�����û��ʹ��ȫ�ֱ���ʱ������ͨ�����һ��������(void*)this���ݽ�ȥ��Callback��Ȼʹ��ȫ�ֺ�����ע�ᣬȻ��ʹ��void*��ǿ��ת�����Ӷ���ȡ��ǰ������ָ��
     * @note OnTrackbarSlide�����û������������Żᴥ��
    */
    VideoPlayer(const std::string& file_path, const int tools,
                const std::string& window_name = "")
        : file_path_(file_path), tools_(tools), window_name_(window_name.empty()? file_path: window_name), track_bar_name_("track_bar"),is_inited_(false), cap_ptr_(nullptr),video_info_(nullptr), slider_position_(0), run_mode_(1), dontset_(0) {}

    virtual ~VideoPlayer();

    bool Play();

   private:
    bool Init();
    void CreateWindow();
    void DestroyWindow();

    void GetVideoInfo();
    static void OnTrackbarSlide(int pos, void*);

   private:
    const std::string file_path_;
    const std::string window_name_;
    const std::string track_bar_name_;
    const int tools_;

    bool is_inited_;
    std::shared_ptr<cv::VideoCapture> cap_ptr_;
    std::shared_ptr<VideoInfo> video_info_;

    int slider_position_; // ��ʾ��������ǰ����
    int run_mode_;// ��1��0��-1�������ǲ������ǲ���
    int dontset_;
};

}  // namespace utils
}  // namespace CVTraning