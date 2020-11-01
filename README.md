# libtlv

![CMake](https://github.com/christianhujer/libtlv/workflows/CMake/badge.svg)

A TLV (Tag Length Value) library inspired by (but not the same) BER-TLV and UTF-8.

The library is valgrind-tested.

## Variable Length Encoding
`libtlv` uses a variable length encoding inspired by UTF-8.

The scheme is as follows:
| Length | Bits | Range                | Scheme                                                             |
| ------ | ---- | -------------------- | ------------------------------------------------------------------ |
| 1      |    7 |         0…127        | `0xxx xxxx`                                                        |
| 2      |   11 |       128…2047       | `110x xxxx  10xx xxxx`                                             |
| 3      |   16 |      2048…65535      | `1110 xxxx  10xx xxxx  10xx xxxx`                                  |
| 4      |   21 |     65536…2097151    | `1110 xxxx  10xx xxxx  10xx xxxx  10xx xxxx`                       |
| 5      |   26 |   2097152…67108863   | `1110 xxxx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx`            |
| 6      |   31 |  67108864…2147483647 | `1110 xxxx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx  10xx xxxx` |
