#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

std::vector<glm::vec3> getFrustumCorners(
    const glm::mat4& viewMatrix,
    const glm::mat4& projectionMatrix)
{
    // NDC 空间的8个角点（近平面 + 远平面）
    std::vector<glm::vec4> ndcCorners = {
        // 近平面（z = -1）
        glm::vec4(-1, -1, -1, 1),
        glm::vec4(1, -1, -1, 1),
        glm::vec4(-1, 1, -1, 1),
        glm::vec4(1, 1, -1, 1),
        // 远平面（z = 1）
        glm::vec4(-1, -1, 1, 1),
        glm::vec4(1, -1, 1, 1),
        glm::vec4(-1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1)};

    // 计算 VP 的逆矩阵（裁剪空间→世界空间）
    glm::mat4 vp    = projectionMatrix * viewMatrix;

    glm::mat4 invVP = glm::inverse(vp);

    // 转换到世界空间
    std::vector<glm::vec3> worldCorners;
    for (const auto& corner : ndcCorners)
    {
        glm::vec4 worldPos = invVP * corner;
        worldPos /= worldPos.w; // 透视除法
        worldCorners.push_back(glm::vec3(worldPos));
    }

    return worldCorners;
}

int main()
{
    // 示例：透视投影相机

    auto eye    = glm::vec3(0, 0, 5);
    auto center = glm::vec3(0, 0, 0);
    auto up     = glm::vec3(0, 1, 0);

    glm::mat4 view = glm::lookAt(eye, center, up);

    auto  fov    = glm::radians(60.0f);
    float aspect = 16.0f / 9.0f;
    float near   = 0.1f;
    float far    = 100.0f;

    glm::mat4 projection = glm::perspective(fov, aspect, near, far);

    // 获取视锥的8个角点
    auto corners = getFrustumCorners(view, projection);

    // 输出结果
    for (int i = 0; i < 8; i++)
    {
        printf("Corner %d: (%.2f, %.2f, %.2f)\n",
               i, corners[i].x, corners[i].y, corners[i].z);
    }

    return 0;
}