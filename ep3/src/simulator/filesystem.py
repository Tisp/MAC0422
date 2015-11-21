from struct import Struct
from math import ceil


def test():
	#abre ou cri arquivo
	device_path = "data/teste"
	try: device = open(device_path, 'r+b')
	except: device = open(device_path, 'w+b')

	#opcoes do bitmap
	fmt_options = '<'
	bitmap_size = 10
	bitmap_fmt = 'B'
	bitmap_struct = Struct(fmt_options + ceil(bitmap_size/8)*bitmap_fmt)

	#opcoes da fat
	fat_size = bitmap_size
	fat_fmt = 'H'
	fat_struct = Struct(fmt_options + fat_size*fat_fmt)

	#opcoes de diretorio
	dir_namesize = 32
	dir_fmt = str(dir_namesize)+'s' + 'Iiii'
	dir_struct = Struct(fmt_options + dir_fmt)

	#le do arquivo
	try:
		#bitmap
		bitmap = []
		for x in bitmap_struct.unpack(device.read(bitmap_struct.size)):
			bitmap.extend([True if y=='1' else False for y in '{0:08b}'.format(x)])
		bitmap = bitmap[:bitmap_size]
		#fat
		fat = list(fat_struct.unpack(device.read(fat_struct.size)))
		#root
		root = list(dir_struct.unpack(device.read(dir_struct.size)))
		root[0] = root[0].decode('utf-8').strip('\0')
		print(root)

	#cria dados vazios
	except:
		print("formato invalido!")
		bitmap = [False] * bitmap_size
		fat = [0] * fat_size
		root = ['', 0, 0, 0, 0]

	#imprime dados
	print(bitmap)
	print(fat)

	#modifica dados
	pos = (fat[0] % (fat_size-1)) + 1
	fat[0] += 1
	bitmap[pos] = (bitmap[pos]+1) % 2
	fat[pos] += 1
	root[0] += 'a' if pos%2 else 'b'
	for i in [1,2,3,4]: root[i] += 10

	#empacota bitmap
	bitmap_packed = [0] * (ceil(bitmap_size/8))
	for i,x in enumerate(bitmap):
		i = i//8
		bitmap_packed[i] = (bitmap_packed[i]<<1) | x
	for i in range(8-(bitmap_size%8)):
		bitmap_packed[-1] = bitmap_packed[-1] << 1

	#escreve dados
	device.seek(0)
	device.write(bitmap_struct.pack(*bitmap_packed))
	device.write(fat_struct.pack(*fat))
	device.write(dir_struct.pack(root[0].encode('utf-8'), *root[1:]))

