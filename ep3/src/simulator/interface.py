from . import filesystem
from . import directory


def test():
	try:
		filesystem.mount("data/filesystem.bin")

		print(filesystem.bitmap)
		print(filesystem.fat)

		pos = (filesystem.fat[0] % (filesystem.fat_size-1)) + 1
		filesystem.fat[0] += 1

		filesystem.bitmap[pos] = (filesystem.bitmap[pos]+1) % 2
		filesystem.fat[pos] += 1

		i = filesystem.fat[0]%2
		for entry in filesystem.root:
			print(entry)
			if i:
				entry.name += 'a'
				entry.size += 1000
				entry.filetype = 'file'
				entry.time_creation += 100
				entry.time_modification += 100
				entry.time_access += 100
			else:
				entry.name += 'b'
				entry.size += 10
				entry.filetype = 'dir'
				entry.time_creation += 1
				entry.time_modification += 1
				entry.time_access += 1
			entry.commit()
			i = not i

		filesystem.umount()

	except Exception as ex:
		print("Erro!")
		raise


def mount(filepath):
	#ve se o arquivo existe e eh do tamanho certo, cria se nao existir
	pass


def umount():
	pass


def mkdir(path):
	pass


def rmdir(path):
	pass


def ls(path):
	pass


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
