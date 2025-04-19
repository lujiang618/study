namespace core
{
template <typename T>
class WeakPtr
{
public:
    WeakPtr()
        : ptr_(nullptr)
        , refCounter_(nullptr)
    {
    }

    WeakPtr(const RefCountPtr<T>& other)
        : ptr_(other.Get())
        , refCounter_(other.Get() ? static_cast<RefCounter<T>*>(other.Get()) : nullptr)
    {
        auto ptr = other.Get();
        if (refCounter_)
        {
            refCounter_->AddRef();
        }
    }

    ~WeakPtr()
    {
        // 引用为0时，会在外部先释放refCounter_， 这里在调用方法会crash
        if (refCounter_)
        {
            refCounter_->ReleaseRef();
        }
    }

    RefCountPtr<T> Lock() const
    {
        if (refCounter_ && refCounter_->UseCount() > 0)
        {
            return RefCountPtr<T>(ptr_);
        }
        return nullptr;
    }

private:
    T*             ptr_;
    RefCounter<T>* refCounter_;
};
} // namespace core