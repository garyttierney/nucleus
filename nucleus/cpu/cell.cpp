/**
 * (c) 2015 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "cell.h"
#include "nucleus/config.h"
#include "nucleus/emulator.h"
#include "nucleus/cpu/backend/x86/x86_compiler.h"
#include "nucleus/cpu/frontend/ppu/ppu_thread.h"
#include "nucleus/cpu/frontend/ppu/ppu_tables.h"
#include "nucleus/logger/logger.h"

#include <algorithm>

#ifdef NUCLEUS_PLATFORM_WINDOWS
#define thread_local __declspec(thread)
#elif NUCLEUS_PLATFORM_OSX
#define thread_local __thread
#endif

namespace cpu {

thread_local CellThread* g_this_thread = nullptr;

Cell::Cell()
{
#if defined(NUCLEUS_ARCH_X86)
    compiler = std::make_unique<backend::x86::X86Compiler>();
#elif defined(NUCLEUS_ARCH_ARM)
    compiler = std::make_unique<backend::arm::ARMCompiler>();
#else
    logger.error(LOG_CPU, "No backend available for this architecture.");
#endif
}

void Cell::init()
{
    if (config.ppuTranslator == PPU_TRANSLATOR_RECOMPILER) {
        // Global target triple
        llvm::Triple triple(llvm::sys::getProcessTriple());
        if (triple.getOS() == llvm::Triple::OSType::Win32) {
            triple.setObjectFormat(llvm::Triple::ObjectFormatType::ELF);
        }

        // Global Nucleus module
        module = new llvm::Module("Nucleus", llvm::getGlobalContext());
        module->setTargetTriple(triple.str());

        // Global variables
        module->getOrInsertGlobal("memoryBase", llvm::Type::getInt64Ty(llvm::getGlobalContext()));
        llvm::GlobalVariable* memoryBase = module->getNamedGlobal("memoryBase");
        memoryBase->setConstant(true);
        memoryBase->setLinkage(llvm::GlobalValue::ExternalLinkage);
        memoryBase->setInitializer(llvm::ConstantInt::get(module->getContext(), llvm::APInt(64, (U64)nucleus.memory.getBaseAddr())));
    }
}

CellThread* Cell::addThread(CellThreadType type, U32 entry=0)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    CellThread* thread;

    switch (type) {
    case CELL_THREAD_PPU:
        thread = new frontend::ppu::Thread(entry);
        ppu_threads.push_back(dynamic_cast<frontend::ppu::Thread*>(thread));
        break;

    case CELL_THREAD_SPU:
        logger.error(LOG_CPU, "Unimplemented Cell thread type");
        return nullptr;

    case CELL_THREAD_RAWSPU:
        logger.error(LOG_CPU, "Unimplemented Cell thread type");
        return nullptr;

    default:
        logger.error(LOG_CPU, "Unsupported Cell thread type");
        return nullptr;
    }

    // Give the thread a unused ID
    while (m_thread_ids.find(m_current_id) != m_thread_ids.end()) {
        m_current_id += 1;
    }
    thread->id = m_current_id;
    m_thread_ids.insert(m_current_id);
    m_current_id += 1;

    return thread;
}

CellThread* Cell::getThread(U64 id)
{
    for (CellThread* thread : ppu_threads) {
        if (thread->id == id) {
            return thread;
        }
    }
    return nullptr;
}

void Cell::removeThread(U64 id)
{
    // Remove thread
    ppu_threads.erase(
        std::remove_if(ppu_threads.begin(), ppu_threads.end(), [&](CellThread* thread){ return thread->id == id; }),
        ppu_threads.end());

    // Remove thread ID
    m_thread_ids.erase(id);
}

CellThread* Cell::getCurrentThread()
{
    return g_this_thread;
}

void Cell::setCurrentThread(CellThread* thread)
{
    g_this_thread = thread;
}

void Cell::run()
{
    for (CellThread* thread : ppu_threads) {
        thread->run();
    }
}

void Cell::pause()
{
    for (CellThread* thread : ppu_threads) {
        thread->pause();
    }
}

void Cell::stop()
{
    for (CellThread* thread : ppu_threads) {
        thread->stop();
    }
}

}  // namespace cpu
