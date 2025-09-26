#include <iostream>
#include <stdexcept>
#include <string>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/hwcontext.h>
#include <libavutil/hwcontext_cuda.h>
#include <libavutil/error.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
}

class NVEncEncoder
{
public:
    NVEncEncoder()
    {
        // 查找支持CUDA的编码器
        codec = avcodec_find_encoder_by_name("h264_nvenc");
        if (!codec)
        {
            throw std::runtime_error("找不到h264_nvenc硬件编码器");
        }

        // 创建编码器上下文
        context = avcodec_alloc_context3(codec);
        if (!context)
        {
            throw std::runtime_error("无法分配编码器上下文");
        }

        std::cout << "使用的编码器: " << codec->name << std::endl;
    }

    bool initialize(int width, int height, int fps)
    {
        // 设置基本编码参数
        context->width      = width;
        context->height     = height;
        context->time_base  = {1, fps};
        context->framerate  = {fps, 1};
        context->codec_type = AVMEDIA_TYPE_VIDEO;
        context->codec_id   = codec->id;

        // 设置编码参数
        context->bit_rate     = 4000000; // 4 Mbps
        context->gop_size     = 10;      // GOP大小
        context->max_b_frames = 1;       // B帧数量

        // 创建CUDA设备上下文
        int ret = av_hwdevice_ctx_create(&hw_device_ctx,
                                         AV_HWDEVICE_TYPE_CUDA,
                                         nullptr, nullptr, 0);
        if (ret < 0)
        {
            printError("无法创建CUDA设备上下文", ret);
            return false;
        }

        // 创建CUDA帧上下文
        hw_frames_ctx = createCUDAFramesContext(hw_device_ctx);
        if (!hw_frames_ctx)
        {
            return false;
        }

        // 将硬件上下文关联到编码器
        context->hw_device_ctx = av_buffer_ref(hw_device_ctx);
        context->hw_frames_ctx = av_buffer_ref(hw_frames_ctx);

        // 设置像素格式
        context->pix_fmt = AV_PIX_FMT_CUDA;

        // 设置编码器预设参数
        av_opt_set(context->priv_data, "preset", "slow", 0);
        av_opt_set(context->priv_data, "profile", "main", 0);

        // 打开编码器
        ret = avcodec_open2(context, codec, nullptr);
        if (ret < 0)
        {
            printError("无法打开编码器", ret);
            return false;
        }

        std::cout << "编码器初始化成功" << std::endl;
        std::cout << "编码器: " << codec->name
                  << ", 尺寸: " << context->width << "x" << context->height
                  << ", 像素格式: " << av_get_pix_fmt_name(context->pix_fmt)
                  << std::endl;

        return true;
    }

    ~NVEncEncoder()
    {
        if (context)
        {
            avcodec_free_context(&context);
        }
        if (hw_frames_ctx)
        {
            av_buffer_unref(&hw_frames_ctx);
        }
        if (hw_device_ctx)
        {
            av_buffer_unref(&hw_device_ctx);
        }
    }

private:
    AVBufferRef* createCUDAFramesContext(AVBufferRef* hw_device_ctx)
    {
        AVBufferRef* frames_ref = av_hwframe_ctx_alloc(hw_device_ctx);
        if (!frames_ref)
        {
            std::cerr << "无法分配硬件帧上下文" << std::endl;
            return nullptr;
        }

        AVHWFramesContext* frames_ctx = (AVHWFramesContext*)frames_ref->data;
        frames_ctx->format            = AV_PIX_FMT_YUV420P;
        frames_ctx->sw_format         = AV_PIX_FMT_NV12; // NV12是CUDA常用的格式
        frames_ctx->width             = context->width;
        frames_ctx->height            = context->height;
        frames_ctx->initial_pool_size = 20; // 帧缓冲池大小
        frames_ctx->device_ref        = av_buffer_ref(hw_device_ctx);

        int ret = av_hwframe_ctx_init(frames_ref);
        if (ret < 0)
        {
            printError("无法初始化硬件帧上下文", ret);
            av_buffer_unref(&frames_ref);
            return nullptr;
        }

        return frames_ref;
    }

    void printError(const std::string& prefix, int errnum)
    {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(errnum, errbuf, sizeof(errbuf));
        std::cerr << prefix << ": " << errbuf << std::endl;
    }

private:
    const AVCodec*  codec         = nullptr;
    AVCodecContext* context       = nullptr;
    AVBufferRef*    hw_device_ctx = nullptr;
    AVBufferRef*    hw_frames_ctx = nullptr;
};

int main()
{
    try
    {
        NVEncEncoder encoder;
        if (!encoder.initialize(1920, 1080, 30))
        {
            std::cerr << "硬件编码器初始化失败" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "异常: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}