#include <regex>
#include "ui/CTextStyler.h"
#include "util/Log.h"

/* SColor */
DEFINE_SERIALIZATION_SCHEME(SColor,
    SERIALIZATION_MEMBER(r)
    SERIALIZATION_MEMBER(g)
    SERIALIZATION_MEMBER(b)
)

SColor::SColor()
    : r(0)
    , g(0)
    , b(0)
{

}

SColor::SColor(uint8_t uR, uint8_t uG, uint8_t uB)
    : r(uR)
    , g(uG)
    , b(uB)
{

}

SColor::SColor(const wxColour& wxc)
    : r(wxc.Red())
    , g(wxc.Green())
    , b(wxc.Blue())
{

}

SColor::operator wxColour() const
{
    return wxColour(r, g, b);
}

/* STextStyle */
DEFINE_SERIALIZATION_SCHEME(STextStyle,
    SERIALIZATION_MEMBER(m_iID)
    SERIALIZATION_MEMBER(m_cForeground)
)

STextStyle::STextStyle()
    : m_iID(0)
    , m_cForeground()
{

}

STextStyle::STextStyle(int iID, const SColor& sForeground)
    : m_iID(iID)
    , m_cForeground(sForeground)
{

}

/* STextStylesheet */
DEFINE_SERIALIZATION_SCHEME(STextStylesheet,
    SERIALIZATION_MEMBER(m_iID)
    SERIALIZATION_MEMBER(m_strExt)
    SERIALIZATION_MEMBER(m_vLex)
)

STextStylesheet::STextStylesheet()
    : m_iID(0)
    , m_strExt("")
{
}

STextStylesheet::STextStylesheet(int iID, const std::string& strExt, std::initializer_list<STextStyle> ilStyles)
    : m_iID(iID)
    , m_strExt(strExt)
    , m_vLex(ilStyles)
{

}

/* CTextStyler */
CTextStyler::CTextStyler(wxStyledTextCtrl& rSTC)
    : m_rSTC(rSTC)
{
    m_rSTC.Bind(wxEVT_STC_STYLENEEDED, &CTextStyler::OnStyleNeeded, this);
}

CTextStyler::~CTextStyler()
{
    m_rSTC.Unbind(wxEVT_STC_STYLENEEDED, &CTextStyler::OnStyleNeeded, this);
}

void CTextStyler::SetLexer(int iID)
{
    m_rSTC.StyleClearAll();
    m_rSTC.SetLexer(iID);

    const vec_styles_t& rvStyles = GetStyles();
    vec_styles_t::const_iterator itFind = std::find_if(rvStyles.cbegin(), rvStyles.cend(),
        [iID](const STextStylesheet& rSearch)-> bool 
        {
            return (rSearch.m_iID == iID);
        }
    );

    if (itFind == rvStyles.cend())
        CERROR("Requested unregistered lexer ID %d", iID);
    else
    {
        for (const STextStyle& rLex : itFind->m_vLex)
            m_rSTC.StyleSetForeground(rLex.m_iID, rLex.m_cForeground);
    }
}

enum class EContainerStyle : int
{
    ERegular = 0,       //!< Regular text / not variables
    EVariableOK,        //!< A variable that exists in the project
    EVariableNOK,       //!< A variable that does not exist in the project
};

void CTextStyler::OnStyleNeeded(wxStyledTextEvent& event)
{
    // Regex used to highlight variables
    const static std::regex c_varRegex("\\$\\(([\\w|\\d]+)\\)");

    // Get range of text requiring the styling
    int iEndPos = m_rSTC.GetCurrentPos();
    const int iEndLine = m_rSTC.LineFromPosition(iEndPos);
    iEndPos = m_rSTC.GetLineEndPosition(iEndLine); // the end of the last line

    int iStartPos = m_rSTC.GetEndStyled();
    const int iStartLine = m_rSTC.LineFromPosition(iStartPos);
    iStartPos = m_rSTC.PositionFromLine(iStartLine); // the start of the first line

    if (iStartPos == iEndPos)
        return;

    const std::string strText = m_rSTC.GetTextRange(iStartPos, iEndPos).ToStdString();

    // Regular style the whole range
    m_rSTC.StartStyling(iStartPos);
    m_rSTC.SetStyling(iEndPos - iStartPos, (int)EContainerStyle::ERegular);

    // Iterate over matching variables
    auto itBegin = std::sregex_iterator(strText.begin(), strText.end(), c_varRegex);
    auto itEnd   = std::sregex_iterator();

    for (std::sregex_iterator i = itBegin; i != itEnd; ++i)
    {
        std::smatch match = *i;

        EContainerStyle eStyle = EContainerStyle::EVariableOK; // TODO: check variable exists

        const int iMatchStart = iStartPos + match.position();
        const int iMatchLength = match.length();

        m_rSTC.StartStyling(iMatchStart);
        m_rSTC.SetStyling(iMatchLength, (int)eStyle);
    }
}

const CTextStyler::vec_styles_t& CTextStyler::GetDefaultStyles()
{
    static const CTextStyler::vec_styles_t vDefaultStyles {
        STextStylesheet(wxSTC_LEX_CONTAINER, ".*", {
                STextStyle((int)EContainerStyle::ERegular,      SColor(  0,   0,   0)),
                STextStyle((int)EContainerStyle::EVariableOK,   SColor(  0, 136,   0)),
                STextStyle((int)EContainerStyle::EVariableNOK,  SColor(163,   0,   0)),
            })
    };

    return vDefaultStyles;
}

const CTextStyler::vec_styles_t& CTextStyler::GetStyles()
{
    // TODO: read from configuration; on failure fallback to default
    return GetDefaultStyles();
}