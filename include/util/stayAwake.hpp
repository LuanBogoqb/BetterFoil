#pragma once

// BetterFoil stay-awake module.
// Keeps the console from sleeping while BetterFoil is open and, during installs,
// turns the screen backlight off after an idle period to save the panel while the
// install keeps running. Any controller/touch input wakes the screen back up.

namespace inst::power {
    // Lifecycle (call once from initApp / deinitApp).
    void init();
    void exit();

    // Per-frame tick. Register with mainApp->AddThread(inst::power::update).
    void update();

    // Mark an install as active/inactive (resets the idle timer; restores the
    // backlight when the install ends).
    void setInstalling(bool installing);

    // Enable/disable the whole feature at runtime (used by the options toggle).
    void setEnabled(bool enabled);
    bool isEnabled();
}
