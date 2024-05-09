#pragma once

namespace PAPI
{
    template<typename T> struct Handle
    {
        Handle() {}
        Handle(T* Ptr)
        {
            BindValuePtr = Ptr;
        }

        bool IsValid()
        {
            return BindValuePtr;
        }
        void Set(T Value)
        {
            *BindValuePtr = Value;
        }
        T Get()
        {
            return *BindValuePtr;
        }
        void Reset()
        {
            BindValuePtr = nullptr;
        }

    private:
        T* BindValuePtr = nullptr;
    };
}   // namespace PAPI
