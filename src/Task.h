#pragma once
#include <M5Unified.h>


/*
const char *const taskName タスクの説明用名前。重複しても動きますがデバッグ用途。最大16文字まで
const uint32_t stackSize スタックサイズ(Byte)
UBaseType_t uxPriority 作成タスクの優先順位(0:低 - 25:高)
BaseType_tconst xCoreID 利用するCPUコア(0-1)
*/
class Task {
public:
  Task(const char* taskName="M5UITask", uint32_t stackSize=4096, UBaseType_t priority=1, BaseType_t coreID=1) {
    xTaskCreatePinnedToCore(taskRunner, taskName, stackSize, this, priority, &taskHandle, coreID);
  }

  virtual void setup() = 0;
  virtual void loop() = 0;

protected:
  void delay(uint32_t ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
  }

  void deleteTask() {
    vTaskDelete(taskHandle);
  }

  void suspend() {
    vTaskSuspend(taskHandle);
  }

  void resume() {
    vTaskResume(taskHandle);
  }

  void changePriority(UBaseType_t newPriority) {
    vTaskPrioritySet(taskHandle, newPriority);
  }

  eTaskState getStatus() {
    return eTaskGetState(taskHandle);
  }

private:
  TaskHandle_t taskHandle = NULL;

  static void taskRunner(void* arg) {
    Task* task = static_cast<Task*>(arg);
    task->setup();
    while (true) {
      task->loop();
      task->delay(1);
    }
  }
};

/* example
class MyTask : public Task {
public:
  MyTask() : Task("MyTask", 4096, 1, 1) {}

  void setup() override {
    Serial.println("Task setup");
  }

  void loop() override {
    Serial.println("Task loop");
    delay(1000);
  }
};

void setup() {
  M5.begin();
  new MyTask();
}

void loop() {
  // メインループのコード
}
*/