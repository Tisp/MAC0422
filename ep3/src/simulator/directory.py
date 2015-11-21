from struct import Struct
from . import filesystem as fs


class Directory:

	num_entries = 2


	def __init__(self, pos):
		self.pos = pos
		self.entries = [None] * self.num_entries


	def get_entry(self, i):
		if self.entries[i] == None:
			self.entries[i] = Entry(self, i)
		return self.entries[i]


	def get_entry_byname(self, name):
		for entry in self:
			if entry.name == name: return entry


	def __iter__(self):
		for i in range(self.num_entries):
			yield self.get_entry(i)



class Entry:

	#formato binario
	entry_namesize = 33
	entry_fmt = str(entry_namesize)+'s' + 'BIiii'
	entry_struct = Struct(fs.fmt_options + entry_fmt)
	entry_filetypes = ['empty', 'dir', 'file']


	def __init__(self, parent, pos):
		self.pos = parent.pos + pos*self.entry_struct.size
		self.parent = parent
		self.unpack(fs.read(self.pos, self.entry_struct.size))


	def unpack(self, data):
		entry = self.entry_struct.unpack(data)
		self.name = entry[0].decode('utf-8').strip('\0')
		self.filetype = self.entry_filetypes[entry[1]]
		self.size = entry[2]
		self.time_creation = entry[3]
		self.time_modification = entry[4]
		self.time_access = entry[5]


	def pack(self):
		return self.entry_struct.pack(self.name.encode('utf-8'), self.entry_filetypes.index(self.filetype), self.size, self.time_creation, self.time_modification, self.time_access)


	def commit(self):
		fs.write(self.pos, self.pack())


	def __str__(self):
		ret = "Nome: " + str(self.name) + "\n"
		ret += "Tipo: " + self.filetype + "\n"
		ret += "Tamanho: " + str(self.size) + " bytes\n"
		ret += "Data de criacao: " + str(self.time_creation) + "\n"
		ret += "Data de modificacao: " + str(self.time_modification) + "\n"
		ret += "Data de acesso: " + str(self.time_access)
		return ret
