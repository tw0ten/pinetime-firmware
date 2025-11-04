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

    lv_label_set_text_fmt(label_time, " %02i/%02i ", hour, minute);
    lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

    uint8_t alarm_minute = alarmController.Minutes();
    uint8_t left_hours = ((minute > alarm_minute ? 23 : 24) + alarmController.Hours() - hour) % 24;
    lv_label_set_text_fmt(label_sleeptime, "%i:%02i", left_hours, (60 + alarm_minute - minute) % 60);
    lv_obj_realign(label_sleeptime);
    lv_obj_set_style_local_text_color(label_sleeptime, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, left_hours <= 8 ? Colors::highlight : Colors::fg);

    currentDate = std::chrono::time_point_cast<std::chrono::days>(currentDateTime.Get());
    if (currentDate.IsUpdated()) {
      lv_label_set_text_fmt(label_date,
                            "%i/%02i/%02i-%i",
                            dateTimeController.Year(),
                            dateTimeController.Month(),
                            dateTimeController.Day(),
                            dateTimeController.DayOfWeek());
      lv_obj_realign(label_date);
    }
  }
}
