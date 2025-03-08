
#include <atomic>

namespace core
{

template <class T>
class RefCounter : public T
{
private:
    std::atomic<unsigned long> m_refCount = 0;

public:
    virtual unsigned long AddRef() override
    {
        return ++m_refCount;
    }

    virtual unsigned long ReleaseRef() override
    {
        unsigned long result = --m_refCount;
        if (result == 0)
        {
            delete this;
        }
        return result;
    }

    virtual unsigned long UseCount() override
    {
        return m_refCount;
    }
};

} // namespace core