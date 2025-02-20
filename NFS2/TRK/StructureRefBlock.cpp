#include "StructureRefBlock.h"

#include "Common/Logging.h"

using namespace LibOpenNFS::NFS2;

StructureRefBlock::StructureRefBlock(std::ifstream &trk) {
    ASSERT(this->StructureRefBlock::_SerializeIn(trk), "Failed to serialize StructureRefBlock from file stream");
}

bool StructureRefBlock::_SerializeIn(std::ifstream &ifstream) {
    std::streamoff padCheck = ifstream.tellg();

    onfs_check(safe_read(ifstream, recSize));
    onfs_check(safe_read(ifstream, recType));
    onfs_check(safe_read(ifstream, structureRef));

    if (recType == 1) {
        // Fixed type
        onfs_check(safe_read(ifstream, refCoordinates));
    } else if (recType == 3) {
        // Animated type
        onfs_check(safe_read(ifstream, animLength));
        onfs_check(safe_read(ifstream, unknown));
        animationData.resize(animLength);
        onfs_check(safe_read(ifstream, animationData));
    } else if (recType == 4) {
        // 4 Component PSX Vert data? TODO: Restructure to allow the 4th component to be read
        onfs_check(safe_read(ifstream, refCoordinates));
    } else {
        LogDebug("Unknown Structure Reference type: %d Size: %d StructRef: %d", (int)recType, (int)recSize,
                 (int)structureRef);
        return true;
    }

    ifstream.seekg(recSize - (ifstream.tellg() - padCheck), std::ios_base::cur); // Eat possible padding

    return true;
}

void StructureRefBlock::_SerializeOut(std::ofstream &ofstream) {
    ASSERT(false, "StructureRefBlock output serialization is not currently implemented");
}
