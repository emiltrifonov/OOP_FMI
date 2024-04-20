#pragma once

namespace Exceptions {
	constexpr char INVALID_FILENAME_EXCEPTION[] = "Invalid file name!";
	constexpr char CLOSED_FILE_EXCEPTION[] = "Cannot open file!";
	constexpr char OUT_OF_RANGE_EXCEPTION[] = "Cannot process signed values bigger than 16 bits!";
	constexpr char UNDEFINED_FUNCTION_EXCEPTION[] = "Function undefined for given argument!";
	constexpr char IRREVIRSIBLE_FUNCTION_EXCEPTION[] = "Cannot generate reverse function for a non-bijective function!";
	constexpr char BIG_STARTING_COORDINATES_EXCEPTION[] = "X and Y cannot be more than 32748!";
	constexpr char INVALID_POWER_EXCEPTION[] = "Power must be a positive number!";
}
