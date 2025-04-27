#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"      // RapidJSON 核心解析器
#include "rapidjson/istreamwrapper.h" // 用于文件流包装

using namespace rapidjson;

int main() {
    // 1. 打开文件
    std::ifstream ifs("world.json");
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    // 2. 将文件流包装为 RapidJSON 的输入流
    IStreamWrapper isw(ifs);

    // 3. 解析 JSON
    Document doc;
    doc.ParseStream(isw);

    // 检查解析是否成功
    if (doc.HasParseError()) {
        std::cerr << "JSON parse error: " << doc.GetParseError() << std::endl;
        return 1;
    }

    // 4. 提取数据
    // (1) 顶层字段
    int type = doc["type"].GetInt();
    std::string name = doc["name"].GetString();
    int octreeMaxDepth = doc["octreeMaxDepth"].GetInt();

    // (2) 嵌套对象: boundingBox
    const Value& boundingBox = doc["boundingBox"];
    const Value& min = boundingBox["min"];
    const Value& max = boundingBox["max"];

    float min_x = min["x"].GetFloat();
    float min_y = min["y"].GetFloat();
    float min_z = min["z"].GetFloat();

    float max_x = max["x"].GetFloat();
    float max_y = max["y"].GetFloat();
    float max_z = max["z"].GetFloat();

    // (3) 数组: assets
    const Value& assets = doc["assets"];
    for (SizeType i = 0; i < assets.Size(); i++) {
        const Value& asset = assets[i];
        std::string asset_name = asset["name"].GetString();
        std::string asset_path = asset["path"].GetString();

        const Value& position = asset["position"];
        float pos_x = position[0].GetFloat();
        float pos_y = position[1].GetFloat();
        float pos_z = position[2].GetFloat();

        std::cout << "Asset: " << asset_name << ", Path: " << asset_path
                  << ", Position: (" << pos_x << ", " << pos_y << ", " << pos_z << ")" << std::endl;
    }

    // (4) 嵌套对象: camera
    const Value& camera = doc["camera"];
    const Value& cam_pos = camera["position"];
    float cam_x = cam_pos[0].GetFloat();
    float cam_y = cam_pos[1].GetFloat();
    float cam_z = cam_pos[2].GetFloat();

    // 5. 打印结果
    std::cout << "Type: " << type << "\n"
              << "Name: " << name << "\n"
              << "BoundingBox Min: (" << min_x << ", " << min_y << ", " << min_z << ")\n"
              << "BoundingBox Max: (" << max_x << ", " << max_y << ", " << max_z << ")\n"
              << "OctreeMaxDepth: " << octreeMaxDepth << "\n"
              << "Camera Position: (" << cam_x << ", " << cam_y << ", " << cam_z << ")" << std::endl;

    return 0;
}