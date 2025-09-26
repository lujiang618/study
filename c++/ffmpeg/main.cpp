extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <algorithm>

// 错误处理宏
#define ERROR_BUF(ret) \
    char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof(errbuf))

// 全局变量
static AVFormatContext* fmt_ctx       = nullptr;
static AVCodecContext*  codec_ctx     = nullptr;
static AVStream*        video_stream  = nullptr;
static AVFrame*         sw_frame      = nullptr; // 软件帧
static AVFrame*         hw_frame      = nullptr; // 硬件帧
static AVPacket*        pkt           = nullptr;
static AVBufferRef*     hw_device_ctx = nullptr;
static AVBufferRef*     hw_frames_ctx = nullptr;

// 初始化硬件设备
int init_hw_device(AVHWDeviceType type)
{
    int ret = av_hwdevice_ctx_create(&hw_device_ctx, type, nullptr, nullptr, 0);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to create VAAPI device: " << errbuf << std::endl;
        return ret;
    }
    return 0;
}

// 初始化硬件帧缓冲区
int init_hw_frames_context(int width, int height)
{
    // 创建硬件帧缓冲区上下文
    hw_frames_ctx = av_hwframe_ctx_alloc(hw_device_ctx);
    if (!hw_frames_ctx)
    {
        std::cerr << "Failed to create VAAPI frame context" << std::endl;
        return -1;
    }

    AVHWFramesContext* frames_ctx = (AVHWFramesContext*)hw_frames_ctx->data;
    frames_ctx->format            = AV_PIX_FMT_VAAPI;
    frames_ctx->sw_format         = AV_PIX_FMT_NV12;
    frames_ctx->width             = width;
    frames_ctx->height            = height;
    frames_ctx->initial_pool_size = 20;

    int ret = av_hwframe_ctx_init(hw_frames_ctx);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to initialize VAAPI frame context: " << errbuf << std::endl;
        return ret;
    }

    return 0;
}

// 初始化编码器
int init_encoder(int width, int height, int fps, int bitrate, int gop_size)
{
    // 查找H.264编码器
    const AVCodec* codec = avcodec_find_encoder_by_name("h264_vaapi");
    if (!codec)
    {
        std::cerr << "H.264 VAAPI encoder not found" << std::endl;
        return -1;
    }

    // 创建编码器上下文
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        std::cerr << "Failed to allocate codec context" << std::endl;
        return -1;
    }

    // 设置编码参数
    codec_ctx->codec_id     = AV_CODEC_ID_H264;
    codec_ctx->codec_type   = AVMEDIA_TYPE_VIDEO;
    codec_ctx->width        = width;
    codec_ctx->height       = height;
    codec_ctx->bit_rate     = bitrate;
    codec_ctx->time_base    = {1, fps};
    codec_ctx->framerate    = {fps, 1};
    codec_ctx->gop_size     = gop_size;
    codec_ctx->max_b_frames = 0;
    codec_ctx->pix_fmt      = AV_PIX_FMT_VAAPI;

    // 设置质量参数
    // av_opt_set(codec_ctx->priv_data, "rc_mode", "CQP", 0);
    // av_opt_set(codec_ctx->priv_data, "qp", "30", 0);
    // av_opt_set(codec_ctx->priv_data, "preset", "fast", 0);
    // av_opt_set(codec_ctx->priv_data, "tune", "zerolatency", 0);

    // // 禁用不支持的打包头部
    // av_opt_set(codec_ctx->priv_data, "sei", "0", 0);

    // // 设置编码配置文件
    av_opt_set(codec_ctx->priv_data, "profile", "main", 0);
    av_opt_set(codec_ctx->priv_data, "level", "4.1", 0);

    av_opt_set(codec_ctx->priv_data, "rc_mode", "CBR", 0);

    // 设置硬件设备上下文
    codec_ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
    if (!codec_ctx->hw_device_ctx)
    {
        std::cerr << "Failed to set hardware device context" << std::endl;
        return -1;
    }

    // 设置硬件帧缓冲区上下文
    codec_ctx->hw_frames_ctx = av_buffer_ref(hw_frames_ctx);
    if (!codec_ctx->hw_frames_ctx)
    {
        std::cerr << "Failed to set hardware frames context" << std::endl;
        return -1;
    }

    // 打开编码器
    int ret = avcodec_open2(codec_ctx, codec, nullptr);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to open codec: " << errbuf << std::endl;
        return ret;
    }

    return 0;
}

