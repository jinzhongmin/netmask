#include "netmask.h"
#include <stdlib.h>

Seg new_seg_by_len(int len) {
    if (len < 1) {
        return 0;
    } else if (len > 7) {
        return 255;
    }
    int seg = 65280;
    for (; len > 0; len--) {
        seg = seg >> 1;
    }
    return seg;
}

int seg_get_len(Seg s) {
    char seg = (char)s;
    int len = -1;
    for (; seg <= 0; seg = seg << 1) {
        len++;
        if (seg == 0) {
            return len;
        }
    }
    return -1;
}

Mask* new_mask(Seg s1, Seg s2, Seg s3, Seg s4) {
    int l1 = seg_get_len(s1);
    int l2 = seg_get_len(s2);
    int l3 = seg_get_len(s3);
    int l4 = seg_get_len(s4);

    Mask* mask = (Mask*)malloc(sizeof(Mask));
    if (l1 < 0 || l2 < 0 || l3 < 0 || l4 < 0) {
        free(mask);
        return NULL;
    }

    if (l1 == 8) {
        if (l2 == 8) {
            if (l3 == 8) {
                mask->segs[0] = s1;
                mask->segs[1] = s2;
                mask->segs[2] = s3;
                mask->segs[3] = s4;
                mask->len = l1 + l2 + l3 + l4;
                return mask;
            } else {
                if (l4 == 0) {
                    mask->segs[0] = s1;
                    mask->segs[1] = s2;
                    mask->segs[2] = s3;
                    mask->segs[3] = s4;
                    mask->len = l1 + l2 + l3 + l4;
                    return mask;
                }
                free(mask);
                return NULL;
            }
        } else {
            if (l3 == 0 && l4 == 0) {
                mask->segs[0] = s1;
                mask->segs[1] = s2;
                mask->segs[2] = s3;
                mask->segs[3] = s4;
                mask->len = l1 + l2 + l3 + l4;
                return mask;
            }
            free(mask);
            return NULL;
        }
    } else {
        if (l2 == 0 && l3 == 0 && l4 == 0) {
            mask->segs[0] = s1;
            mask->segs[1] = s2;
            mask->segs[2] = s3;
            mask->segs[3] = s4;
            mask->len = l1 + l2 + l3 + l4;
            return mask;
        }
        free(mask);
        return NULL;
    }

    free(mask);
    return NULL;
}

Mask* new_mask_by_len(int len) {
    Mask* mask;
    int tmp = len;
    if (len > 8) {
        len = len - 8;
        if (len > 8) {
            len = len - 8;
            if (len > 8) {
                len = len - 8;
                mask = new_mask(255, 255, 255, new_seg_by_len(len));
                mask->len = tmp;
                return mask;
            }
            mask = new_mask(255, 255, new_seg_by_len(len), 0);
            mask->len = tmp;
            return mask;
        }
        mask = new_mask(255, new_seg_by_len(len), 0, 0);
        mask->len = tmp;
        return mask;
    }
    mask = new_mask(new_seg_by_len(len), 0, 0, 0);
    mask->len = tmp;
    return mask;
}

void mask_destory(Mask* mask) {
    free(mask);
}

int mask_get_len(const Mask mask) {
    int len = 0;
    len += seg_get_len(mask.segs[0]);
    len += seg_get_len(mask.segs[1]);
    len += seg_get_len(mask.segs[2]);
    len += seg_get_len(mask.segs[3]);

    return len;
}

IpAddress* new_ipaddress(Seg i1, Seg i2, Seg i3, Seg i4, Mask mask) {
    IpAddress* ip = (IpAddress*)malloc(sizeof(IpAddress));

    int fm1 = ~mask.segs[0];
    int fm2 = ~mask.segs[1];
    int fm3 = ~mask.segs[2];
    int fm4 = ~mask.segs[3];

    ip->network[0] = i1 & mask.segs[0];
    ip->network[1] = i2 & mask.segs[1];
    ip->network[2] = i3 & mask.segs[2];
    ip->network[3] = i4 & mask.segs[3];

    ip->broadcast[0] = i1 | fm1;
    ip->broadcast[1] = i2 | fm2;
    ip->broadcast[2] = i3 | fm3;
    ip->broadcast[3] = i4 | fm4;

    ip->mask.segs[0] = mask.segs[0];
    ip->mask.segs[1] = mask.segs[1];
    ip->mask.segs[2] = mask.segs[2];
    ip->mask.segs[3] = mask.segs[3];

    ip->mask.len = mask.len;

    return ip;
}

void ipaddress_destory(IpAddress* ip) {
    free(ip);
}