#pragma once

class UnitDualButton
{
public:
    UnitDualButton()
    {
        begin();
    }
    void begin()
    {
        pinMode(26, INPUT_PULLUP); // Red Button Pin setting
        pinMode(36, INPUT_PULLUP); // Blue Button Pin setting
    }

    bool isRedButtonPressed()
    {
        if (digitalRead(26) == 0)
            return true;
        return false;
    }
    bool isBlueButtonPressed()
    {
        if (digitalRead(36) == 0)
            return true;
        return false;
    }

    bool wasRedButtonPressed()
    {
        static bool wasPressed = false;
        static bool isPressed = false;
        isPressed = isRedButtonPressed();
        if (isPressed && !wasPressed)
        {
            wasPressed = true;
            return true;
        }
        else if (!isPressed)
        {
            wasPressed = false;
        }
        return false;
    }
    bool wasBlueButtonPressed()
    {
        static bool wasPressed = false;
        static bool isPressed = false;
        isPressed = isBlueButtonPressed();
        if (isPressed && !wasPressed)
        {
            wasPressed = true;
            return true;
        }
        else if (!isPressed)
        {
            wasPressed = false;
        }
        return false;
    }
};
