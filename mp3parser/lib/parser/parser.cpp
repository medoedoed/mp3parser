#include "parser.h"


std::vector<uint8_t> ReadBytes(std::ifstream& file, const size_t n) {
    std::vector<uint8_t> bytes (n);
    for (size_t i = 0; i < n; i++) {
        file >> bytes[i];
    }

    return bytes;
}

void SkipBytes(std::ifstream& file, uint32_t n) {
    file.seekg(n, std::ios_base::cur);
}


auto CreateFrame(std::vector<uint8_t>& buf) {
    FrameHeader h;

    h.id = std::string(buf.begin(), buf.begin() + 4);
    h.size = (buf[4] << 21) | (buf[5] << 14) | (buf[6] << 7) | buf[7];
    h.flags = (buf[8] << 8) | buf[9];
    Frame* frame;

    if (h.id == "ETCO") {
        frame = new ETCOframe(h);
    } else if (h.id[0] == 'T' && (h.id != "TFLT" || h.id != "TMED")) {
        frame = new TEXTframe(h);
    } else if (h.id[0] == 'W' && h.id != "WXXX") {
        frame = new URLframe(h);
    } else if (h.id == "WXXX") {
        frame = new WXXXframe(h);
    }  else if (h.id == "USLT") {
        frame = new USLTframe(h);
    } else if (h.id == "SYLT") {
        frame = new SYLTframe(h);
    } else if (h.id == "COMM") {
        frame = new COMMframe(h);
    } else if (h.id == "RVA2") {
        frame = new RVA2frame(h);
    } else if (h.id == "EQU2") {
        frame = new EQU2frame(h);
    } else if (h.id == "RVRB") {
        frame = new RVRBframe(h);
    } else if (h.id == "PCNT") {
        frame = new PCNTframe(h);
    } else if (h.id == "POPM") {
        frame = new POPMframe(h);
    } else if (h.id == "RBUF") {
        frame = new RBUFframe(h);
    }else {
        frame = new Frame(h);
        frame->Bad();
    }

    return frame;
}

void fun(std::string& path) {
    std::ifstream file(path, std::ios_base::binary);
    std::noskipws(file);

    std::vector<uint8_t> buf = ReadBytes(file, 10);

    Header header;

    for (size_t i = 0; i < 3; i++) {
        header.identifier += (char) buf[i];
    }

    header.version = buf[3];

    if (header.version != 4) {
        std::cerr << "wrong ID3v2 version";
        exit(1);
    }
    header.revision = buf[4];
    header.flags = buf[5];
    header.size = (buf[6] << 21) | (buf[7] << 14) | (buf[8] << 7) | buf[9] - 10;

    if (header.flags & 64) {
        buf = ReadBytes(file, 4);
        uint32_t ext_header_size = (buf[0] << 21) | (buf[1] << 14) | (buf[2] << 7) | buf[3];
        SkipBytes(file, ext_header_size - 4);
        header.size -= ext_header_size;
    }

    if (header.flags & 16) {
        header.size -= 10;
    }

    while (file.tellg() < header.size) {
        buf = ReadBytes(file, 10);
        if (buf[0] == 0) {
            break;
        }

        auto frame = CreateFrame(buf);

        if (frame->Good()) {
            frame->SetData(ReadBytes(file, frame->size()));
            frame->Print();
        } else {
            SkipBytes(file, frame->size());
        }
    }
}


