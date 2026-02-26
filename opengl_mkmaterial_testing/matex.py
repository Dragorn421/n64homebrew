import argparse
import json
from pathlib import Path
import sys
import tempfile

import bpy


azAZ09 = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")


def sanitize(s: str):
    return "".join(c if c in azAZ09 else "_" for c in s)


if "--" in sys.argv:
    argv = sys.argv[sys.argv.index("--") + 1 :]
else:
    argv = []

parser = argparse.ArgumentParser()
parser.add_argument("out_jmat")
parser.add_argument("material_names", nargs="+")
args = parser.parse_args(argv)

out_jmat_p = Path(args.out_jmat)
textures_p = out_jmat_p.parent / "pngs"

textures_p.mkdir(parents=True, exist_ok=True)

matlib = {}
images = {}

with tempfile.TemporaryDirectory() as tmpdir:
    tmpdir_p = Path(tmpdir)
    tmpfile_p = tmpdir_p / "mat.jmat"
    for material_name in args.material_names:
        with bpy.context.temp_override(material=bpy.data.materials[material_name]):
            bpy.ops.libdragon_rdpq.rdpq_material_export(out_file=str(tmpfile_p))
        with tmpfile_p.open() as f:
            mat_data = json.load(f)
        for i in (0, 1):
            key = f"tex{i}.name"
            if key in mat_data[material_name]:
                image_name = mat_data[material_name][key]
                if image_name not in images:
                    image_path = str(textures_p / f"{sanitize(image_name)}.png")
                    assert image_path not in images.values()  # I'm lazy
                    bpy.data.images[image_name].save(filepath=image_path)
                    images[image_name] = image_path
                mat_data[material_name][key] = images[image_name]
        matlib.update(mat_data)

out_jmat_p.write_text(json.dumps(matlib, indent=0))
