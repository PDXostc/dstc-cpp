struct SimpleStruct {
    char a;
    uint32_t b;
} __attribute__((packed));

struct DifferentSimpleStruct {
    uint16_t a;
    char b;
    uint32_t c;
} __attribute__((packed));

struct ForManipulation {
    char ch;
    uint16_t u16;
    int i;
    double d;
} __attribute__((packed));

struct StructA {
    uint16_t a;
    uint8_t b;
    uint8_t c;
} __attribute__((packed));

struct StructB {
    uint16_t d;
    uint8_t e;
    uint16_t f;
} __attribute__((packed));

struct Struct16 {
    uint16_t a;
    uint16_t d;
    uint16_t f;
} __attribute__((packed));

struct Struct8 {
    uint8_t b;
    uint8_t c;
    uint8_t e;
} __attribute__((packed));