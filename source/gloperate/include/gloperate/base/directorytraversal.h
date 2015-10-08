
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


// [TODO] Will be removed and replaced by functionality in iozeug

/**
*  @brief
*    List files in directory
*
*  @param[in] directory
*    Path to directory
*  @param[in] alias
*    The alias for the directory, used to register the named string
*  @param[in] fileExtension
*    File extension ("*" for all files)
*  @param[in] recursive
*    Search recursively in sub directories?
*
*  @remarks
*    For each found file, an instance of globjects::File is created and registered as NamedString.
*    The path for the named string is either the full file path or the prefix replaced with the alias, if provided.
*    Use globjects::FileRegistry to access all files.
*
*  @see globjects::File
*  @see globjects::FileRegistry
*/
GLOPERATE_API void scanDirectory(const std::string & directory, const std::string & fileExtension, bool recursive = false);
GLOPERATE_API void scanDirectory(const std::string & directory, const std::string & alias, const std::string & fileExtension, bool recursive = false);

/**
*  @brief
*    Makes sure given path does not end with (platform specific) separator
*
*  @param[in] path
*    Path to directory or file
*
*  @return
*    Path to directory or file with ensured missing separator ending
*/
GLOPERATE_API std::string removeTrailingPathSeparator(const std::string & path);

/**
*  @brief
*    Makes sure given path ends with (platform specific) separator
*
*  @param[in] path
*    Path to directory or file
*
*  @return
*    Path to directory or file with ensured separator ending
*/
GLOPERATE_API std::string ensurePathSeparatorEnding(const std::string & path);


} // namespace gloperate
