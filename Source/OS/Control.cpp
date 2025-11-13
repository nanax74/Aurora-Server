#include <wups.h>
#include <vpad/input.h>

#include "OS/Control.hpp"

DECL_FUNCTION(int32_t, VPADRead, VPADChan chan, VPADStatus* buffers, uint32_t count, VPADReadError* error)
{
    int32_t result = real_VPADRead(chan, buffers, count, error);

    if (result > 0 && *error == VPAD_READ_SUCCESS)
    {
        OS::Control::tick(buffers[0]);
    }

    return result;
}

WUPS_MUST_REPLACE_FOR_PROCESS(VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_ALL);