#include <m5ui.h>

class UnitAngle8Task : public Task
{
public:
  UnitAngle8 unit;
  UnitAngle8Task() : Task("UnitAngle8Task", 4096, 1, 1)
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
