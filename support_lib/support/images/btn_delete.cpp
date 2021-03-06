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

unsigned char button_delete_data[] =
{
0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF,
0x61, 0x00, 0x00, 0x02, 0xB8, 0x49, 0x44, 0x41,
0x54, 0x78, 0xDA, 0x7D, 0x93, 0xC9, 0x4F, 0x53,
0x51, 0x14, 0xC6, 0xBF, 0xFB, 0x5E, 0xE7, 0xF6,
0x95, 0xF6, 0x51, 0xA4, 0xB4, 0x94, 0x96, 0x41,
0x6B, 0x64, 0x58, 0x38, 0xC4, 0xC4, 0x21, 0x8A,
0x81, 0x2A, 0x0B, 0x5D, 0xC8, 0x02, 0xE3, 0xC6,
0xBF, 0xCD, 0x8D, 0xD1, 0x85, 0x26, 0xE2, 0x10,
0x45, 0x12, 0x95, 0xA8, 0x31, 0x4A, 0x8C, 0x82,
0x2C, 0x90, 0x02, 0xBE, 0x56, 0x28, 0xB5, 0x23,
0x1D, 0x78, 0x9D, 0xDE, 0x3B, 0x5E, 0x4B, 0x82,
0x69, 0x10, 0xEE, 0xF2, 0x9E, 0xF3, 0xFD, 0xEE,
0x39, 0xDF, 0x3D, 0x87, 0x81, 0x9F, 0xFC, 0xF8,
0x35, 0x12, 0x3C, 0x32, 0xEA, 0x9F, 0x17, 0xD1,
0x1E, 0x5D, 0x64, 0x38, 0xE4, 0x64, 0x06, 0x86,
0xC9, 0x78, 0x66, 0x08, 0x7A, 0x2A, 0x07, 0xD7,
0xEC, 0x0B, 0xC6, 0x32, 0xA3, 0x63, 0x64, 0xB9,
0x71, 0x16, 0xCC, 0x48, 0xD0, 0x12, 0x55, 0xA8,
0x0F, 0x66, 0x70, 0xE4, 0x00, 0xC8, 0x6F, 0x2E,
0xB6, 0xDE, 0x8A, 0x40, 0xEC, 0x34, 0x03, 0x0D,
0x40, 0x9D, 0xFE, 0x04, 0x96, 0x1E, 0x1B, 0x25,
0xCB, 0x88, 0x0C, 0xBD, 0xB0, 0x0D, 0x21, 0x10,
0x82, 0x5E, 0xB6, 0xA1, 0x74, 0xFF, 0x25, 0x7C,
0xB1, 0xE5, 0x16, 0x48, 0xE2, 0xE8, 0x30, 0xD9,
0xA7, 0xAE, 0x42, 0x30, 0xE4, 0xA1, 0xFF, 0x52,
0xC0, 0x1C, 0x12, 0x2A, 0xDF, 0x52, 0x60, 0x31,
0x7F, 0x1F, 0xD9, 0x86, 0x3A, 0x61, 0xEE, 0x73,
0x83, 0xAA, 0x35, 0x30, 0xAF, 0x1F, 0x5A, 0xC9,
0x8A, 0xC2, 0xE3, 0xD7, 0xE8, 0x51, 0x76, 0x21,
0xB1, 0xFE, 0x21, 0x6A, 0x9B, 0x1A, 0x87, 0xC8,
0x72, 0xD0, 0x13, 0x71, 0x30, 0xB3, 0x19, 0x95,
0xD5, 0x2C, 0xD4, 0xC5, 0x2D, 0x34, 0x13, 0xD6,
0xBC, 0x21, 0xB2, 0x73, 0x88, 0x35, 0xD8, 0x06,
0xAA, 0xD5, 0x20, 0xF8, 0xBA, 0xD1, 0x20, 0x19,
0xF9, 0xE9, 0x39, 0x40, 0xD3, 0xE1, 0x9E, 0xBC,
0x02, 0x03, 0x65, 0xA0, 0x6D, 0x72, 0xB1, 0xD1,
0x88, 0x9D, 0x95, 0x2C, 0xCA, 0x3F, 0xB2, 0xE8,
0x4F, 0xAE, 0xB3, 0xBD, 0x32, 0x97, 0x39, 0xC4,
0x79, 0xA2, 0x03, 0x8E, 0x80, 0x04, 0xBD, 0x56,
0x87, 0xD0, 0xE1, 0x05, 0xB9, 0x7B, 0x00, 0x02,
0x58, 0x3E, 0xCE, 0x4D, 0x4B, 0x40, 0xE0, 0xE2,
0xED, 0x68, 0x0E, 0xE5, 0x68, 0x16, 0xE1, 0xDF,
0x4A, 0x53, 0xDB, 0xD2, 0xE7, 0xF7, 0xCE, 0x20,
0xB9, 0xC2, 0x32, 0x24, 0x9F, 0x04, 0xAA, 0x73,
0x88, 0xDD, 0xD1, 0xCC, 0xD0, 0x4B, 0x65, 0xC0,
0x60, 0x40, 0x31, 0x5E, 0x40, 0x7E, 0x25, 0x87,
0xE1, 0x94, 0xB2, 0xA7, 0xDB, 0xE7, 0xF6, 0x17,
0x4F, 0x80, 0xBC, 0x43, 0xBC, 0x12, 0x8F, 0x0D,
0x7A, 0xA3, 0xD1, 0xAC, 0x40, 0x30, 0x8A, 0x50,
0xB7, 0xAB, 0xD8, 0xF8, 0x9A, 0xC4, 0xC9, 0x54,
0xBC, 0x45, 0xB3, 0x0F, 0x30, 0xCF, 0x0D, 0x0B,
0x8C, 0x74, 0xC1, 0xD2, 0xE0, 0x6E, 0xD3, 0x6E,
0x98, 0x31, 0x42, 0xCD, 0x20, 0x21, 0xBE, 0x90,
0xC4, 0xA9, 0xD5, 0xA5, 0x83, 0x01, 0x1F, 0x7A,
0xC3, 0x14, 0x8A, 0x9C, 0x87, 0x23, 0xA7, 0x40,
0xCF, 0x66, 0xF8, 0x97, 0x19, 0x9A, 0xF7, 0x1A,
0xAF, 0x44, 0x74, 0xCB, 0x28, 0xBA, 0x83, 0x58,
0x7F, 0xFA, 0x16, 0x17, 0x36, 0xD7, 0xF6, 0xB7,
0x30, 0xD7, 0x7B, 0x8C, 0xFA, 0x27, 0x2E, 0xC1,
0x99, 0x8E, 0xF1, 0x81, 0xDA, 0x00, 0xFB, 0xDB,
0x73, 0xB9, 0xDE, 0x8C, 0x49, 0x76, 0x23, 0xA8,
0xA1, 0x41, 0xEC, 0x0E, 0x20, 0x63, 0x6A, 0xC7,
0xFA, 0xCC, 0x3B, 0x8C, 0x26, 0xD6, 0xFF, 0x99,
0xF8, 0x2A, 0x30, 0x40, 0xC7, 0x27, 0x46, 0xE1,
0xCA, 0xC4, 0xD1, 0x50, 0x14, 0x88, 0x66, 0x13,
0x12, 0x79, 0x15, 0xCA, 0x56, 0xA9, 0x09, 0x08,
0x7A, 0x1D, 0xE8, 0x72, 0x59, 0xA1, 0x55, 0xAB,
0x30, 0xF4, 0x04, 0x91, 0x6E, 0xF3, 0x62, 0x65,
0xF6, 0x3D, 0x22, 0xF1, 0x28, 0x63, 0xCF, 0xBB,
0xFB, 0x68, 0x30, 0x72, 0x19, 0x72, 0x32, 0x86,
0x46, 0x2C, 0x06, 0xD1, 0x64, 0xC2, 0x46, 0x51,
0x45, 0x34, 0x5D, 0xC6, 0xF5, 0xEC, 0x56, 0xF3,
0x81, 0x27, 0xB2, 0x97, 0x06, 0x3C, 0x76, 0xF8,
0x25, 0x0E, 0xE1, 0x73, 0x62, 0x0C, 0xF5, 0x22,
0xD3, 0xE1, 0xC7, 0xD2, 0xCC, 0x1B, 0xB0, 0x8F,
0xE7, 0x2E, 0xD2, 0x88, 0xD3, 0x8A, 0xFA, 0xEA,
0x1A, 0x04, 0x2E, 0x56, 0x76, 0x54, 0x2C, 0x67,
0x8B, 0x98, 0xDC, 0x4E, 0xB7, 0xF8, 0xF3, 0xB0,
0xCD, 0x43, 0x61, 0x59, 0x42, 0xD0, 0x66, 0xE5,
0x73, 0xC2, 0x21, 0xFD, 0x7D, 0x58, 0x28, 0xA8,
0x60, 0xCF, 0x7C, 0x41, 0x3A, 0x6D, 0x31, 0xC1,
0xC2, 0x04, 0xFC, 0xAC, 0x54, 0xB0, 0x54, 0x2C,
0xE1, 0x76, 0x21, 0xF3, 0xDF, 0x65, 0xBA, 0xE7,
0x6C, 0xA7, 0x41, 0xC9, 0x81, 0x90, 0xC5, 0x82,
0x0A, 0xE9, 0x98, 0xAF, 0xD4, 0x76, 0x3D, 0x78,
0xE4, 0xF4, 0x90, 0x4D, 0x14, 0x91, 0xAC, 0x55,
0x71, 0xA7, 0x9C, 0x3F, 0x74, 0x9D, 0xEF, 0xDA,
0x5D, 0xD4, 0x69, 0x32, 0x63, 0x47, 0xD3, 0x70,
0xB3, 0x90, 0x66, 0x7F, 0x00, 0x11, 0xE4, 0x2B,
0x31, 0x00, 0x9A, 0x23, 0x09, 0x00, 0x00, 0x00,
0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60,
0x82
};

SUPPORT_DLL_EXPORT const unsigned char *button_delete = &button_delete_data[0];
SUPPORT_DLL_EXPORT int button_delete_length = sizeof(button_delete_data);


}
}
