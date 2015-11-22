from struct import Struct
from . import filesystem as fs


class Entry:

	#formato binario
	entry_namesize = 31
	entry_fmt = str(entry_namesize)+'s' + 'BIiiiH'
	entry_struct = Struct(fs.fmt_options + entry_fmt)
	entry_filetypes = ['empty', 'dir', 'file']


	def __init__(self, pos, new=False):
		self.pos = pos
		if not new:
			self.unpack(fs.read(self.pos, self.entry_struct.size))
		else:
			self.clear()


	def clear(self):
		self.name = ''
		self.filetype = 'empty'
		self.size = 0
		self.time_creation = 0
		self.time_modification = 0
		self.time_access = 0
		self.sector = 0


	def unpack(self, data):
		entry = self.entry_struct.unpack(data)
		self.name = entry[0].decode('utf-8').strip('\0')
		self.filetype = self.entry_filetypes[entry[1]]
		self.size = entry[2]
		self.time_creation = entry[3]
		self.time_modification = entry[4]
		self.time_access = entry[5]
		self.sector = entry[6]


	def pack(self):
		return self.entry_struct.pack(self.name.encode('utf-8'), self.entry_filetypes.index(self.filetype), self.size, self.time_creation, self.time_modification, self.time_access, self.sector)


	def commit(self):
		fs.write(self.pos, self.pack())


	def __str__(self):
		ret = "Nome: " + str(self.name) + "\n"
		ret += "Tipo: " + self.filetype + "\n"
		ret += "Tamanho: " + str(self.size) + " bytes\n"
		ret += "Data de criacao: " + str(self.time_creation) + "\n"
		ret += "Data de modificacao: " + str(self.time_modification) + "\n"
		ret += "Data de acesso: " + str(self.time_access) + "\n"
		ret += "Setor inicial: " + str(self.sector)
		return ret



class Directory:

	num_entries = 3
	size = num_entries * Entry.entry_struct.size


	def __init__(self, entry, new=False):
		self.pos = entry.sector * fs.sector_size
		self.entries = [None] * self.num_entries
		self.new = new


	@classmethod
	def frompos(cls, sector):
		self = cls.__new__(cls)
		self.pos = sector * fs.sector_size
		self.entries = [None] * self.num_entries
		self.new = False
		return self


	def commit(self):
		for entry in self: entry.commit()


	def getentry(self, i):
		if self.entries[i] == None:
			self.entries[i] = Entry(self.pos + i*Entry.entry_struct.size, self.new)
		return self.entries[i]


	def getentry_byname(self, name):
		for entry in self:
			if entry.name == name and entry.filetype != 'empty':
				return entry
		raise Exception("Arquivo ou diretorio '{}' nao encontrado".format(name))


	def getentry_empty(self):
		for entry in self:
			if entry.filetype == 'empty': return entry
		raise Exception("Diretorio cheio")


	def getentry_bypath(self, path):
		path = path.split('/')
		for directory in path[:-1]:
			entry = self.getentry_byname(directory)
			if entry.filetype != 'dir':
				raise Exception("'{}' nao e um diretorio".format(entry.name))
			self = Directory(entry)
		return self.getentry_byname(path[-1])


	def getdir_bypath(self, path):
		if path == '':
			return self
		else:
			entry = self.getentry_bypath(path)
			if entry.filetype != 'dir': raise Exception("'{}' nao e um diretorio".format(entry.name))
			return Directory()


	def __iter__(self):
		for i in range(self.num_entries):
			yield self.getentry(i)


	def __str__(self):
		ret = ''
		for entry in self: ret += str(entry) + '\n\n'
		return ret[:-2]


	@staticmethod
	def splitpath(path):
		path = path.strip('/')
		path = path.split('/')
		base = '/'.join(path[:-1])
		return base, path[-1]
