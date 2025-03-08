
#include <map>
#include <memory>

#include "Texture.h"
#include "WeakPtr.h"

namespace core
{
class TextureMgr
{
public:
    TextureMgr()
    {
    }

    ~TextureMgr()
    {

    }

    int Size()
    {
        return textures.size();
    }

    TextureHandle GetTexture(const std::string& name)
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            auto ptr = it->second;
            // if (!ptr)
            // {
            //     textures.erase(it);
            //     std::cout << "delete, remain count:" << textures.size() << std::endl;
            //     return nullptr;
            // }
            return ptr;
        }
        return nullptr;
    }

    TextureHandle CreateTexture(const std::string& name)
    {
        auto tex = new Texture(name);

        // TextureHandle handle(tex);

        // std::cout << "create, remain  size:" << textures.size() << std::endl;
        // std::cout << "create, ref    count:" << handle->UseCount() << std::endl;

        // textures[name] = WeakPtr<Texture>(handle);
        textures[name] = RefCountPtr<Texture>(tex);
        std::cout << "create, cache  count:" << textures[name]->UseCount() << std::endl;


        return textures[name];
    }

    // 这样mgr会持有1次计数
    std::map<std::string, TextureHandle> textures;
};

} // namespace core
