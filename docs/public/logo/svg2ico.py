import sys
import subprocess

# 尝试导入 Pillow，如果未安装则自动安装
try:
    from PIL import Image
except ImportError:
    print("Pillow 未安装，正在安装...")
    subprocess.check_call([sys.executable, "-m", "pip", "install", "Pillow"])
    from PIL import Image

# 文件路径
svg_path = "./logo.png"
icon_path = "./logo.ico"

# 加载 PNG 文件并转换为 ICO
img = Image.open(svg_path)
icon_sizes = [(16, 16), (32, 32), (48, 48), (64, 64), (128, 128)]
img.save(icon_path, format='ICO', sizes=icon_sizes)

print("ICO 文件已保存到:", icon_path)
