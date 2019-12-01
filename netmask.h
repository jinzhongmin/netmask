#ifndef _NETMASK_H_
#define _NETMASK_H_

typedef unsigned char Seg;
typedef struct _s_mask Mask;
typedef struct _s_ip_address IpAddress;

struct _s_mask {
    int len;
    Seg segs[4];
};

struct _s_ip_address {
    Seg ip[4];

    Mask mask;
    Seg network[4];
    Seg broadcast[4];
};

Seg new_seg_by_len(int len);
int seg_get_len(Seg s);

Mask* new_mask(Seg s1, Seg s2, Seg s3, Seg s4);
Mask* new_mask_by_len(int len);
void mask_destory(Mask* mask);
int mask_get_len(const Mask mask);

IpAddress* new_ipaddress(Seg i1, Seg i2, Seg i3, Seg i4, Mask mask);
void ipaddress_destory(IpAddress* ip);

#endif