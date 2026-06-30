#include "switch.h"
#include "util/stayAwake.hpp"
#include "util/config.hpp"

namespace inst::power {
    namespace {
        bool g_enabled = false;
        bool g_lblReady = false;
        bool g_installing = false;
        bool g_backlightOff = false;
        bool g_autoSleepApplied = false;

        PadState g_pad;
        u64 g_lastInputTick = 0;
        u64 g_tickFreq = 1;

        constexpr u64 kIdleSeconds = 15;          // dim the screen after this idle time during installs
        constexpr u64 kFadeNs = 300000000ULL;     // 0.3s backlight fade

        bool anyInput() {
            padUpdate(&g_pad);
            if (padGetButtonsDown(&g_pad) || padGetButtons(&g_pad)) return true;
            HidTouchScreenState ts = {0};
            if (hidGetTouchScreenStates(&ts, 1) && ts.count > 0) return true;
            return false;
        }

        void screenOn() {
            if (!g_backlightOff) return;
            if (g_lblReady) {
                lblSwitchBacklightOn(kFadeNs);
                lblEnableAutoBrightnessControl();
            }
            g_backlightOff = false;
        }

        void screenOff() {
            if (g_backlightOff) return;
            if (g_lblReady) {
                lblDisableAutoBrightnessControl();
                lblSwitchBacklightOff(kFadeNs);
            }
            g_backlightOff = true;
        }

        void applyAutoSleep(bool disabled) {
            if (disabled && !g_autoSleepApplied) {
                appletSetAutoSleepDisabled(true);
                g_autoSleepApplied = true;
            } else if (!disabled && g_autoSleepApplied) {
                appletSetAutoSleepDisabled(false);
                g_autoSleepApplied = false;
            }
        }
    }

    void init() {
        g_tickFreq = armGetSystemTickFreq();
        g_lastInputTick = armGetSystemTick();
        // The app (Plutonium) already calls padConfigureInput globally; we only
        // need our own PadState reading the default (player 1 + handheld).
        padInitializeDefault(&g_pad);
        g_lblReady = R_SUCCEEDED(lblInitialize());
        g_enabled = inst::config::stayAwake;
        if (g_enabled) applyAutoSleep(true);
    }

    void exit() {
        screenOn();
        applyAutoSleep(false);
        if (g_lblReady) { lblExit(); g_lblReady = false; }
    }

    void setEnabled(bool enabled) {
        g_enabled = enabled;
        if (enabled) {
            applyAutoSleep(true);
        } else {
            screenOn();
            applyAutoSleep(false);
        }
    }

    bool isEnabled() { return g_enabled; }

    void setInstalling(bool installing) {
        g_installing = installing;
        g_lastInputTick = armGetSystemTick();
        if (!installing) screenOn();
    }

    void update() {
        if (!g_enabled) {
            if (g_backlightOff) screenOn();
            return;
        }
        // Only manage the backlight during installs; otherwise just hold no-sleep.
        if (g_installing) {
            if (anyInput()) {
                g_lastInputTick = armGetSystemTick();
                screenOn();
            } else if (!g_backlightOff) {
                u64 elapsed = (armGetSystemTick() - g_lastInputTick) / g_tickFreq;
                if (elapsed >= kIdleSeconds) screenOff();
            }
        } else if (g_backlightOff) {
            screenOn();
        }
    }
}
