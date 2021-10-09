#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include "Input_Output.hpp"

extern std::string CalibrationImages_Folder;

bool Camera_Calibration(IO_TYPE IO_type, std::string = CalibrationImages_Folder, uint8_t corner_counts_x = 11, uint8_t corner_counts_y = 12);

#endif // CALIBRATION_HPP
