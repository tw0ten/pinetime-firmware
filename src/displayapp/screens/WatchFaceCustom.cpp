#include "displayapp/screens/WatchFaceCustom.h"

#include <lvgl/lvgl.h>
#include <cstdio>

#include "displayapp/screens/NotificationIcon.h"
#include "displayapp/screens/Symbols.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Screens;

WatchFaceCustom::WatchFaceCustom(Controllers::DateTime& dateTimeController,
                                 const Controllers::Battery& batteryController,
                                 const Controllers::Ble& bleController,
                                 const Controllers::AlarmController& alarmController,
                                 const Controllers::NotificationManager& notificationManager)
  : currentDateTime {{}},
    dateTimeController {dateTimeController},
    alarmController {alarmController},
    statusIcons(batteryController, bleController, notificationManager) {

  statusIcons.Create();

  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);

  label_sleeptime = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_align(label_sleeptime, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

  label_time = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
  Refresh();
}

WatchFaceCustom::~WatchFaceCustom() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void WatchFaceCustom::Refresh() {
  statusIcons.Update();

  currentDateTime = std::chrono::time_point_cast<std::chrono::minutes>(dateTimeController.CurrentDateTime());

  if (currentDateTime.IsUpdated()) {
    uint8_t hour = dateTimeController.Hours();
    uint8_t minute = dateTimeController.Minutes();

    lv_label_set_text_fmt(label_time, " %02d %02d ", hour, minute);
    lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    uint8_t alarm_hour = alarmController.Hours();
    uint8_t alarm_minute = alarmController.Minutes();
    lv_label_set_text_fmt(label_sleeptime, "%d:%02d", (24 + alarm_hour - hour) % 24, (60 + alarm_minute - minute) % 60);
    lv_obj_realign(label_sleeptime);

    currentDate = std::chrono::time_point_cast<std::chrono::days>(currentDateTime.Get());
    if (currentDate.IsUpdated()) {
      lv_label_set_text_fmt(label_date,
                            "%d/%02d/%02d %d",
                            dateTimeController.Year(),
                            dateTimeController.Month(),
                            dateTimeController.Day(),
                            dateTimeController.DayOfWeek());
      lv_obj_realign(label_date);
    }
  }
}
