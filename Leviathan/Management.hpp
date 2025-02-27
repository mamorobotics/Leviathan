/**
 * @file Management.hpp
 * @brief This file contains all error codes and what they will return in console
 */

#include <string>

#ifndef FILE_H
#define FILE_H

/**
 * @enum LEV_CODE
 * @brief Enumeration representing various error and status codes.
 */
enum LEV_CODE { 
    CLEAR,         /**< No error, clear state. */
    CONN_ERROR,    /**< Connection error occurred. */
    TEST,          /**< Test-related error. */
    IMAGE_ERROR,   /**< Image processing error. */
    GENERAL_ERROR, /**< General error condition. */
    WARNING        /**< Warning condition. */
};

/**
 * @brief Returns a string representation of a given LEV_CODE value.
 * @param code The error/status code to convert.
 * @return std::string A string describing the code.
 */
static std::string CodeDef(LEV_CODE code) {
    if (code == LEV_CODE::CLEAR)
        return "Clear";
    if (code == LEV_CODE::CONN_ERROR)
        return "Connection Error";
    if (code == LEV_CODE::TEST)
        return "Test Error";
    if (code == LEV_CODE::IMAGE_ERROR)
        return "Image Error";
    if (code == LEV_CODE::GENERAL_ERROR)
        return "Error";
    if (code == LEV_CODE::WARNING)
        return "Warning";

    return "NULL CODE";
}

#endif