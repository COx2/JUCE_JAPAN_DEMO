/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   SimpleEqualizer_bg_png;
    const int            SimpleEqualizer_bg_pngSize = 202596;

    extern const char*   SimpleEqualizer_bypass_png;
    const int            SimpleEqualizer_bypass_pngSize = 134007;

    extern const char*   SimpleEqualizer_knob_png;
    const int            SimpleEqualizer_knob_pngSize = 87533;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 3;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
