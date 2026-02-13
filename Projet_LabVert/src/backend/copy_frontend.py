import shutil
import os

frontend_dir = "../frontend/"
backend_dir = "data/"

files = ["index.html", "style.css", "script.js"]

print("Copie automatique du frontend vers backend/data...")

for f in files:
    src = os.path.join(frontend_dir, f)
    dst = os.path.join(backend_dir, f)

    if os.path.exists(src):
        shutil.copy(src, dst)
        print(f"Copié : {f}")
    else:
        print(f"Introuvable : {src}")
