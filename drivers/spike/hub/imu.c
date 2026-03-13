/*
 * SPDX-License-Identifier: MIT
 *
 * API for the hub built-in IMU.
 *
 * Copyright (c) 2022 Embedded and Real-Time Systems Laboratory,
 *            Graduate School of Information Science, Nagoya Univ., JAPAN
 */
#include <t_syslog.h>
#include <spike/hub/imu.h>

#include <pbio/imu.h>
#include <math.h>

static pbio_geometry_matrix_3x3_t rot_matrix = {
  .m11 = 1.0f, .m12 = 0.0f, .m13 = 0.0f,
  .m21 = 0.0f, .m22 = 1.0f, .m23 = 0.0f,
  .m31 = 0.0f, .m32 = 0.0f, .m33 = 1.0f,
};

/*
 * We implement the IMU API by just wrapping functions in external/libpybricks/lib/pbio/src/imu.c.
 */

pbio_error_t hub_imu_init(void) {
  pbio_imu_init();
  pbio_imu_persistent_settings_t settings;
  if (pbio_imu_get_settings(&settings) == PBIO_SUCCESS) {
    pbio_imu_apply_loaded_settings(&settings);
  } else {
    pbio_imu_set_default_settings(&settings);
  }
  return PBIO_SUCCESS;
}

bool hub_imu_is_ready(void) {
  return pbio_imu_is_ready();
}

bool hub_imu_is_stationary(void) {
  return pbio_imu_is_stationary();
}

void hub_imu_set_tilt(float angle) {
  double T = (double) angle * M_PI / 180.0f;
  double sinT = sin(T);
  double cosT = cos(T);
  rot_matrix.m11 = +cosT; rot_matrix.m12 = 0.0f; rot_matrix.m13 = -sinT;
  rot_matrix.m21 = 0.0f ; rot_matrix.m22 = 1.0f; rot_matrix.m23 = 0.0f ;
  rot_matrix.m31 = +sinT; rot_matrix.m32 = 0.0f; rot_matrix.m33 = +cosT;
}

void hub_imu_get_acceleration(float accel[3]) {
  pbio_geometry_xyz_t values;
  pbio_imu_get_acceleration(&values, true);
  pbio_geometry_vector_map(&rot_matrix, &values, (pbio_geometry_xyz_t *) accel);
}

void hub_imu_get_angular_velocity(float angv[3]) {
  pbio_geometry_xyz_t values;
  pbio_imu_get_angular_velocity(&values, true);
  pbio_geometry_vector_map(&rot_matrix, &values, (pbio_geometry_xyz_t *) angv);
}

float hub_imu_get_temperature(void) {
  return 0.0;
}

float hub_imu_get_heading(void) {
  return pbio_imu_get_heading(PBIO_IMU_HEADING_TYPE_3D);
}

void hub_imu_reset_heading(void) {
  pbio_imu_set_heading(0.0f);
}
