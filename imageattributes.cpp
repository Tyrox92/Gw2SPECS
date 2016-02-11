#include "imageattributes.h"

using namespace GW2;

const int ImageAttributes::s_LastLineControlYOffsets[] = {
    18,21,0,
    17,20,0,
    0,17,0,
    0,21,0
};

const int ImageAttributes::s_LineDistances[] = {
    15,16,0,
    15,16,0,
    0,0,0,
    0,0,20
};

const int ImageAttributes::s_CharacterWidths[] = {
    7,7,0,
    7,7,0,
    0,0,0,
    0,0,0
};

const int ImageAttributes::s_CharacterHeights[] = {
    9,10,0,
    9,10,0,
    0,0,0,
    0,0,0
};


ImageAttributes::ImageAttributes() :
    m_InterfaceSize(NormalInterfaceSize),
    m_ChatTextSize(MediumTextSize),
    m_InterfaceChatTextSizeIndex(m_InterfaceSize * ChatTextSizeCount + m_ChatTextSize),
    m_LastLineY(-1)
{

}

ImageAttributes::~ImageAttributes()
{

}
