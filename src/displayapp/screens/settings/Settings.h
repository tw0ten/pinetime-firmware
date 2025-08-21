#pragma once

#include <array>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/ScreenList.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/screens/List.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class Settings : public Screen {
      public:
        Settings(DisplayApp* app, Pinetime::Controllers::Settings& settingsController);
        ~Settings() override;

        bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;

      private:
        DisplayApp* app;
        auto CreateScreenList() const;
        std::unique_ptr<Screen> CreateScreen(unsigned int screenNum) const;

        Controllers::Settings& settingsController;

        static constexpr int entriesPerScreen = 4;

        // Increment this when more space is needed
        static constexpr int nScreens = 4;

        static constexpr std::array<List::Applications, entriesPerScreen * nScreens> entries {{
          {Symbols::list, "system", Apps::SysInfo},
          {Symbols::check, "firmware", Apps::FirmwareValidation},
          {Symbols::batteryHalf, "battery", Apps::BatteryInfo},
          {Symbols::home, "clock", Apps::SettingWatchFace},

          {Symbols::sun, "display", Apps::SettingDisplay},
          {Symbols::eye, "wake up", Apps::SettingWakeUp},
          {Symbols::shoe, "steps", Apps::SettingSteps},
          {Symbols::clock, "time", Apps::SettingSetDateTime},

          {Symbols::cloudSunRain, "weather", Apps::SettingWeatherFormat},
          {Symbols::clock, "time format", Apps::SettingTimeFormat},
          {Symbols::clock, "chimes", Apps::SettingChimes},
          {Symbols::tachometer, "shake", Apps::SettingShakeThreshold},

          {Symbols::bluetooth, "bluetooth", Apps::SettingBluetooth},
        }};
        ScreenList<nScreens> screens;
      };
    }
  }
}
