import os
import zipfile

version = '1.0.0'
path = './__trash/'
dirs = ['src', 'examples']
files = ['library.properties', 'keywords.txt', 'README.md']

zip_filename = os.path.join(path, 'ardukit-'+version+'.zip')

with zipfile.ZipFile(zip_filename, "w") as zf:
    for filename in files:
        zf.write(os.path.join('.', filename))

    for dir in dirs:
        for dirname, subdirs, files in os.walk(dir):
            zf.write(dirname)
            for filename in files:
                zf.write(os.path.join(dirname, filename))

print(zip_filename + ' created.')
