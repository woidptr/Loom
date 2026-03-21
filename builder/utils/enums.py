from enum import IntEnum


class SignatureType(IntEnum):
    DIRECT = 0
    FIELD = 1
    VTABLE_INDEX = 2
    VTABLE_PTR = 3
