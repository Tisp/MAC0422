from math import ceil
from struct import Struct


#estruturas globais
device = None
fat = None
bitmap = None
root = None

#opcoes do sistema de arquivos
sector_size = 50

#formato binario dos dados
fmt_options = '<'

bitmap_size = 15
bitmap_fmt = 'B'
bitmap_struct = Struct(fmt_options + ceil(bitmap_size/8)*bitmap_fmt)

fat_size = bitmap_size
fat_marker = 65535
fat_fmt = 'H'
fat_struct = Struct(fmt_options + fat_size*fat_fmt)

storage_offset = bitmap_struct.size + fat_struct.size


def mount(filepath):
	global device, bitmap, fat, root

	#abre aquivo
	device = open(filepath, 'r+b')

	#le bitmap
	bitmap = []
	for byte in bitmap_struct.unpack(device.read(bitmap_struct.size)):
		bitmap.extend([True if y=='1' else False for y in '{0:08b}'.format(byte)])
	bitmap = bitmap[:bitmap_size]

	#le fat
	fat = list(fat_struct.unpack(device.read(fat_struct.size)))

	#le diretorio raiz e todas suas entradas para ficar em cache
	from . import directory
	root = directory.Directory.frompos(0)
	root.commit()


def umount():
	global device, bitmap, fat, root

	#escreve metadados no arquivo
	flush()

	#limpa cache e valores em memoria
	device.close()
	device = bitmap = fat = root = None


def flush():
	#empacota bitmap em inteiros de um byte
	bitmap_packed = [0] * (ceil(bitmap_size/8))
	for i,bit in enumerate(bitmap):
		i = i//8
		bitmap_packed[i] = (bitmap_packed[i]<<1) | bit
	for i in range(8-(bitmap_size%8)):
		bitmap_packed[-1] = bitmap_packed[-1] << 1

	#escreve diretorio raiz, nao deveria ser necessario mas nao custa nada
	root.commit()

	#escreve metadados no arquivo
	device.seek(0)
	device.write(bitmap_struct.pack(*bitmap_packed))
	device.write(fat_struct.pack(*fat))


def zero():
	global device, bitmap, fat, root

	from . import directory
	fat = [0] * fat_size
	bitmap = [False] * bitmap_size
	alloc(directory.Directory.size)
	root = directory.Directory.frompos(0, True)
	root.commit()


def read(pos, size):
	device.seek(storage_offset + pos)
	return device.read(size)


def write(pos, data):
	device.seek(storage_offset + pos)
	device.write(data)


def alloc(size):
	nblocks = ceil(size/sector_size)
	blocks = [None] * nblocks

	for i in range(nblocks):
		try:
			block = bitmap.index(False)
			bitmap[block] = True
			blocks[i] = block
		except:
			for block in blocks:
				if block is not None:
					bitmap[block] = False
			raise Exception("Sem espaco livre")

	for i,block in enumerate(blocks[:-1]):
		fat[block] = blocks[i+1]
	fat[blocks[-1]] = fat_marker

	return blocks[0]


def free(block):
	while(fat[block] != fat_marker):
		next_block = fat[block]
		fat[block] = 0
		bitmap[block] = False
		block = next_block
	fat[block] = 0
	bitmap[block] = False
