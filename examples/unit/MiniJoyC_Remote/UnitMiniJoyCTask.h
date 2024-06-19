#include <m5ui.h>

class UnitMiniJoyCTask : public Task
{
public:
  UnitMiniJoyC unit;
  UnitMiniJoyCTask() : Task("UnitJoyCTask", 4096, 1, 1)
  {
  }

  void setup() override
  {
    unit.begin();
  }

  void loop() override
  {
    unit.update();
  }
};
