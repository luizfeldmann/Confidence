/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CTEXTSTYLER_H_
#define _CTEXTSTYLER_H_

#include <wx/stc/stc.h>
#include <JInterface/JsonSerialization.h>

//! @brief Serializable representation of colour
//         Convertible to and from wxColour  
struct SColor
{
    uint8_t r;  //!< The red color component
    uint8_t g;  //!< The green color component
    uint8_t b;  //!< The blue color component

    //! @brief Initializes a black color
    SColor();

    //! @brief Initializes the color from the given components
    //! @param[in] uR @copybrief r
    //! @param[in] uG @copybrief g
    //! @param[in] uB @copybrief b
    SColor(uint8_t uR, uint8_t uG, uint8_t uB);

    //! @brief Constructs from a given wxColour
    SColor(const wxColour&);

    //! @brief Converts to a wxColour
    operator wxColour() const;
};

DECLARE_SERIALIZATION_SCHEME(SColor);

//! @brief An identifyable text style associated to a lexical state
struct STextStyle
{
    int m_iID;               //!< ID of lexical state associated to this style
    SColor m_cForeground;    //!< Text foreground colour

    //! @brief Initializes an empty style
    STextStyle();

    //! @brief Initializes a style from the given parameters
    //! @param[in] iID @copybrief m_iID
    //! @param[in] sForeground @copybrief m_cForeground
    STextStyle(int iID, const SColor& sForeground);
};

DECLARE_SERIALIZATION_SCHEME(STextStyle);

//! @brief A collection of styles associated to a specific Lexer
struct STextStylesheet
{
    int m_iID;                          //!< ID of the lexer implementation
    std::string m_strExt;               //!< Default file extension for this style
    std::vector<STextStyle> m_vLex;     //!< Collection of lexical states

    //! @brief Constructs an empty stylesheet
    STextStylesheet();

    //! @brief Constructs a stylesheet with the provided parameters
    //! @param[in] iID @copybrief m_iID
    //! @param[in] strExt @copybrief m_strExt
    //! @param[in] ilStyles @copybrief m_vLex
    STextStylesheet(int iID, const std::string& strExt, std::initializer_list<STextStyle> ilStyles);
};

DECLARE_SERIALIZATION_SCHEME(STextStylesheet);

//! @brief Manages and styles a wxStyledTextCtrl
class CTextStyler : public wxEvtHandler
{
protected:
    //! The underlying styled text control being managed
    wxStyledTextCtrl& m_rSTC;

    //! @brief Called by the STC whenever a text range needs to be styled
    void OnStyleNeeded(wxStyledTextEvent& event);

    //! Type used as a collection of #STextStylesheet
    using vec_styles_t = std::vector<STextStylesheet>;

    //! @brief Default styles collection
    static const vec_styles_t& GetDefaultStyles();

    //! @brief Retrieves a collection of all supported styles
    static const vec_styles_t& GetStyles();

public:
    //! @brief Constructs a new styler object
    //! param[in] rSTC @copybrief m_rSTC
    CTextStyler(wxStyledTextCtrl& rSTC);
    ~CTextStyler();

    //! Sets the lexer and associated styles
    void SetLexer(int iID);
};

#endif