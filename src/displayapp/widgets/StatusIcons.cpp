#include "displayapp/widgets/StatusIcons.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/InfiniTimeTheme.h"

using namespace Pinetime::Applications::Widgets;

StatusIcons::StatusIcons(const Controllers::Battery& batteryController,
                         const Controllers::Ble& bleController,
                         const Controllers::NotificationManager& notificationManager)
  : batteryIcon(true), batteryController {batteryController}, bleController {bleController}, notificationManager {notificationManager} {
}

void StatusIcons::Create() {
  container = lv_cont_create(lv_scr_act(), nullptr);
  lv_cont_set_layout(container, LV_LAYOUT_ROW_TOP);
  lv_cont_set_fit(container, LV_FIT_TIGHT);
  lv_obj_set_style_local_pad_inner(container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
  lv_obj_set_style_local_bg_opa(container, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

  notificationIcon = lv_label_create(container, nullptr);
  lv_label_set_text_static(notificationIcon, "*");
  lv_obj_set_style_local_text_color(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, Colors::highlight);

  bleIcon = lv_label_create(container, nullptr);
  lv_label_set_text_static(bleIcon, Screens::Symbols::bluetooth);

  batteryIcon.Create(container);

  lv_obj_align(container, nullptr, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
}

void StatusIcons::Update() {
  batteryPercentRemaining = batteryController.PercentRemaining();
  powerPresent = batteryController.IsPowerPresent();
  if (batteryPercentRemaining.IsUpdated() || powerPresent.IsUpdated()) {
    auto batteryPercent = batteryPercentRemaining.Get();
    batteryIcon.SetBatteryPercentage(batteryPercent);
    batteryIcon.SetBatteryCharging(powerPresent.Get());
  }

  notificationState = notificationManager.AreNewNotificationsAvailable();
  if (notificationState.IsUpdated()) {
    lv_obj_set_hidden(notificationIcon, !notificationState.Get());
  }

  bleState = bleController.IsConnected();
  bleRadioEnabled = bleController.IsRadioEnabled();
  if (bleState.IsUpdated() || bleRadioEnabled.IsUpdated()) {
    lv_obj_set_hidden(bleIcon, !bleRadioEnabled.Get());
    lv_obj_set_style_local_text_color(bleIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, bleState.Get() ? Colors::fg : Colors::highlight);
  }

  lv_obj_realign(container);
}
