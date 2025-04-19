//  copyright daxiangyun, Inc. All Rights Reserved.

#pragma once

#include <string>

#include "RefCountPtr.h"
#include "RefCounter.h"
#include "Resource.h"

namespace core
{
class Device;

class Texture : public RefCounter<IResource>
{
public:
    Texture(std::string name)
        : name(name)
    {
    }

    ~Texture() {
        std::cout << "release : " << name << std::endl;
    }

    std::string GetName()
    {
        return name;
    }

    std::string name;
};

typedef RefCountPtr<Texture> TextureHandle;
} // namespace core