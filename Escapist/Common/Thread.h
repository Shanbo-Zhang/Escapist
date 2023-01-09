//
// Created by Escap on 1/8/2023.
//

#ifndef ESCAPIST_THREAD_H
#define ESCAPIST_THREAD_H

#include "../General.h"

namespace OS {
#ifdef ESCAPIST_OS_WINDOWS

    Handle CreateThread() noexcept {

    }

#endif
}

class Thread {
private:
    Handle hThread;
    unsigned long threadID;
    bool started = false;
    bool finished = false;

    static DWORD __stdcall Start0(void *argv) {
        Thread *thr = (Thread *) argv;
        if (thr) {
            thr->Run();
        }
        thr->finished = true;
    }

public:
    Thread() noexcept: hThread(nullptr), threadID(0) {}

    Thread(const Thread &other) noexcept = delete;

    void Start() {
        hThread = ::CreateThread(nullptr, 0, Thread::Start0, this, 0, &threadID);
        if (hThread) {
            started = true;
        }
    }

    void Terminate(int exitCode) noexcept {
        if (hThread && ::TerminateThread(hThread, exitCode)) {
            started = false;
        }
    }

    void Wait(DWORD ms) {
        if (hThread && started) {
            ::WaitForSingleObject(hThread, ms);
        }
    }

    void Wait() {
        Thread::Wait(INFINITE);
    }

    virtual void Run() = 0;
};

#endif //ESCAPIST_THREAD_H
