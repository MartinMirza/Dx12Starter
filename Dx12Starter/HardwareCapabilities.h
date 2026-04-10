#pragma once

class HardwareCapabilities
{
public:
    enum Capability
    {
        ALLOW_TEARING,
        // Future capabilities can be added here
        COUNT
    };

    void SetCapability(Capability capability) 
    {
        if (capability >= 0 && capability < COUNT)
        {
            capabilities[capability] = true;
        }
    }

    bool GetCapability(Capability capability) const
    {
        if (capability >= 0 && capability < COUNT)
        {
            return capabilities[capability];
        }
        return false;
    }

private:
    bool capabilities[COUNT] = { false };
};