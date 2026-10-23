// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice
#include "dt_localization/msg/detail/dt35_distances__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
dt_localization__msg__Dt35Distances__init(dt_localization__msg__Dt35Distances * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    dt_localization__msg__Dt35Distances__fini(msg);
    return false;
  }
  // distance
  // valid
  // frame_count
  // crc_error_count
  return true;
}

void
dt_localization__msg__Dt35Distances__fini(dt_localization__msg__Dt35Distances * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // distance
  // valid
  // frame_count
  // crc_error_count
}

bool
dt_localization__msg__Dt35Distances__are_equal(const dt_localization__msg__Dt35Distances * lhs, const dt_localization__msg__Dt35Distances * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // distance
  for (size_t i = 0; i < 4; ++i) {
    if (lhs->distance[i] != rhs->distance[i]) {
      return false;
    }
  }
  // valid
  for (size_t i = 0; i < 4; ++i) {
    if (lhs->valid[i] != rhs->valid[i]) {
      return false;
    }
  }
  // frame_count
  if (lhs->frame_count != rhs->frame_count) {
    return false;
  }
  // crc_error_count
  if (lhs->crc_error_count != rhs->crc_error_count) {
    return false;
  }
  return true;
}

bool
dt_localization__msg__Dt35Distances__copy(
  const dt_localization__msg__Dt35Distances * input,
  dt_localization__msg__Dt35Distances * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // distance
  for (size_t i = 0; i < 4; ++i) {
    output->distance[i] = input->distance[i];
  }
  // valid
  for (size_t i = 0; i < 4; ++i) {
    output->valid[i] = input->valid[i];
  }
  // frame_count
  output->frame_count = input->frame_count;
  // crc_error_count
  output->crc_error_count = input->crc_error_count;
  return true;
}

dt_localization__msg__Dt35Distances *
dt_localization__msg__Dt35Distances__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dt_localization__msg__Dt35Distances * msg = (dt_localization__msg__Dt35Distances *)allocator.allocate(sizeof(dt_localization__msg__Dt35Distances), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(dt_localization__msg__Dt35Distances));
  bool success = dt_localization__msg__Dt35Distances__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
dt_localization__msg__Dt35Distances__destroy(dt_localization__msg__Dt35Distances * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    dt_localization__msg__Dt35Distances__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
dt_localization__msg__Dt35Distances__Sequence__init(dt_localization__msg__Dt35Distances__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dt_localization__msg__Dt35Distances * data = NULL;

  if (size) {
    data = (dt_localization__msg__Dt35Distances *)allocator.zero_allocate(size, sizeof(dt_localization__msg__Dt35Distances), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = dt_localization__msg__Dt35Distances__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        dt_localization__msg__Dt35Distances__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
dt_localization__msg__Dt35Distances__Sequence__fini(dt_localization__msg__Dt35Distances__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      dt_localization__msg__Dt35Distances__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

dt_localization__msg__Dt35Distances__Sequence *
dt_localization__msg__Dt35Distances__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  dt_localization__msg__Dt35Distances__Sequence * array = (dt_localization__msg__Dt35Distances__Sequence *)allocator.allocate(sizeof(dt_localization__msg__Dt35Distances__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = dt_localization__msg__Dt35Distances__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
dt_localization__msg__Dt35Distances__Sequence__destroy(dt_localization__msg__Dt35Distances__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    dt_localization__msg__Dt35Distances__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
dt_localization__msg__Dt35Distances__Sequence__are_equal(const dt_localization__msg__Dt35Distances__Sequence * lhs, const dt_localization__msg__Dt35Distances__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!dt_localization__msg__Dt35Distances__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
dt_localization__msg__Dt35Distances__Sequence__copy(
  const dt_localization__msg__Dt35Distances__Sequence * input,
  dt_localization__msg__Dt35Distances__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(dt_localization__msg__Dt35Distances);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    dt_localization__msg__Dt35Distances * data =
      (dt_localization__msg__Dt35Distances *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!dt_localization__msg__Dt35Distances__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          dt_localization__msg__Dt35Distances__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!dt_localization__msg__Dt35Distances__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
