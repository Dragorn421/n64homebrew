import argparse
import sys

import bpy


if "--" in sys.argv:
    argv = sys.argv[sys.argv.index("--") + 1 :]
else:
    argv = []

parser = argparse.ArgumentParser()
parser.add_argument("material_name")
parser.add_argument("out_jmat")
args = parser.parse_args(argv)

with bpy.context.temp_override(material=bpy.data.materials[args.material_name]):
    bpy.ops.libdragon_rdpq.rdpq_material_export(out_file=args.out_jmat)
