#pragma once

#include "displayapp/screens/Screen.h"
#include "components/brightness/BrightnessController.h"
#include "systemtask/SystemTask.h"
#include "systemtask/WakeLock.h"
#include <cstdint>
#include <lvgl/lvgl.h>
#include "Symbols.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class FlashLight : public Screen {
      public:
        FlashLight(System::SystemTask& systemTask, Controllers::BrightnessController& brightness);
        ~FlashLight() override;

        bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;
        void Toggle();

      private:
        void SetIndicators();
        void SetColors();

        Pinetime::System::WakeLock wakeLock;
        Controllers::BrightnessController& brightnessController;

        Controllers::BrightnessController::Levels brightnessLevel = Controllers::BrightnessController::Levels::High;
        Controllers::BrightnessController::Levels previousBrightnessLevel;

        lv_obj_t* flashLight;
        lv_obj_t* backgroundAction;
        lv_obj_t* indicators[3];
        bool isOn = false;
      };
    }

    template <>
    struct AppTraits<Apps::FlashLight> {
      static constexpr Apps app = Apps::FlashLight;
      static constexpr const char* icon = Screens::Symbols::flashlight;

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::FlashLight(*controllers.systemTask, controllers.brightnessController);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
        return true;
      };
    };
  }
}
