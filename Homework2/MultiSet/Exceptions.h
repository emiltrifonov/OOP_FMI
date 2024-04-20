#pragma once

namespace Exceptions {
	constexpr char INVALID_MAX_NUM_EXCEPTION[] = "Error! Multiset must have a positive maximum value!";
	constexpr char INVALID_BIT_STORAGE_EXCEPTION[] = "Error! Multiset must use between 1 and 8 bits for storing infortmation!";
	constexpr char INVALID_NEW_NUMBER_EXCEPTION[] = "Error! Cannot add number outside the allowed range!";
	constexpr char INVALID_FILE_NAME_EXCEPTION[] = "Error! Cannot read file name!";
	constexpr char CLOSED_FILE_EXCEPTION[] = "Error! Cannot open file!";
}
