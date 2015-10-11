from io import SEEK_SET


class FileMem:
	"""Interface para lidar com arquivos como se fossem memoria."""
	def __init__(self, file, size, pagesize):
		self.size = size
		self.pagesize = pagesize
		self.npages = size//pagesize
		self.file = open(file, 'w+b', 0)
		written = self.file.write(bytes([255]*size))
		assert written == size

	def readbyte(self, pos):
		self.file.seek(pos, SEEK_SET)
		return int(self.file.read(1)[0])

	def writebyte(self, pos, data):
		self.file.seek(pos, SEEK_SET)
		self.file.write(bytes([data]))

	def readpage(self, page):
		self.file.seek(page*self.pagesize, SEEK_SET)
		return [int(x) for x in self.file.read(self.pagesize)]

	def writepage(self, page, data):
		self.file.seek(page*self.pagesize, SEEK_SET)
		self.file.write(bytes(data))


class VirtMem:
	"""Junta duas memorias (uma ram e uma swap) em uma memoria virtual."""

	def __init__(self, ram, swap, pager):
		"""Recebe as memorias ram e swap e uma funcao pager que implementa o algoritmo de subtituicao de pagina."""
		self.ram = ram
		self.swap = swap
		self.pagesize = ram.pagesize
		self.size = ram.size + swap.size
		self.npages = self.size // self.pagesize
		self.pager = pager
		self.table = []
		for i in range(ram.npages): self.table.append({'loc':'ram', 'page':i})
		for i in range(swap.npages): self.table.append({'loc':'swap', 'page':i})

	def readbyte(self, vpos):
		rpos = self.fetch(vpos)
		return self.ram.readbyte(rpos)

	def writebyte(self, vpos, data):
		rpos = self.fetch(vpos)
		self.ram.writebyte(rpos, data)

	def fetch(self, vpos):
		"""Retorna o endereco da ram onde esta a posicao de memoria virtual vpos (trazendo-a para a ram se necessario)."""
		vpage = vpos // self.pagesize
		offset = vpos % self.pagesize
		if self.table[vpage]['loc'] == 'ram':
			rpage = self.table[vpage]['page']
		else:
			vpage_toremove = self.pager(self, vpage)
			self.swap_page(vpage, vpage_toremove)
			rpage = self.table[vpage]['page']
		return self.pagesize*rpage + offset

	def swap_page(self, newpage, oldpage):
		"""Troca uma pagina que esta no swap(newpage) com uma pagina que esta na ram(oldpage)."""
		assert self.table[oldpage]['loc']=='ram' and self.table[newpage]['loc']=='swap'
		realpage = self.table[oldpage]['page']
		swappage = self.table[newpage]['page']
		olddata = self.ram.readpage(realpage)
		newdata = self.swap.readpage(swappage)
		self.ram.writepage(realpage, newdata)
		self.swap.writepage(swappage, olddata)
		self.table[newpage] = {'loc':'ram', 'page':realpage}
		self.table[oldpage] = {'loc':'swap', 'page':swappage}
