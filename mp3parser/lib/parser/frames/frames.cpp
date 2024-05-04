#include "frames.h"


void Frame::GetEncoding() {
    std::cout << "Text encoding:\t\t\t";
    if (data_[0] == 0) {
        std::cout << "ISO-8859-1\n";
    } else if (data_[0] == 1) {
        std::cout << "UTF-16\n";
    } else if (data_[0] == 2) {
        std::cout << "UTF-16BE\n";
    } else {
        std::cout << "UTF-8\n";
    }
}

bool Frame::Good() {
    return good_;
}

void Frame::Bad() {
    good_ = false;
}

void Frame::Print() {
    std::cout << '\n' << "___________________________________________________" << '\n';
    std::cout << "Frame ID:\t\t\t" << identifier_ << '\n';
}

void TEXTframe::Print() {
    Frame::Print();
    GetEncoding();

    size_t index = 1;

    if (identifier_ == "TXXX") {
        std::cout << "Description:\t\t\t";
        while (data_[index] != 0) {
            std::cout << data_[index++];
        }

        std::cout << '\n';
    }

    std::cout << "Information:\t\t\t";

    for (size_t i = index + 1; i < data_.size(); i++) {
        if (data_[i] == '\0') {
            std::cout << "\n\t\t\t\t";
        } else {
            std::cout << data_[i];
        }

    }
}

void URLframe::Print() {
    Frame::Print();
    std::cout << "URL:\t\t\t\t";

    for (unsigned char i : data_) {
        if (i == 0) {
            std::cout << '\n';
        } else {
            std::cout << i;
        }
    }
}

void WXXXframe::Print() {
    Frame::Print();
    GetEncoding();

    std::cout << "URL:\t\t\t\t";

    for (size_t i = 1; i < data_.size(); i++) {
        if (data_[i] == 0) {
            if (i == 1) {
                continue;
            }

            std::cout << ' ';
        } else {
            std::cout << data_[i];
        }
    }
}

void USLTframe::Print() {
    Frame::Print();
    GetEncoding();
    std::cout << "Language:\t\t\t";

    for (size_t i = 1; i < 4; i++) {
        std::cout << data_[i];
    }

    std::cout << "\nContent descriptor:\t\t";

    for (size_t i = 4; i < data_.size(); i++) {
        if (data_[i] == 0) {
            std::cout << "\nLyrics/text:\n\n";
        } else {
            std::cout << data_[i];
        }
    }
}

void SYLTframe::Print() {
    Frame::Print();
    GetEncoding();
    std::cout << "Language:\t\t\t";

    for (size_t i = 1; i < 4; i++) {
        std::cout << data_[i];
    }

    std::cout << '\n' << "Time stamp format:\t\t";

    if (data_[4] == 1) {
        std::cout << "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit";
    } else {
        std::cout << "Absolute time, 32 bit sized, using milliseconds as unit";
    }

    std::cout << '\n' << "Content type:\t\t\t";

    if (data_[5] == 0) {
        std::cout << "other\n";
    } else if (data_[5] == 1) {
        std::cout << "lyrics\n";
    } else if (data_[5] == 2) {
        std::cout << "text transcription\n";
    } else if (data_[5] == 3) {
        std::cout << "movement/part name\n";
    } else if (data_[5] == 4) {
        std::cout << "events\n";
    } else if (data_[5] == 5) {
        std::cout << "chord\n";
    } else if (data_[5] == 6) {
        std::cout << "trivia/'pop up' information\n";
    } else if (data_[5] == 7) {
        std::cout << "URLs to webpages\n";
    } else {
        std::cout << "URLs to images\n";
    }

    std::cout << "Content descriptor:\t\t";

    size_t index = 0;
    for (size_t i = 6; i < data_.size(); i++) {
        if (data_[i] == 0) {
            std::cout << '\n';
            index = i + 1;
            break;
        } else {
            std::cout << data_[i];
        }
    }

    for (size_t i = index; i < data_.size(); i++) {
        if (data_[i] == 0) {
           std::cout << "\t\t\t" << static_cast<uint32_t>((data_[i + 1] << 24) |
           (data_[i + 2] << 16) | (data_[i + 3] << 8) | data_[i + 4]) << " ms";
           i += 4;
        } else {
            std::cout << data_[i];
        }
    }

}

