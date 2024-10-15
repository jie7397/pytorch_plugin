#include <functional>
#include <link.h>
#include <list>
#include <mutex>
#include <vector>
#include <cstring>
#include <cxxabi.h>

#include "hook_register.h"

namespace hook {

typedef ElfW(Rela) RelaAddr;
typedef ElfW(Sym) SymAddr;

struct PltInfo {
    std::string lib_name;
    char *base_addr;
    int pltrelsz;
    char *dynstr;
    RelaAddr *rela_plt; // .rela.plt 段起始地址
    SymAddr *dynsym;    // .dynsym 段起始地址
};

int callback(struct dl_phdr_info *info, size_t size, void *data) {

    const char *lib_name = info->dlpi_name; // 动态库名称
    char *base_addr = reinterpret_cast<char *>(info->dlpi_addr);
    int pltrelsz = 0;          // .rela.plt 段大小
    char *dynstr = NULL;       // .dynstr 段起始地址
    RelaAddr *rela_plt = NULL; // .rela.plt 段起始地址
    SymAddr *dynsym = NULL;    // .dynsym 段起始地址

    if (strstr(lib_name, "libtorch_cuda.so") == nullptr) {
        return 0;
    }

    LOG_WARN("lib name is : " + std::string(lib_name));

    // 遍历当前动态库中所有段的信息
    for (size_t i = 0; i < info->dlpi_phnum; i++) {
        const ElfW(Phdr) *phdr = &info->dlpi_phdr[i];

        // 如果不是 .dynamic 段则跳过
        if (phdr->p_type != PT_DYNAMIC) {
            continue;
        }

        int dynEntryCount = phdr->p_memsz / sizeof(ElfW(Dyn));
        ElfW(Dyn) *dyn = (ElfW(Dyn) *)(phdr->p_vaddr + info->dlpi_addr);

        // 遍历获取 .dynamic 段中的信息
        for (int j = 0; j < dynEntryCount; j++) {
            ElfW(Dyn) *entry = &dyn[j];
            switch (dyn->d_tag) {
            case DT_PLTRELSZ: {
                pltrelsz = dyn->d_un.d_val;
                break;
            }
            case DT_JMPREL: {
                rela_plt = (ElfW(Rela) *)(dyn->d_un.d_ptr);
                break;
            }
            case DT_STRTAB: {
                dynstr = (char *)(dyn->d_un.d_ptr);
                break;
            }
            case DT_SYMTAB: {
                dynsym = (ElfW(Sym) *)(dyn->d_un.d_ptr);
                break;
            }
            }
            dyn++;
        }
    }


  // 遍历 .rela.plt 段中的信息
  int relaEntryCount = pltrelsz / sizeof(ElfW(Rela));
  for (int i = 0; i < relaEntryCount; i++) {
    ElfW(Rela) *entry = &rela_plt[i];

    // 获取 .dynsym 中的索引
    int r_sym = ELF64_R_SYM(entry->r_info);

    // 获取 .dynstr 中的索引
    int st_name = dynsym[r_sym].st_name;

    // 获取当前的符号名称
    char *name = &dynstr[st_name];

    if (strstr(name, "deregisterImpl_") == nullptr) {
        LOG_INFO("symbol is : {}", name);
        continue;
    }

    // int status = 0;
    // // __cxa_demangle will allocate memory for the demangled name using malloc.
    // // We need to free this memory ourselves.
    // char* demangled_name = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    
    // if(status == 0 && demangled_name != nullptr) {
    //     std::cout << "Demangled name: " << demangled_name << std::endl;
    // } else {
    //     std::cerr << "Failed to demangle name: " << name <<std::endl;
    // }

    // std::cout << "symbol is : " << name << std::endl;


//     // std::cout << "===== replace symbol is : " << name << std::endl;


//     // void* func = ;

//     // 如果不是 `puts` 符号, 则跳过
//     // if (!func) {
//     //   continue;
//     // }

//     // 计算 `puts` 符号在 .got.plt 表中的位置
//     // uintptr_t hook_point = (uintptr_t)(info->dlpi_addr + entry->r_offset);

//     // 修改 `puts` 符号的跳转地址为: my_printf
//     // *(void **)hook_point = func;
  }

  return 0;
}


void hook() { dl_iterate_phdr(callback, NULL); }

}