// 初始化输出格式
int init_output(const char* filename, int width, int height, int fps)
{
    // 创建输出格式上下文
    int ret = avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, filename);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to create output context: " << errbuf << std::endl;
        return ret;
    }

    // 创建视频流
    video_stream = avformat_new_stream(fmt_ctx, nullptr);
    if (!video_stream)
    {
        std::cerr << "Failed to create video stream" << std::endl;
        return -1;
    }

    // 设置流参数
    video_stream->codecpar->codec_id   = AV_CODEC_ID_H264;
    video_stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    video_stream->codecpar->width      = width;
    video_stream->codecpar->height     = height;
    // video_stream->codecpar->format     = AV_PIX_FMT_VAAPI;
    video_stream->time_base = {1, fps};

    // 打开输出文件
    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&fmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            ERROR_BUF(ret);
            std::cerr << "Failed to open output file: " << errbuf << std::endl;
            return ret;
        }
    }

    return 0;
}

// 写入文件头
int write_header()
{
    int ret = avformat_write_header(fmt_ctx, nullptr);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to write file header: " << errbuf << std::endl;
        return ret;
    }
    return 0;
}

// 写入文件尾
int write_trailer()
{
    int ret = av_write_trailer(fmt_ctx);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to write file trailer: " << errbuf << std::endl;
        return ret;
    }
    return 0;
}

// 编码并写入帧
int encode_and_write(AVFrame* frame)
{
    std::cout << "Encoding frame: format=" << frame->format
              << ", width=" << frame->width
              << ", height=" << frame->height
              << ", pts=" << frame->pts << std::endl;

    int ret = avcodec_send_frame(codec_ctx, frame);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to send frame to encoder: " << errbuf << std::endl;
        return ret;
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_packet(codec_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            break;
        }
        else if (ret < 0)
        {
            ERROR_BUF(ret);
            std::cerr << "Failed to receive packet from encoder: " << errbuf << std::endl;
            return ret;
        }

        // 设置包的时间基
        av_packet_rescale_ts(pkt, codec_ctx->time_base, video_stream->time_base);
        pkt->stream_index = video_stream->index;

        // 写入包
        ret = av_interleaved_write_frame(fmt_ctx, pkt);
        if (ret < 0)
        {
            ERROR_BUF(ret);
            std::cerr << "Failed to write packet: " << errbuf << std::endl;
            av_packet_unref(pkt);
            return ret;
        }

        av_packet_unref(pkt);
    }

    return 0;
}

// 读取并解码JPEG图片
int read_jpeg_image(const char* filename, AVFrame* output_frame)
{
    AVFormatContext* rfmt_ctx      = nullptr;
    AVCodecContext*  dec_ctx       = nullptr;
    const AVCodec*   decoder       = nullptr;
    AVPacket*        packet        = nullptr;
    AVFrame*         jpeg_frame    = nullptr;
    SwsContext*      sws_ctx_local = nullptr;
    int              ret           = 0;

    // 提前声明变量
    AVCodecParameters* codecpar = nullptr;

    // 分配资源
    packet     = av_packet_alloc();
    jpeg_frame = av_frame_alloc();
    if (!packet || !jpeg_frame)
    {
        std::cerr << "Failed to allocate packet or frame" << std::endl;
        ret = -1;
        goto end;
    }

    // 打开输入文件
    ret = avformat_open_input(&rfmt_ctx, filename, nullptr, nullptr);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to open JPEG file " << filename << ": " << errbuf << std::endl;
        goto end;
    }

    // 查找流信息
    ret = avformat_find_stream_info(rfmt_ctx, nullptr);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to find stream info: " << errbuf << std::endl;
        goto end;
    }

    // 查找解码器
    codecpar = rfmt_ctx->streams[0]->codecpar;
    decoder  = avcodec_find_decoder(codecpar->codec_id);
    if (!decoder)
    {
        std::cerr << "Failed to find JPEG decoder" << std::endl;
        ret = -1;
        goto end;
    }

    // 分配解码器上下文
    dec_ctx = avcodec_alloc_context3(decoder);
    if (!dec_ctx)
    {
        std::cerr << "Failed to allocate decoder context" << std::endl;
        ret = -1;
        goto end;
    }

    // 复制编解码器参数到解码器上下文
    ret = avcodec_parameters_to_context(dec_ctx, codecpar);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to copy codec parameters to decoder context: " << errbuf << std::endl;
        goto end;
    }

    // 打开解码器
    ret = avcodec_open2(dec_ctx, decoder, nullptr);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to open decoder: " << errbuf << std::endl;
        goto end;
    }

    // 读取并解码帧
    ret = av_read_frame(rfmt_ctx, packet);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to read frame: " << errbuf << std::endl;
        goto end;
    }

    // 发送包到解码器
    ret = avcodec_send_packet(dec_ctx, packet);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to send packet to decoder: " << errbuf << std::endl;
        goto end;
    }

    // 接收解码后的帧
    ret = avcodec_receive_frame(dec_ctx, jpeg_frame);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to receive frame from decoder: " << errbuf << std::endl;
        goto end;
    }

    // 创建转换上下文
    sws_ctx_local = sws_getContext(
        jpeg_frame->width, jpeg_frame->height, (AVPixelFormat)jpeg_frame->format,
        output_frame->width, output_frame->height, AV_PIX_FMT_NV12,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    if (!sws_ctx_local)
    {
        std::cerr << "Failed to create scale context" << std::endl;
        ret = -1;
        goto end;
    }

    if (sws_ctx_local)
    {
        sws_setColorspaceDetails(
            sws_ctx_local,
            sws_getCoefficients(SWS_CS_ITU709), // 或 SWS_CS_DEFAULT
            0,
            sws_getCoefficients(SWS_CS_ITU709),
            0,
            0, 1 << 16, 1 << 16);
    }

    // 执行转换
    sws_scale(sws_ctx_local,
              (const uint8_t* const*)jpeg_frame->data, jpeg_frame->linesize,
              0, jpeg_frame->height,
              output_frame->data, output_frame->linesize);

    output_frame->pts = 0;

end:
    // 清理资源
    av_frame_free(&jpeg_frame);
    av_packet_free(&packet);
    avcodec_free_context(&dec_ctx);
    if (rfmt_ctx)
    {
        avformat_close_input(&rfmt_ctx);
    }
    if (sws_ctx_local)
    {
        sws_freeContext(sws_ctx_local);
    }
    return ret;
}

