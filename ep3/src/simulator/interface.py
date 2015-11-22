from . import filesystem
from . import directory


def test():
	try:
		filesystem.mount("data/filesystem.bin")

		print(filesystem.bitmap)
		print(filesystem.fat)

		#pos = (filesystem.fat[0] % (filesystem.fat_size-1)) + 1
		#filesystem.fat[0] += 1

		#filesystem.bitmap[pos] = (filesystem.bitmap[pos]+1) % 2
		#filesystem.fat[pos] += 1

		#entry = filesystem.root.getentry(0)
		#entry.name = 'arquivo'
		#entry.size = 1
		#entry.filetype = 'file'
		#entry.time_creation = 10
		#entry.time_modification = 100
		#entry.time_access = 1000
		#entry.sector = 0

		#entry = filesystem.root.getentry(1)
		#entry.name = 'pasta'
		#entry.size = 2
		#entry.filetype = 'dir'
		#entry.time_creation = 20
		#entry.time_modification = 200
		#entry.time_access = 2000
		#entry.sector = 3
		#filesystem.root.commit()

		#dir = directory.Directory(filesystem.root.getentry(1).sector)
		#for entry in dir:
			#entry.name = 'dirfile'
			#entry.size = 3
			#entry.filetype = 'file'
			#entry.time_creation = 30
			#entry.time_modification = 300
			#entry.time_access = 3000
			#entry.sector = 0
		#dir.commit()

		#filesystem.fat = [1, 2, filesystem.fat_marker] + [0]*(filesystem.fat_size-3)
		#filesystem.bitmap = [True, True, True] + [False]*(filesystem.bitmap_size-3)
		#filesystem.umount()
		#return

		#path = "/pasta2"
		#base,name = directory.Directory.splitpath(path)
		#dir = filesystem.root.getdir_bypath(base)

		#dir.getentry(2).clear()
		#newentry = dir.getentry_empty()
		#newentry.name = name
		#newentry.filetype = 'dir'
		#newentry.size = 0
		#newentry.time_creation = 9
		#newentry.time_modification = 9
		#newentry.time_access = 9
		#newentry.sector = filesystem.alloc(120)
		#newentry.commit()

		#directory.Directory(newentry, True).commit()

		mkdir("/nivel1_1")
		mkdir("/nivel1_2")
		mkdir("/nivel1_1/nivel2-1_1")
		mkdir("/nivel1_2/nivel2-2_1")

		rmdir("/nivel1_1/nivel2-1_1")
		rmdir("/nivel1_2/nivel2-2_1")
		rmdir("/nivel1_1")
		rmdir("/nivel1_2")

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
    cmd = ['dd', 'if=/dev/zero', 'of='+path, 'bs=100m', 'count=1']
    a = subprocess.Popen(cmd)
    a.wait()	


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
	newentry.time_creation = 9
	newentry.time_modification = 9
	newentry.time_access = 9
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


def ls(path):
	path = path.strip('/')
	return str(filesystem.root.getdir_bypath(path))


def cp(origin, destination):
	pass


def rm(filepath):
	pass


def cat(filepath):
	pass


def touch(filepath):
	pass


def find(path, filename):
	pass


def df():
	pass
