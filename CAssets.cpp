#include "CAssets.h"

void CAssets::Load() {
    /*todo add font file*/
    if(!m_Font.loadFromFile(RESOURCE_DIR_PATH + "Fonts/Ustroke\ Regular.ttf")) throw;
}