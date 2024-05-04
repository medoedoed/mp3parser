#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>


struct FrameHeader {
    std::string id;
    uint32_t size;
    uint16_t flags;
};

class Frame {
public:
    Frame(FrameHeader& header);

    void SetId(const std::basic_string<char>& str);
    void SetSize(const uint32_t& size);
    void SetFlags(const uint16_t& flags);
    void SetData(const std::vector<uint8_t>& data);

    const std::string& id() const;
    uint32_t size() const;
    uint16_t flags() const;
    const std::vector<uint8_t>& data() const;

    virtual void Print();
    void GetEncoding();

    bool Good();
    void Bad();
protected:
    std::string identifier_;
    uint32_t size_;
    uint16_t flags_;
    std::vector<uint8_t> data_;

    bool good_ = true;
};

class TEXTframe : public Frame {
public:

    explicit TEXTframe(FrameHeader& header);
    void Print();
};

class URLframe : public Frame {
public:
    explicit URLframe(FrameHeader& header);
    void Print();
};

class WXXXframe : public Frame {
public:
    explicit WXXXframe(FrameHeader& header);
    void Print();
};

class USLTframe : public Frame {
public:
    explicit USLTframe(FrameHeader& header);
    void Print();
};

class SYLTframe : public Frame {
public:
    explicit SYLTframe(FrameHeader& header);
    void Print();
};

class COMMframe : public Frame {
public:
    explicit COMMframe(FrameHeader& header);
    void Print();
};

class ETCOframe : public Frame {
public:
    explicit ETCOframe(FrameHeader& header);
    void Print();
    std::string Events(size_t i);
};

class RVA2frame : public Frame {
public:
    explicit RVA2frame(FrameHeader& header);
    void Print();
};

class EQU2frame : public Frame {
public:
    explicit EQU2frame(FrameHeader& header);
    void Print();
};

class RVRBframe : public Frame {
public:
    explicit RVRBframe(FrameHeader& header);
    void Print();
};

class PCNTframe : public Frame {
public:
    explicit PCNTframe(FrameHeader& header);
    void Print();
};

class POPMframe : public Frame {
public:
    explicit POPMframe(FrameHeader& header);
    void Print();
};

class RBUFframe : public Frame {
public:
    explicit RBUFframe(FrameHeader& header);
    void Print();
};