void COMMframe::Print() {
    Frame::Print();
    GetEncoding();

    std::cout << "Language:\t\t\t";

    for (size_t i = 1; i < 4; i++) {
        std::cout << data_[i];
    }

    std::cout << "\nShort content description:\t";

    for (size_t i = 4; i < data_.size(); i++) {
        if (data_[i] == 0) {
            std::cout << "\nThe actual text:\t\t";
        } else {
            std::cout << data_[i];
        }
    }
}

void ETCOframe::Print() {
    Frame::Print();

    std::cout << "Time stamp format:\t\t";

    if (data_[0] == 1) {
        std::cout << "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit";
    } else {
        std::cout << "Absolute time, 32 bit sized, using milliseconds as unit";
    }

    std::cout << "\n\n";

    for (size_t i = 1; i < data_.size(); i += 5) {
        std::cout << "Type of event:\t\t\t" << Events(data_[i]) << '\n';
        std::cout << "Time stamp:\t\t\t" << static_cast<uint32_t>((data_[i + 1] << 24) | (data_[i + 2] << 16) | (data_[i + 3] << 8) | data_[i + 4])/1000 << "sec\n\n";
    }
}

void RVA2frame::Print() {
    Frame::Print();

    std::cout << "Identification:\t\t";

    size_t index = 0;

    for (size_t i = 1; i < data_.size(); i++) {
        if (data_[i] == 0) {
            std::cout << '\n';
            index = i + 1;
            break;
        } else {
            std::cout << data_[i];
        }
    }

    for (size_t i = index; i < data_.size(); i += 5) {
        std::cout << "Type of channel:\t\t" << data_[i];
        std::cout << "Volume adjustment:\t\t" << data_[i + 1] << data_[i + 2];
        std::cout << "Bits representing peak:\t\t" << data_[i + 3];
        std::cout << "Peak volume:\t\t" << data_[i + 4];

    }
}

void EQU2frame::Print() {
    Frame::Print();

    std::cout << "Interpolation method:\t\t";
    if (data_[0] == 0) {
        std::cout << "Band\n";
    } else {
        std::cout << "Linear\n";
    }

    std::cout << "Identification:\t\t";

    size_t index = 1;

    while(data_[index] != 0) {
        std::cout << data_[index++];
    }

    std::cout << "Frequency:\t\t\t" << static_cast<uint16_t>(data_[index + 1] << 8 | data_[index + 2]);
    std::cout << "Volume adjustment:\t\t" << static_cast<uint16_t>(data_[index + 3] << 8 | data_[index + 4]);
}

void RVRBframe::Print() {
    Frame::Print();

    std::cout << "Reverb left (ms):\t\t" << static_cast<uint16_t>(data_[0] << 8 | data_[1])/2.55 << '%';
    std::cout << "Reverb right (ms):\t\t" << static_cast<uint16_t>(data_[2] << 8 | data_[3])/2.55 << '%';
    std::cout << ":\t\t\t" << data_[4]/2.55 << '%';
    std::cout << "Reverb bounces, left:\t\t\t" << data_[5]/2.55 << '%';
    std::cout << "Reverb bounces, right:\t\t\t" << data_[6]/2.55 << '%';
    std::cout << "Reverb feedback, left to left:\t\t\t" << data_[7]/2.55 << '%';
    std::cout << "Reverb feedback, left to right:\t\t\t" << data_[9]/2.55 << '%';
    std::cout << "Reverb feedback, right to right:\t\t\t" << data_[10]/2.55 << '%';
    std::cout << "Reverb feedback, right to left:\t\t\t" << data_[11]/2.55 << '%';
    std::cout << "Premix left to right:\t\t\t" << data_[12]/2.55 << '%';
    std::cout << "Premix right to left:\t\t\t" << data_[13]/2.55 << '%';
}

void PCNTframe::Print() {
    Frame::Print();

    std::cout << "Counter:\t\t\t" << static_cast<uint32_t>((data_[1] << 24) |
       (data_[2] << 16) | (data_[3] << 8) | data_[4]);
}

void POPMframe::Print() {
    Frame::Print();

    std::cout << "Email to user:\t\t\t";

    for (size_t i = 0; i < data_.size(); i++) {
        if (data_[i] == 0) {

            std::cout << "\nRating:\t\t\t\t" << data_[i + 1] << '\n';
            std::cout << "Counter:\t\t\t" << static_cast<uint32_t>((data_[i + 2] << 24) |
                                                                   (data_[i + 3] << 16) | (data_[i + 4] << 8) | data_[i + 5]);
            break;
        }
        std::cout << data_[i];
    }
}

