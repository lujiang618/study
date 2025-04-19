namespace core
{
class IResource
{
protected:
    IResource()          = default;
    virtual ~IResource() = default;

public:
    virtual unsigned long AddRef()     = 0;
    virtual unsigned long ReleaseRef() = 0;
    virtual unsigned long UseCount()   = 0;

    IResource(const IResource&)             = delete;
    IResource(const IResource&&)            = delete;
    IResource& operator=(const IResource&)  = delete;
    IResource& operator=(const IResource&&) = delete;
};
} // namespace core