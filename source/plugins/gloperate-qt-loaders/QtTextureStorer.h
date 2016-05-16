
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/base/Storer.h>


namespace globjects
{
    class Texture;
}


/**
*  @brief
*    Texture storer based on Qt
*
*  Supported options:
*    none
*/
class QtTextureStorer : public gloperate::Storer<globjects::Texture> 
{
CPPEXPOSE_DECLARE_COMPONENT(QtTextureStorer, gloperate::AbstractStorer)


public:
    /**
    *  @brief
    *    Constructor
    */
    QtTextureStorer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~QtTextureStorer();

    // Virtual gloperate::AbstractStorer functions
    virtual bool canStore(const std::string & ext) const;
    virtual std::vector<std::string> storingTypes() const;
    virtual std::string allStoringTypes() const;

    // Virtual gloperate::Storer<globjects::Texture> functions
    virtual bool store(const std::string & filename, const globjects::Texture * texture, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
    std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */
};
