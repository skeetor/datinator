/*******************************************************************************
 *
 * SupportLib (c) by Gerhard W. Gruber in 2014
 *
 ******************************************************************************/

#include <support/support_dll_api.h>

namespace spt
{
namespace image
{
const unsigned char button_add_data[] =
{
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF,
0x61, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47,
0x44, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xA0,
0xBD, 0xA7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70,
0x48, 0x59, 0x73, 0x00, 0x00, 0x0B, 0x13, 0x00,
0x00, 0x0B, 0x13, 0x01, 0x00, 0x9A, 0x9C, 0x18,
0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45,
0x07, 0xDD, 0x07, 0x18, 0x0C, 0x15, 0x18, 0x68,
0x3A, 0x81, 0x17, 0x00, 0x00, 0x01, 0x85, 0x49,
0x44, 0x41, 0x54, 0x38, 0xCB, 0x9D, 0x92, 0x3F,
0x4B, 0x5C, 0x41, 0x14, 0xC5, 0x7F, 0x33, 0x6F,
0x76, 0x95, 0x8D, 0x45, 0xD0, 0x62, 0x21, 0xC4,
0x42, 0xB2, 0x65, 0xC0, 0x4A, 0x2B, 0x09, 0xA6,
0xB5, 0x4B, 0x95, 0x56, 0xFC, 0x0E, 0x29, 0xF3,
0x0D, 0xD2, 0x06, 0xAC, 0xB6, 0x4A, 0x9A, 0xE0,
0x37, 0x08, 0x29, 0x82, 0x36, 0xA2, 0xA2, 0x04,
0x82, 0x8D, 0x12, 0x25, 0xB0, 0x61, 0x0B, 0x57,
0x16, 0xF7, 0xEF, 0x7B, 0x73, 0xEF, 0x4D, 0xF1,
0x50, 0x92, 0xDD, 0xB7, 0x0B, 0xEB, 0x74, 0x33,
0xFC, 0xE6, 0x70, 0xCF, 0x3D, 0xC7, 0x51, 0x70,
0x36, 0xEA, 0x35, 0xAB, 0x2E, 0xCF, 0x13, 0xCA,
0x25, 0x00, 0x62, 0x9A, 0xD1, 0xFC, 0x3D, 0xE0,
0x60, 0xE7, 0xC2, 0x8D, 0xB2, 0xA1, 0x48, 0xA0,
0x97, 0xA6, 0xBC, 0x5D, 0xDB, 0xE2, 0x67, 0xEB,
0x13, 0x00, 0xED, 0xAE, 0xE7, 0xD7, 0x65, 0xA5,
0x08, 0x2D, 0x16, 0x00, 0x40, 0x1D, 0x25, 0x3F,
0x07, 0x40, 0x62, 0x13, 0xA9, 0xC9, 0x02, 0x51,
0x1D, 0xA2, 0x20, 0x0A, 0xDD, 0xA1, 0x9F, 0x5D,
0x00, 0x15, 0x32, 0xF1, 0x18, 0x10, 0x65, 0xCA,
0x04, 0x1B, 0xF5, 0x5A, 0xE1, 0x80, 0x69, 0xE6,
0xE8, 0x67, 0x0E, 0x35, 0x48, 0x33, 0xA1, 0xDD,
0xE9, 0xF3, 0xE2, 0xC3, 0xE2, 0x18, 0x1B, 0x4A,
0xCC, 0xB1, 0xF3, 0x66, 0x05, 0x73, 0x9E, 0xA8,
0xF9, 0x07, 0xCD, 0x02, 0x67, 0x37, 0x5F, 0x40,
0x03, 0x8A, 0xA7, 0xB2, 0xE0, 0x78, 0xB5, 0x99,
0xF0, 0xB2, 0xBA, 0x8E, 0x5A, 0x6E, 0x4B, 0x05,
0xF6, 0xBE, 0x1E, 0x13, 0xD2, 0x81, 0xF2, 0xA3,
0x79, 0x84, 0xA3, 0x84, 0x08, 0x28, 0x10, 0xD5,
0xA3, 0x5A, 0x46, 0x0C, 0x54, 0x41, 0xCC, 0x63,
0x06, 0x67, 0x8D, 0x13, 0xB8, 0x17, 0x00, 0xDA,
0x77, 0x1D, 0x82, 0x88, 0x11, 0xA3, 0xC3, 0x3B,
0xC8, 0x14, 0x4C, 0x3D, 0xD1, 0xC0, 0x00, 0x11,
0x87, 0x00, 0x31, 0xE6, 0x77, 0x35, 0x88, 0x0A,
0x18, 0x28, 0x1E, 0x89, 0x10, 0x62, 0x6A, 0x5C,
0x5C, 0x25, 0xF9, 0x2B, 0x00, 0xF9, 0xC6, 0x56,
0xAB, 0xAF, 0xB9, 0xD6, 0x7D, 0xD4, 0xE0, 0x4F,
0x23, 0x02, 0xF0, 0xA4, 0x3C, 0x92, 0xD4, 0x50,
0x09, 0xDD, 0xDE, 0x90, 0xD3, 0xEF, 0xFF, 0xEF,
0xE6, 0xB6, 0xD7, 0x62, 0x75, 0x9B, 0x07, 0x4B,
0x00, 0x87, 0xDF, 0x6E, 0x8A, 0x53, 0x38, 0x7F,
0x7F, 0x35, 0x56, 0xCF, 0xE7, 0xEF, 0x9E, 0xD9,
0xD3, 0xCA, 0x12, 0xC3, 0x96, 0x07, 0x04, 0x71,
0x39, 0xD2, 0xFD, 0x38, 0x18, 0x63, 0x27, 0x36,
0x24, 0x13, 0xC3, 0x4C, 0x10, 0x4B, 0x30, 0x7D,
0x44, 0x13, 0xC5, 0x40, 0x34, 0x41, 0xC9, 0xAD,
0xCC, 0xDE, 0xC4, 0x7F, 0xE2, 0xD2, 0xC7, 0x4C,
0xB0, 0xFB, 0x79, 0x8F, 0xBB, 0xB4, 0x9D, 0x0B,
0x4C, 0xE9, 0xF2, 0x5F, 0x8C, 0x68, 0xCB, 0x3B,
0xB0, 0x11, 0xAF, 0x40, 0x00, 0x00, 0x00, 0x00,
0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

SUPPORT_DLL_EXPORT const unsigned char *button_add = &button_add_data[0];
SUPPORT_DLL_EXPORT int button_add_length = sizeof(button_add_data);

}
}
