#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double *new_array = new double[new_size];
  for (int i = 0; i < length; ++i) {
      new_array[i] = array[i];
  }
  for (int i = length; i < new_size; ++i) {
      new_array[i] = 0;
  }

  delete[] array;
  return new_array; // YOU CAN CHANGE THIS
}

double* shrink_array(double* array, int length, int new_size) {
    double *new_array = new double[new_size];
    for (int i = 0; i < new_size; ++i) {
        new_array[i] = array[i];
    }

    delete[] array;
    return new_array; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  if (current_size == max_size) {
    array = extend_array(array, current_size, max_size + 5);
    max_size += 5;
  }
  array[current_size] = element;
  current_size += 1;
  return array;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  current_size -= 1;
  if (current_size == max_size - 5) {
      array = shrink_array(array, max_size, current_size);
      max_size -= 5;
  }
  return array; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
  int *ids = new int[tot_telemetries];
  for (int i = 0; i < tot_telemetries; ++i) ids[i] = 0;
  while (true) {
    double t = 1e50;
    int tel = -1;
    for (int i = 0; i < tot_telemetries; ++i) {
        if (ids[i] < telemetries_sizes[i] && telemetries[i][ids[i]] < t) {
            tel = i;
            t = telemetries[i][ids[i]];
        }
    }
    if (tel == -1) break;
    global_telemetry = append_to_array(telemetries[tel][ids[tel]++], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
    global_telemetry = append_to_array(telemetries[tel][ids[tel]++], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
    global_telemetry = append_to_array(telemetries[tel][ids[tel]++], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
  }
  delete[] ids;
}
