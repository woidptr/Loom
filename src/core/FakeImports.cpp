#include "FakeImports.hpp"

static std::atomic_bool batching = false;

extern "C" {
    FARPROC WINAPI __delayLoadHelper2(const fi::ImgDelayDescr* pidd, FARPROC* ppfnIATEntry) {
        $log_debug("Resolving: {:x}", std::bit_cast<uintptr_t>(ppfnIATEntry));
        fi::IDD idd{ pidd };
        if (idd.attributes != fi::Attributes::RVA) {
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
        fi::RVAPtr<IMAGE_IMPORT_BY_NAME> name_struct{
            static_cast<fi::RVA>(thunk_data.u1.AddressOfData)
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
        fi::IATWrite writer{
            std::bit_cast<uintptr_t>(ppfnIATEntry),
            [&] { *ppfnIATEntry = std::bit_cast<FARPROC>(resolved_address); },
            idd
        };

        return reinterpret_cast<FARPROC>(resolved_address);
    }
}

namespace fi {
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

    std::mutex IATWrite::IatMutex = std::mutex{};

    IATWrite::IATWrite(uintptr_t iat_address, const std::function<void()>& callback, const IDD& idd) {
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

    const ImgDelayDescr* ImgDelayDescr::idd_from_base() {
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

    IDD::IDD(const ImgDelayDescr* descr) {
        this->attributes = static_cast<Attributes>(descr->grAttrs);
        this->module_name = descr->rvaDLLName;
        this->module_handle = descr->rvaHmod;
        this->import_address_table = descr->rvaIAT;
        this->import_name_table = descr->rvaINT;
        this->bound_import_address_table = descr->rvaBoundIAT;
        this->unload_import_address_table = descr->rvaUnloadIAT;
    }

    size_t IDD::iat_size() const {
        size_t ret = 0;
        const auto* itd = this->import_address_table.get();
        while (itd->u1.Function) {
            ret++;
            itd++;
        }
        return ret;
    }

    uintptr_t IDD::offset_in_iat(const IATEntry iat_entry) const {
        const auto* iat_entry_true = std::bit_cast<const IMAGE_THUNK_DATA*>(iat_entry);
        return std::bit_cast<uintptr_t>(iat_entry_true - this->import_address_table);
    }
}

void FakeImports::construct(ImportResolver&& resolver) {
    FakeImports::resolver = std::move(resolver);
}

void FakeImports::load_all_imports() {
    const auto raw_idd = fi::ImgDelayDescr::idd_from_base();
    if (!raw_idd) return;

    const fi::IDD idd{ raw_idd };
    auto iat_entry = idd.import_address_table;
    const auto iat_size = idd.iat_size();
    const auto iat_end = iat_entry + iat_size;

    for (; iat_entry < iat_end; ++iat_entry) {
        __delayLoadHelper2(raw_idd, const_cast<FARPROC*>(reinterpret_cast<const FARPROC*>(iat_entry.get())));
    }
}