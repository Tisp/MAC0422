from . import filesystem
from . import directory
import time


def test():
	try:
		filesystem.mount("data/filesystem.bin")

		print(filesystem.bitmap)
		print(filesystem.fat)

		dirname = "/diretorio/"
		filename = dirname + "arquivo"

		#mkdir(dirname)
		#cp("/home/user/desktop/dev/so/ep3/data/restore.sh", filename)

		print(cat(filename))

		#rm(filename)
		#rmdir(dirname)


		print(filesystem.bitmap)
		print(filesystem.fat)

		print("==========")
		print(ls("/"))
		print("==========")
		for entry in filesystem.root:
			if entry.filetype == 'dir':
				print(ls("/" + entry.name))
				print("==========")

		umount()

	except:
		print("Erro!")
		raise


def mount(filepath):
	#ve se o arquivo existe e eh do tamanho certo, cria se nao existir
	pass


def umount():
	if filesystem.device is not None:
		filesystem.umount()
	else:
		raise Exception("Sistema de arquivos nao montado")


def mkdir(path):
	base,name = directory.Directory.splitpath(path)
	dir = filesystem.root.getdir_bypath(base)

	#@todo testar se tem espaco livre, se cabe na base, etc

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


def rmdir(path):
	base,name = directory.Directory.splitpath(path)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	filesystem.free(entry.sector)
	entry.clear()
	entry.commit()
	now = int(time.time())
	dir.entry.time_access = now
	dir.entry.time_modification = now
	dir.entry.commit()


def ls(path):
	path = path.strip('/')
	return str(filesystem.root.getdir_bypath(path))


def create_file(base, name):
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_empty()
	entry.name = name
	entry.filetype = 'file'
	entry.size = 0
	now = int(time.time())
	entry.time_creation = now
	entry.time_modification = now
	entry.time_access = now
	entry.sector = 0
	dir.entry.time_access = now
	dir.entry.time_modification = now
	dir.entry.commit()
	return entry


def cp(origin, destination):
	#@todo testar origem, espaco, etc
	#@todo copiar arquivo de verdade
	base,name = directory.Directory.splitpath(destination)
	with open(origin, "rb") as file_origin:
		data = file_origin.read()
		size = len(data)
		entry = create_file(base, name)
		entry.sector = filesystem.alloc(size)
		entry.size = size
		filesystem.write(entry.sector*filesystem.sector_size, data)
		entry.commit()


def rm(filepath):
	base,name = directory.Directory.splitpath(filepath)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	if entry.size > 0: filesystem.free(entry.sector)
	entry.clear()
	entry.commit()
	now = int(time.time())
	dir.entry.time_access = now
	dir.entry.time_modification = now
	dir.entry.commit()


def cat(filepath):
	base,name = directory.Directory.splitpath(filepath)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	now = int(time.time())
	dir.entry.time_access = now
	dir.entry.time_modification = now
	dir.entry.commit()
	entry.time_access = now
	entry.commit()
	return filesystem.read(filesystem.sector_size*entry.sector, entry.size).decode('utf-8')


def touch(filepath):
	base,name = directory.Directory.splitpath(filepath)
	dir = filesystem.root.getdir_bypath(base)
	entry = dir.getentry_byname(name)
	now = int(time.time())
	dir.entry.time_access = now
	dir.entry.time_modification = now
	dir.entry.commit()
	entry.time_access = now
	entry.commit()


def find(path, filename):
	pass


def df():
	pass
