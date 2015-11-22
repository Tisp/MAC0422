from . import filesystem
from . import directory
import time


def test():
	try:
		#filesystem.mount("data/filesystem.bin")
		mount("data/filesystem.bin")

		print(filesystem.bitmap)
		print(filesystem.fat)
		print()

		dirname = "/diretorio/"
		filename = dirname + "arquivo"

		#mkdir(dirname)
		#cp("/home/user/desktop/tempos", filename)

		#print(cat(filename))

		#rm(filename)
		#rmdir(dirname)

		#print(cat("/diretorio/arquivo"))
		#print(cat("/diretorio/subdir1/subarquivo1"))
		#rmdir("/diretorio")

		print(filesystem.bitmap)
		print(filesystem.fat)
		print()
		ls_recursive(filesystem.root)

		umount()

	except:
		print("Erro!")
		try: umount()
		except: pass
		raise


def mount(filepath):
	try:
		filesystem.mount(filepath)
	except FileNotFoundError:
		with open(filepath, 'wb') as f: f.write(bytes([0]*100000000))
		mount(filepath)
		filesystem.zero()
	except:
		raise Exception("Arquivo {} nao e um sistema de arquivos valido".format(filepath))


def umount():
	check_mounted()
	filesystem.umount()


def check_mounted():
	if filesystem.device is None:
		raise Exception("Sistema de arquivos nao montado")


def update(dir):
	check_mounted()
	now = int(time.time())
	if dir is not filesystem.root:
		dir.entry.time_access = now
		dir.entry.time_modification = now
		dir.entry.commit()


def mkdir(path):
	check_mounted()
	base,name = directory.Directory.splitpath(path)
	dir = filesystem.root.getdir_bypath(base)

	newentry = dir.getentry_empty()
	newentry.name = name
	newentry.filetype = 'dir'
	newentry.size = 0
	now = int(time.time())
	newentry.time_creation = now
	newentry.time_modification = now
	newentry.time_access = now
	newentry.sector = filesystem.alloc(directory.Directory.size)
	newentry.commit()

	directory.Directory(newentry, True).commit()

	update(dir)


def rmdir_recurse(dir):
	for entry in dir:
		if entry.filetype == 'dir':
			rmdir_recurse(directory.Directory(entry))
			filesystem.free(entry.sector)
		elif entry.filetype == 'file':
			if entry.size > 0: filesystem.free(entry.sector)


def rmdir(path):
	check_mounted()
	base,name = directory.Directory.splitpath(path)

	if name == '':
		filesystem.zero()
		return

	dir = filesystem.root.getdir_bypath(base)

	entry = dir.getentry_byname(name)

	rmdir_recurse(directory.Directory(entry))
	filesystem.free(entry.sector)
	entry.clear()
	entry.commit()

	update(dir)


def ls_recursive(dir):
	print(dir)
	print()

	for entry in dir:
		if entry.filetype == 'dir':
			print("==========")
			print(entry.name)
			print("==========")
			ls_recursive(directory.Directory(entry))


def ls(path):
	check_mounted()
	path = path.strip('/')
	return str(filesystem.root.getdir_bypath(path))


def create_file(base, name):
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_empty()
	entry.name = name
	entry.filetype = 'file'
	entry.size = 0
	entry.sector = 0
	now = int(time.time())
	entry.time_creation = now
	entry.time_modification = now
	entry.time_access = now
	return entry


def cp(origin, destination):
	check_mounted()
	base,name = directory.Directory.splitpath(destination)
	with open(origin, "rb") as file_origin:
		data = file_origin.read()
		size = len(data)
		entry = create_file(base, name)
		entry.sector = filesystem.alloc(size)
		entry.size = size
		filesystem.write(entry.sector*filesystem.sector_size, data)
		entry.commit()
		update(dir)


def rm(filepath):
	check_mounted()
	base,name = directory.Directory.splitpath(filepath)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	if entry.size > 0: filesystem.free(entry.sector)
	entry.clear()
	entry.commit()
	update(dir)


def cat(filepath):
	check_mounted()
	base,name = directory.Directory.splitpath(filepath)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	entry.time_access = int(time.time())
	entry.commit()
	update(dir)
	return filesystem.read(filesystem.sector_size*entry.sector, entry.size).decode('utf-8')


def touch(filepath):
	check_mounted()
	base,name = directory.Directory.splitpath(filepath)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	entry.time_access = int(time.time())
	entry.commit()
	update(dir)


def find(path, filename):
	check_mounted()


def df():
	check_mounted()