void RBUFframe::Print() {
    Frame::Print();

    std::cout << "Buffer size:\t\t\t" << static_cast<uint32_t>((data_[0] << 16) | (data_[1] << 8) | data_[2]) << '\n';
    std::cout << "Embedded info flag:\t\t";
    if (data_[3] == 1) {
        std::cout << "True\n";
    } else {
        std::cout << "False\n";
    }

    std::cout << "Offset to next tag" << static_cast<uint32_t>((data_[4] << 24) | (data_[5] << 16) |
                                                                (data_[6] << 8) | data_[7]);

}

void Frame::SetId(const std::basic_string<char>& str) {
    identifier_ = str;
}

void Frame::SetSize(const uint32_t& size) {
    size_ = size;
}

void Frame::SetFlags(const uint16_t& flags) {
    flags_ = flags;
}

void Frame::SetData(const std::vector<uint8_t>& data) {
    data_ = data;
}

const std::string& Frame::id() const {
    return identifier_;
}

uint32_t Frame::size() const {
    return size_;
}

uint16_t Frame::flags() const {
    return flags_;
}

const std::vector<uint8_t>& Frame::data() const {
    return data_;
}

Frame::Frame(FrameHeader& header)
        : identifier_(header.id)
        , size_(header.size)
        , flags_(header.flags)
{

}

TEXTframe::TEXTframe(FrameHeader& header) : Frame(header)
{

}

URLframe::URLframe(FrameHeader& header) : Frame(header)
{

}

WXXXframe::WXXXframe(FrameHeader& header) : Frame(header) {

}

USLTframe::USLTframe(FrameHeader& header) : Frame(header) {

}

SYLTframe::SYLTframe(FrameHeader& header) : Frame(header) {

}

COMMframe::COMMframe(FrameHeader& header) : Frame(header) {

}

ETCOframe::ETCOframe(FrameHeader& header) : Frame(header) {

}

std::string ETCOframe::Events(size_t i) {
    std::string event;
    if (i == 0) {
        event = "padding";
    } else if (i == 1) {
        event = "end of initial silence";
    } else if (i == 2) {
        event = "intro start";
    } else if (i == 3) {
        event = "main part start";
    } else if (i == 4) {
        event = "outro start";
    } else if (i == 5) {
        event = "outro end";
    } else if (i == 6) {
        event = "verse start";
    } else if (i == 7) {
        event = "refrain start";
    } else if (i == 8) {
        event = "interlude start";
    } else if (i == 9) {
        event = "theme start";
    } else if (i == 10) {
        event = "variation start";
    } else if (i == 11) {
        event = "key change";
    } else if (i == 12) {
        event = "time change";
    } else if (i == 13) {
        event = "momentary unwanted noise (Snap, Crackle & Pop)";
    } else if (i == 14) {
        event = "sustained noise";
    } else if (i == 15) {
        event = "sustained noise end";
    } else if (i == 16) {
        event = "intro end";
    } else if (i == 17) {
        event = "main part end";
    } else if (i == 18) {
        event = "verse end";
    } else if (i == 19) {
        event = "refrain end";
    } else if (i == 20) {
        event = "theme end";
    } else if (i == 21) {
        event = "profanity";
    } else if (i == 22) {
        event = "profanity end";
    }  else if (i > 22 && i < 224) {
        event = "reserved for future use";
    } else if (i > 223 && i < 240) {
        event = "not predefined synch 0-F";
    } else if (i > 239 && i < 253) {
        event = "reserved for future use";
    } else if (i == 253) {
        event = "audio end (start of silence)";
    } else if (i == 254) {
        event = "audio file ends";
    } else if (i == 255) {
        event = "one more byte of events follows";
    }

    return event;
}

RVA2frame::RVA2frame(FrameHeader& header) : Frame(header) {

}

EQU2frame::EQU2frame(FrameHeader& header) : Frame(header) {

}

RVRBframe::RVRBframe(FrameHeader& header) : Frame(header) {

}

PCNTframe::PCNTframe(FrameHeader& header) : Frame(header) {

}

POPMframe::POPMframe(FrameHeader& header) : Frame(header) {

}

RBUFframe::RBUFframe(FrameHeader& header) : Frame(header) {

}
