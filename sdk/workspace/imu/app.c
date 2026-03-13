#include "kernel_cfg.h"
#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>

#include <spike/hub/imu.h>
#include <pbio/imu.h>
#include <math.h>

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused) {

  dly_tsk(5*1000*1000);

  FILE *fp = serial_open_newlib_file(SIO_USB_PORTID);
  fprintf(fp, "Start !!\n");
    
  hub_imu_init();

  // HackSPi's hub is tilted at ~51 degrees.
  hub_imu_set_tilt(51.0f);

  // Show angular velocity, acceleration, heading
  while (1) {
    float w[3], a[3];
    bool stationary = hub_imu_is_stationary();
    fprintf(fp, "[%c] ", stationary ? 'T' : 'F');
    hub_imu_get_acceleration(a);
    fprintf(fp, "a = (%.0f %.0f %.0f); ", a[0], a[1], a[2]);
    hub_imu_get_angular_velocity(w);
    fprintf(fp, "ω = (%.0f %.0f %.0f); ", w[0], w[1], w[2]);
    fprintf(fp, "h = %.0f\n", hub_imu_get_heading());
    dly_tsk(1000*1000);
  }
  
  /* タスク終了 */
  ext_tsk();
}
