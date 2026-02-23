#pragma once
#include <cstdint>
#include <cstddef>
#include <bit>
#include <functional>
#include <mutex>
#include <string>
#include <wtypes.h>
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#include <windows.h>
#include <core/Logger.hpp>
#include <libhat/memory_protector.hpp>
#include <libhat/process.hpp>

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace fi {
    using RVA = uint32_t;

    template <typename T> class RVAPtr {
    private:
        static T* resolve_ptr(const RVA rva) {
            return reinterpret_cast<T*>(
                reinterpret_cast<uint8_t*>(&__ImageBase) + rva);
        }

    public:
        RVAPtr(const RVA rva) : resolved(resolve_ptr(rva)) {}
        RVAPtr(std::nullptr_t) : resolved(nullptr) {}
        RVAPtr() = default;

        T* operator->() { return this->resolved; }

        T* operator->() const { return this->resolved; }

        T& operator*() { return *this->resolved; }

        T& operator*() const { return *this->resolved; }

        operator T* () { return this->resolved; }

        operator T* () const { return this->resolved; }

        bool operator==(const RVAPtr<T> other) const {
            return other.resolved == this->resolved;
        }

        bool operator==(T* other) const { return other == this->resolved; }

        bool operator!=(const RVAPtr<T> other) const { return !(*this == other); }

        bool operator!=(T* other) const { return !(*this == other); }

        T* get() const { return this->resolved; }

        T operator[](size_t index) { return this->resolved[index]; }

        RVAPtr operator++() {
            const auto ptr = *this;
            ++this->resolved;
            return ptr;
        }

        RVAPtr operator++(int) {
            const auto ptr = *this;
            ++this->resolved;
            return ptr;
        }

    private:
        T* resolved{ nullptr };
    };

    using RVAString = RVAPtr<const char>;

    using IATEntry = FARPROC*;
    enum class Attributes : DWORD { RVA = 0x1 };

    IMAGE_NT_HEADERS* image_header_from_base();

    struct ImgDelayDescr {
        DWORD grAttrs;
        RVA rvaDLLName;
        RVA rvaHmod;
        RVA rvaIAT;
        RVA rvaINT;
        RVA rvaBoundIAT;
        RVA rvaUnloadIAT;
        DWORD dwTimeStamp;

        static const ImgDelayDescr* idd_from_base();
    };

    struct IDD {
        Attributes attributes;
        RVAString module_name;
        RVAPtr<HMODULE> module_handle;
        RVAPtr<const IMAGE_THUNK_DATA> import_address_table;
        RVAPtr<const IMAGE_THUNK_DATA> import_name_table;
        RVAPtr<const IMAGE_THUNK_DATA> bound_import_address_table;
        RVAPtr<const IMAGE_THUNK_DATA> unload_import_address_table;

        explicit IDD(const ImgDelayDescr* descr);

        [[nodiscard]] size_t iat_size() const;

        uintptr_t offset_in_iat(IATEntry iat_entry) const;
    };

    class IATWrite {
    public:
        IATWrite(uintptr_t iat_address, const std::function<void()>& callback, const IDD& idd);
        IATWrite(const IATWrite&) = delete;
        IATWrite& operator=(const IATWrite&) = delete;
        IATWrite(IATWrite&&) = default;

    private:
        static std::mutex IatMutex;
    };
}

struct ImportResolver {
private:
    using ResolveAddressPtrFn = uintptr_t(*)(const char* mangledName);
public:
    std::string dll_name{};
    ResolveAddressPtrFn resolve{ nullptr };
};

class FakeImports {
public:
    static inline ImportResolver resolver;
public:
    static void construct(ImportResolver&& config);
    static void load_all_imports();
};