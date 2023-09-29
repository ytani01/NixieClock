/**
 * Copyright (c) 2023 Yoichi Tanibayashi
 */
#include "Task_NixieTubeArray.h"

/** constructor
 *
 */
Task_NixieTubeArray::Task_NixieTubeArray(NixieTubeArray *nta)
  : Task(String(Task_NixieTubeArray::name)) {

  this->_nta = nta;
} // Task_NixieTubeArray::Task_NixieTubeArray()

/**
 * 
 */
void Task_NixieTubeArray::setup() {
  log_i("%s", this->conf.name);
} // Task_NixieTubeArray::setup()

/**
 *
 */
void Task_NixieTubeArray::loop() {
  log_i("%s", this->conf.name);
  delay(5000);
} // Task_NixieTubeArray::loop()
