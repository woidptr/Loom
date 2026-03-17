#include "FakeImports.hpp"

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {
    using RVA = uint32_t;

    template <typename T>
    class RVAPtr {
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

    IMAGE_NT_HEADERS* image_header_from_base() {
        auto dos = &__ImageBase;

        if (dos->e_magic != IMAGE_DOS_SIGNATURE) {
            return nullptr;
        }

        auto nt = std::bit_cast<IMAGE_NT_HEADERS*>(
            std::bit_cast<std::byte*>(dos) + dos->e_lfanew);

        if (nt->Signature != IMAGE_NT_SIGNATURE) {
            return nullptr;
        }

        return nt;
    }

    struct ImgDelayDescr {
        DWORD grAttrs;
        RVA rvaDLLName;
        RVA rvaHmod;
        RVA rvaIAT;
        RVA rvaINT;
        RVA rvaBoundIAT;
        RVA rvaUnloadIAT;
        DWORD dwTimeStamp;

        static const ImgDelayDescr* idd_from_base() {
            auto* image_header = image_header_from_base();
            if (!image_header) {
                return nullptr;
            }

            const auto& entry = image_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
            if (entry.Size == 0) {
                return nullptr;
            }

            auto current_idd = RVAPtr<const ImgDelayDescr>(entry.VirtualAddress);

            RVAString name{ current_idd->rvaDLLName };
            while (current_idd->rvaDLLName != 0) {

                if (FakeImports::resolver.dll_name == name.get()) {
                    return current_idd;
                }

                ++current_idd;
                (name = current_idd->rvaDLLName);
            }

            return nullptr;
        }
    };

    struct IDD {
        Attributes attributes;
        RVAString module_name;
        RVAPtr<HMODULE> module_handle;
        RVAPtr<const IMAGE_THUNK_DATA> import_address_table;
        RVAPtr<const IMAGE_THUNK_DATA> import_name_table;
        RVAPtr<const IMAGE_THUNK_DATA> bound_import_address_table;
        RVAPtr<const IMAGE_THUNK_DATA> unload_import_address_table;

        explicit IDD(const ImgDelayDescr* descr) {
            this->attributes = static_cast<Attributes>(descr->grAttrs);
            this->module_name = descr->rvaDLLName;
            this->module_handle = descr->rvaHmod;
            this->import_address_table = descr->rvaIAT;
            this->import_name_table = descr->rvaINT;
            this->bound_import_address_table = descr->rvaBoundIAT;
            this->unload_import_address_table = descr->rvaUnloadIAT;
        }

        [[nodiscard]] size_t iat_size() const {
            size_t ret = 0;
            const auto* itd = this->import_address_table.get();
            while (itd->u1.Function) {
                ret++;
                itd++;
            }
            return ret;
        }

        uintptr_t offset_in_iat(IATEntry iat_entry) const {
            const auto* iat_entry_true = std::bit_cast<const IMAGE_THUNK_DATA*>(iat_entry);
            return std::bit_cast<uintptr_t>(iat_entry_true - this->import_address_table);
        }
    };

    class IATWrite {
    private:
        static inline std::mutex IatMutex;
    public:
        IATWrite(uintptr_t iat_address, const std::function<void()>& callback, const IDD& idd) {
            IATWrite::IatMutex.lock();

            const auto memory_size = idd.iat_size() * sizeof(IMAGE_THUNK_DATA);
            hat::memory_protector memory_prot{
                std::bit_cast<uintptr_t>(idd.import_address_table),
                memory_size,
                hat::protection::Read | hat::protection::Write
            };

            callback();
            IATWrite::IatMutex.unlock();
        }

        IATWrite(const IATWrite&) = delete;
        IATWrite& operator=(const IATWrite&) = delete;
        IATWrite(IATWrite&&) = default;
    };
}

extern "C" {
    FARPROC WINAPI __delayLoadHelper2(const ImgDelayDescr* pidd, FARPROC* ppfnIATEntry) {
        $log_debug("Resolving: {:x}", std::bit_cast<uintptr_t>(ppfnIATEntry));
        IDD idd{ pidd };
        if (idd.attributes != Attributes::RVA) {
            __debugbreak();
            return nullptr;
        }

        const auto iat_offset = idd.offset_in_iat(ppfnIATEntry);
        const auto int_offset = iat_offset;

        auto& thunk_data = idd.import_name_table[int_offset];

        if (IMAGE_SNAP_BY_ORDINAL(thunk_data.u1.Ordinal)) {
            // const auto resolved_address = fi::FakeImportConfig::config().resolve_address(static_cast<uint32_t>(thunk_data.u1.Ordinal));

            // fi::IATWrite writer{ std::bit_cast<uintptr_t>(ppfnIATEntry), [&] {*ppfnIATEntry = std::bit_cast<FARPROC>(resolved_address); }, idd };

            // return reinterpret_cast<FARPROC>(resolved_address);
            __debugbreak();
            return nullptr;
        }
        RVAPtr<IMAGE_IMPORT_BY_NAME> name_struct{
            static_cast<RVA>(thunk_data.u1.AddressOfData)
        };

        const char* funcName = name_struct->Name; // Access directly via -> operator

        // 6. Call User Resolver
        uintptr_t resolved_address = 0;
        if (auto resolver = FakeImports::resolver.resolve) {
            $log_debug("{}", funcName);
            resolved_address = resolver(funcName);
        }

        if (!resolved_address) {
            $log_error("Failed to resolve: {}", funcName);
            __debugbreak();
            return nullptr;
        }

        // 7. Patch IAT
        IATWrite writer{
            std::bit_cast<uintptr_t>(ppfnIATEntry),
            [&] { *ppfnIATEntry = std::bit_cast<FARPROC>(resolved_address); },
            idd
        };

        return reinterpret_cast<FARPROC>(resolved_address);
    }
}

void FakeImports::load_all_imports(const ImportResolver& resolver) {
    FakeImports::resolver = resolver;

    const auto raw_idd = ImgDelayDescr::idd_from_base();
    if (!raw_idd) return;

    const IDD idd{ raw_idd };
    auto iat_entry = idd.import_address_table;
    const auto iat_size = idd.iat_size();
    const auto iat_end = iat_entry + iat_size;

    for (; iat_entry < iat_end; ++iat_entry) {
        __delayLoadHelper2(raw_idd, const_cast<FARPROC*>(reinterpret_cast<const FARPROC*>(iat_entry.get())));
    }
}