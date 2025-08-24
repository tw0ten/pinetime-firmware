#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/ble/BleController.h"
#include "displayapp/widgets/StatusIcons.h"
#include "utility/DirtyValue.h"
#include "displayapp/apps/Apps.h"

namespace Pinetime {
  namespace Controllers {
    class Battery;
    class Ble;
    class NotificationManager;
  }

  namespace Applications {
    namespace Screens {

      class WatchFaceCustom : public Screen {
      public:
        WatchFaceCustom(Controllers::DateTime& dateTimeController,
                        const Controllers::Battery& batteryController,
                        const Controllers::Ble& bleController,
                        const Controllers::NotificationManager& notificationManager);
        ~WatchFaceCustom() override;

        void Refresh() override;

      private:
        uint8_t displayedHour = -1;
        uint8_t displayedMinute = -1;

        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>> currentDateTime {};

        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::days>> currentDate;

        lv_obj_t* label_time;
        lv_obj_t* label_date;

        Controllers::DateTime& dateTimeController;

        lv_task_t* taskRefresh;
        Widgets::StatusIcons statusIcons;
      };
    }

    template <>
    struct WatchFaceTraits<WatchFace::Custom> {
      static constexpr WatchFace watchFace = WatchFace::Custom;
      static constexpr const char* name = "custom";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::WatchFaceCustom(controllers.dateTimeController,
                                            controllers.batteryController,
                                            controllers.bleController,
                                            controllers.notificationManager);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
        return true;
      }
    };
  }
}
