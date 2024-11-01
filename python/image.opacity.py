# -*- coding: utf-8 -*-
import os
from PIL import Image

def make_background_transparent(image_path, output_path):
    """
    将图像的背景色设置为完全透明
    :param image_path: 输入图像的路径
    :param output_path: 输出图像的路径
    """
    with Image.open(image_path) as img:
        if img.mode != 'RGBA':
            img = img.convert('RGBA')
        
        # 获取图像数据
        data = img.getdata()
        
        # 找到最常见的颜色（假设这是背景色）
        colors = {}
        for item in data:
            if item[:3] in colors:
                colors[item[:3]] += 1
            else:
                colors[item[:3]] = 1
        
        # 找到出现次数最多的颜色
        background_color = max(colors, key=colors.get)
        
        # 创建新的图像数据列表
        new_data = []
        for item in data:
            # 如果像素的颜色与背景色相同，则将其 alpha 通道设置为 0（完全透明）
            if item[:3] == background_color:
                new_data.append((item[0], item[1], item[2], 0))
            else:
                new_data.append(item)
        
        # 更新图像数据
        img.putdata(new_data)
        
        # 保存图像
        img.save(output_path)

def main():
    # 获取用户输入的图像路径
    input_path = input("请输入图像文件路径: ")
    
    # 检查文件是否存在
    if not os.path.exists(input_path):
        print("文件不存在，请检查路径是否正确。")
        return
    
    # 生成输出文件路径
    file_name, file_ext = os.path.splitext(os.path.basename(input_path))
    output_path = os.path.join(os.path.dirname(input_path), "{}_opacity{}".format(file_name, file_ext))
    
    # 将背景色设置为完全透明并保存
    make_background_transparent(input_path, output_path)
    print("图像已保存到: {}".format(output_path))

if __name__ == "__main__":
    main()