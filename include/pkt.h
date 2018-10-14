/**
 * desc: 数据包结构体定义
 * file: pkt.h
 *
 * author:  myw31415926
 * date:    20181013
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#ifndef _PKT_V01_H
#define _PKT_V01_H

#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: 二进制数据包结构体定义
 *
 * |<-----   Total length: pkt_hdr.len * 4   ----->|
 *
 * +-----------+-------------------------------+---+
 * |  pkt_hdr  | payload                       |   |
 * +-----------+-------------------------------+---+
 *                                             ^
 * + 最后为填充字节，确保数据包长度为4的整数倍 |
 */
typedef struct {
    /* byte 0 */
    unsigned short sync;     /* 同步标志0x7e5a (魔术字)                   */
    unsigned short len;      /* 数据包字长 (4-byte)，包括头部             */

    /* byte 4 */
    unsigned char  type;     /* 数据包类型                                */
    unsigned char  subtype;  /* 数据包字类型                              */
    unsigned short filling;  /* 填充                                      */
                             /* Bit 0-1: Number of filler bytes, 0-3      */

    /* byte 8 */
} pkt_hdr;

#define PKT_SYNC_FLAG 0x7e5a

static inline unsigned short pkthdr_get_sync(pkt_hdr *ph)
{
    return ntohs(ph->sync);
}

static inline void pkthdr_set_sync(pkt_hdr *ph)
{
    ph->sync = htons(PKT_SYNC_FLAG);
}

static inline int pkthdr_valid_sync(pkt_hdr *ph)
{
    return pkthdr_get_sync(ph) == PKT_SYNC_FLAG;
}

static inline unsigned short pkthdr_get_flen(pkt_hdr *ph)
{
    return (ph->filling & 0x03);
}

#ifdef __cplusplus
}
#endif

#endif /* _PKT_V01_H */