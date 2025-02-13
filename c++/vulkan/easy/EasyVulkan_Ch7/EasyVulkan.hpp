#include "VkBase+.h"

using namespace vulkan;
const VkExtent2D& windowSize = graphicsBase::Base().SwapchainCreateInfo().imageExtent;

namespace easyVulkan
{
struct renderPassWithFramebuffers
{
    renderPasses               renderPasses;
    std::vector<framebuffer> framebuffers;
};

const auto& CreateRpwf_Screen()
{
    static renderPassWithFramebuffers rpwf;

    VkAttachmentDescription attachmentDescription = {
        .format        = graphicsBase::Base().SwapchainCreateInfo().imageFormat,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp       = VK_ATTACHMENT_STORE_OP_STORE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};
    VkAttachmentReference attachmentReference = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkSubpassDescription  subpassDescription  = {
          .pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS,
          .colorAttachmentCount = 1,
          .pColorAttachments    = &attachmentReference};
    VkSubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        // You may use VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT if synchronization is done by semaphore.
        .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask   = 0,
        .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT};
    VkRenderPassCreateInfo renderPassCreateInfo = {
        .attachmentCount = 1,
        .pAttachments    = &attachmentDescription,
        .subpassCount    = 1,
        .pSubpasses      = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies   = &subpassDependency};
    rpwf.renderPass.Create(renderPassCreateInfo);
    auto CreateFramebuffers = []
    {
        rpwf.framebuffers.resize(graphicsBase::Base().SwapchainImageCount());
        VkFramebufferCreateInfo framebufferCreateInfo = {
            .renderPass      = rpwf.renderPass,
            .attachmentCount = 1,
            .width           = windowSize.width,
            .height          = windowSize.height,
            .layers          = 1};
        for (size_t i = 0; i < graphicsBase::Base().SwapchainImageCount(); i++)
        {
            VkImageView attachment             = graphicsBase::Base().SwapchainImageView(i);
            framebufferCreateInfo.pAttachments = &attachment;
            rpwf.framebuffers[i].Create(framebufferCreateInfo);
        }
    };
    auto DestroyFramebuffers = []
    {
        rpwf.framebuffers.clear();
    };
    CreateFramebuffers();

    ExecuteOnce(rpwf);
    graphicsBase::Base().AddCallback_CreateSwapchain(CreateFramebuffers);
    graphicsBase::Base().AddCallback_DestroySwapchain(DestroyFramebuffers);
    return rpwf;
}