// 获取图片文件列表
std::vector<std::string> get_image_files(const std::string& directory)
{
    std::vector<std::string> files;
    DIR*                     dir;
    struct dirent*           ent;

    if ((dir = opendir(directory.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            std::string filename = ent->d_name;
            if (filename.find(".jpg") != std::string::npos ||
                filename.find(".jpeg") != std::string::npos)
            {
                files.push_back(directory + "/" + filename);
            }
        }
        closedir(dir);
    }

    // 排序文件
    std::sort(files.begin(), files.end());
    return files;
}

// 清理资源
void cleanup()
{
    av_frame_free(&sw_frame);
    av_frame_free(&hw_frame);
    av_packet_free(&pkt);
    avcodec_free_context(&codec_ctx);
    if (fmt_ctx && !(fmt_ctx->oformat->flags & AVFMT_NOFILE))
    {
        avio_closep(&fmt_ctx->pb);
    }
    avformat_free_context(fmt_ctx);
    av_buffer_unref(&hw_device_ctx);
    av_buffer_unref(&hw_frames_ctx);
}

int main()
{
    // av_log_set_level(AV_LOG_TRACE); // 打开详细日志

    // 初始化参数
    const char*       output_file = "output.mp4";
    const std::string image_dir   = "images";
    int               width       = 1920;
    int               height      = 1080;
    int               fps         = 25;
    int               bitrate     = 2000000; // 2Mbps
    int               gop_size    = 600;     // 对应 -g 0
    int               ret         = 0;

    // 初始化libav
    avformat_network_init();

    // 获取图片文件列表
    std::vector<std::string> image_files = get_image_files(image_dir);
    if (image_files.empty())
    {
        std::cerr << "No JPEG images found in directory: " << image_dir << std::endl;
        return -1;
    }

    // 初始化硬件设备
    ret = init_hw_device(AV_HWDEVICE_TYPE_VAAPI);
    if (ret < 0)
    {
        cleanup();
        return ret;
    }

    // 初始化硬件帧缓冲区
    ret = init_hw_frames_context(width, height);
    if (ret < 0)
    {
        cleanup();
        return ret;
    }

    // 初始化编码器
    ret = init_encoder(width, height, fps, bitrate, gop_size);
    if (ret < 0)
    {
        cleanup();
        return ret;
    }

    // 初始化输出格式
    ret = init_output(output_file, width, height, fps);
    if (ret < 0)
    {
        cleanup();
        return ret;
    }

    // 分配帧和包
    sw_frame = av_frame_alloc(); // 软件帧
    hw_frame = av_frame_alloc(); // 硬件帧
    pkt      = av_packet_alloc();
    if (!sw_frame || !hw_frame || !pkt)
    {
        std::cerr << "Failed to allocate frame or packet" << std::endl;
        cleanup();
        return -1;
    }

    // 设置软件帧参数
    sw_frame->format = AV_PIX_FMT_NV12;
    sw_frame->width  = width;
    sw_frame->height = height;

    ret = av_frame_get_buffer(sw_frame, 32);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to allocate software frame buffer: " << errbuf << std::endl;
        cleanup();
        return ret;
    }

    // 设置硬件帧参数（从硬件帧缓冲区获取）
    ret = av_hwframe_get_buffer(hw_frames_ctx, hw_frame, 0);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to allocate hardware frame: " << errbuf << std::endl;
        cleanup();
        return ret;
    }

    ret = avcodec_parameters_from_context(video_stream->codecpar, codec_ctx);
    if (ret < 0)
    {
        ERROR_BUF(ret);
        std::cerr << "Failed to copy codec params to stream: " << errbuf << std::endl;
        return ret;
    }

    // 写入文件头
    ret = write_header();
    if (ret < 0)
    {
        cleanup();
        return ret;
    }

    AVRational time_base      = {1, fps};
    int        encoded_frames = 0;

    // 处理每一帧图片
    for (size_t i = 0; i < image_files.size(); ++i)
    {
        // 读取并解码JPEG图片到软件帧
        ret = read_jpeg_image(image_files[i].c_str(), sw_frame);
        if (ret < 0)
        {
            std::cerr << "Failed to read JPEG image: " << image_files[i] << std::endl;
            continue;
        }

        std::cout << "sw_frame: format=" << sw_frame->format
                  << ", width=" << sw_frame->width
                  << ", height=" << sw_frame->height << std::endl;

        sw_frame->color_range     = AVCOL_RANGE_MPEG;
        sw_frame->colorspace      = AVCOL_SPC_BT709;
        sw_frame->color_primaries = AVCOL_PRI_BT709;
        sw_frame->color_trc       = AVCOL_TRC_BT709;

        // 将软件帧传输到硬件帧
        ret = av_hwframe_transfer_data(hw_frame, sw_frame, 0);
        if (ret < 0)
        {
            ERROR_BUF(ret);
            std::cerr << "Failed to transfer frame to hardware: " << errbuf << std::endl;
            break;
        }

        std::cout << "hw_frame: format=" << hw_frame->format
                  << ", width=" << hw_frame->width
                  << ", height=" << hw_frame->height << std::endl;

        // 设置帧参数
        hw_frame->pts = av_rescale_q(i, time_base, codec_ctx->time_base);

        // 编码并写入帧
        ret = encode_and_write(hw_frame);
        if (ret < 0)
        {
            std::cerr << "Failed to encode frame " << i << std::endl;
            break;
        }

        encoded_frames++;
        std::cout << "Processed frame: " << i + 1 << "/" << image_files.size() << std::endl;
    }

    // 只有在成功编码了帧之后才尝试刷新编码器
    if (encoded_frames > 0)
    {
        std::cout << "Flushing encoder..." << std::endl;
        // 发送空帧以刷新编码器
        ret = avcodec_send_frame(codec_ctx, nullptr);
        if (ret >= 0)
        {
            // 接收所有剩余的包
            while (1)
            {
                ret = avcodec_receive_packet(codec_ctx, pkt);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                    break;
                else if (ret < 0)
                {
                    ERROR_BUF(ret);
                    std::cerr << "Error receiving packet from encoder: " << errbuf << std::endl;
                    break;
                }

                // 设置包的时间基
                av_packet_rescale_ts(pkt, codec_ctx->time_base, video_stream->time_base);
                pkt->stream_index = video_stream->index;

                // 写入包
                ret = av_interleaved_write_frame(fmt_ctx, pkt);
                av_packet_unref(pkt);
                if (ret < 0)
                {
                    ERROR_BUF(ret);
                    std::cerr << "Error writing packet: " << errbuf << std::endl;
                    break;
                }
            }
        }
    }

    // 写入文件尾
    std::cout << "Writing trailer..." << std::endl;
    write_trailer();

    // 清理资源
    cleanup();

    std::cout << "Video encoding completed: " << output_file << std::endl;
    std::cout << "Encoded " << encoded_frames << " frames" << std::endl;
    return 0;
}