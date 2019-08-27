struct SimpleStruct {
    char a;
    uint32_t b;
} __attribute__((packed));

struct DifferentSimpleStruct {
    uint16_t a;
    char b;
    uint32_t c;
} __attribute__((packed));