void BootScreen(const char* imagePath, VkFormat imageFormat)
{
    VkExtent2D                 imageExtent;
    std::unique_ptr<uint8_t[]> pImageData = texture2d::LoadFile(imagePath, imageExtent, FormatInfo(imageFormat));
    if (!pImageData)
        return;
    stagingBuffer::BufferData_MainThread(pImageData.get(), FormatInfo(imageFormat).sizePerPixel * imageExtent.width * imageExtent.height);

    semaphore     semaphore_imageIsAvailable;
    fence         fence;
    commandBuffer commandBuffer;
    graphicsBase::Plus().CommandPool_Graphics().AllocateBuffers(commandBuffer);

    graphicsBase::Base().SwapImage(semaphore_imageIsAvailable);
    commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VkExtent2D swapchainImageSize = graphicsBase::Base().SwapchainCreateInfo().imageExtent;
    bool       blit =
        imageExtent.width != swapchainImageSize.width ||
        imageExtent.height != swapchainImageSize.height ||
        imageFormat != graphicsBase::Base().SwapchainCreateInfo().imageFormat;
    imageMemory imageMemory;
    if (blit)
    {
        VkImage image = stagingBuffer::AliasedImage2d_MainThread(imageFormat, imageExtent);
        if (image)
        {
            VkImageMemoryBarrier imageMemoryBarrier = {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                nullptr,
                0,
                VK_ACCESS_TRANSFER_READ_BIT,
                VK_IMAGE_LAYOUT_PREINITIALIZED,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                image,
                {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}};
            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                                 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
        }
        else
        {
            VkImageCreateInfo imageCreateInfo = {
                .imageType   = VK_IMAGE_TYPE_2D,
                .format      = imageFormat,
                .extent      = {imageExtent.width, imageExtent.height, 1},
                .mipLevels   = 1,
                .arrayLayers = 1,
                .samples     = VK_SAMPLE_COUNT_1_BIT,
                .usage       = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT};
            imageMemory.Create(imageCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            VkBufferImageCopy region_copy = {
                .imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
                .imageExtent      = imageCreateInfo.extent};
            imageOperation::CmdCopyBufferToImage(commandBuffer,
                                                 stagingBuffer::Buffer_MainThread(),
                                                 imageMemory.Image(),
                                                 region_copy,
                                                 {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_UNDEFINED},
                                                 {VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL});

            image = imageMemory.Image();
        }
        VkImageBlit region_blit = {
            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
            {{}, {int32_t(imageExtent.width), int32_t(imageExtent.height), 1}},
            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
            {{}, {int32_t(swapchainImageSize.width), int32_t(swapchainImageSize.height), 1}}};
        imageOperation::CmdBlitImage(commandBuffer,
                                     image,
                                     graphicsBase::Base().SwapchainImage(graphicsBase::Base().CurrentImageIndex()),
                                     region_blit,
                                     {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_UNDEFINED},
                                     {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR}, VK_FILTER_LINEAR);
    }
    else
    {
        VkBufferImageCopy region_copy = {
            .imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
            .imageExtent      = {imageExtent.width, imageExtent.height, 1}};
        imageOperation::CmdCopyBufferToImage(commandBuffer,
                                             stagingBuffer::Buffer_MainThread(),
                                             graphicsBase::Base().SwapchainImage(graphicsBase::Base().CurrentImageIndex()),
                                             region_copy,
                                             {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_UNDEFINED},
                                             {VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR});
    }
    commandBuffer.End();

    VkPipelineStageFlags waitDstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkSubmitInfo         submitInfo   = {
                  .waitSemaphoreCount = 1,
                  .pWaitSemaphores    = semaphore_imageIsAvailable.Address(),
                  .pWaitDstStageMask  = &waitDstStage,
                  .commandBufferCount = 1,
                  .pCommandBuffers    = commandBuffer.Address()};
    graphicsBase::Base().SubmitCommandBuffer_Graphics(submitInfo, fence);
    fence.WaitAndReset();
    graphicsBase::Base().PresentImage();

    graphicsBase::Plus().CommandPool_Graphics().FreeBuffers(commandBuffer);
}

std::vector<colorAttachment> cas_msaaToScreen;

const auto& CreateRpwf_MsaaToScreen(VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_4_BIT)
{
    static renderPassWithFramebuffers rpwf;
    static VkSampleCountFlagBits      _sampleCount = sampleCount;

    VkAttachmentDescription attachmentDescriptions[2] = {
        {// Swapchain attachment, used as resolve attachment
         .format         = graphicsBase::Base().SwapchainCreateInfo().imageFormat,
         .samples        = VK_SAMPLE_COUNT_1_BIT,
         .loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
         .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
         .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
         .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
         .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR},
        {
            // Msaa color attachment
            .format         = graphicsBase::Base().SwapchainCreateInfo().imageFormat, // Must be the same format as above
            .samples        = _sampleCount,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // Not VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        }};
    VkAttachmentReference attachmentReferences[2] = {
        {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL},
        {1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};
    VkSubpassDescription subpassDescription = {
        .pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments    = attachmentReferences + 1,
        .pResolveAttachments  = attachmentReferences};
    VkSubpassDependency subpassDependency = {
        .srcSubpass      = VK_SUBPASS_EXTERNAL,
        .dstSubpass      = 0,
        .srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask   = 0,
        .dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT};
    VkRenderPassCreateInfo renderPassCreateInfo = {
        .attachmentCount = 2,
        .pAttachments    = attachmentDescriptions,
        .subpassCount    = 1,
        .pSubpasses      = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies   = &subpassDependency};
    rpwf.renderPass.Create(renderPassCreateInfo);
    auto CreateFramebuffers = []
    {
        cas_msaaToScreen.resize(graphicsBase::Base().SwapchainImageCount());
        rpwf.framebuffers.resize(graphicsBase::Base().SwapchainImageCount());
        for (auto& i : cas_msaaToScreen)
            i.Create(graphicsBase::Base().SwapchainCreateInfo().imageFormat, windowSize, 1, _sampleCount, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT);
        VkFramebufferCreateInfo framebufferCreateInfo = {
            .renderPass      = rpwf.renderPass,
            .attachmentCount = 2,
            .width           = windowSize.width,
            .height          = windowSize.height,
            .layers          = 1};
        for (size_t i = 0; i < graphicsBase::Base().SwapchainImageCount(); i++)
        {
            VkImageView attachments[2] = {
                graphicsBase::Base().SwapchainImageView(i),
                cas_msaaToScreen[i].ImageView(),
            };
            framebufferCreateInfo.pAttachments = attachments;
            rpwf.framebuffers[i].Create(framebufferCreateInfo);
        }
    };
    auto DestroyFramebuffers = []
    {
        cas_msaaToScreen.clear();
        rpwf.framebuffers.clear();
    };
    CreateFramebuffers();

    ExecuteOnce(rpwf);
    graphicsBase::Base().AddCallback_CreateSwapchain(CreateFramebuffers);
    graphicsBase::Base().AddCallback_DestroySwapchain(DestroyFramebuffers);
    return rpwf;
}
} // namespace easyVulkan