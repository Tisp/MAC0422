from linkedlist import LinkedList


class MemManager:
	"""Gerencia a alocacao e intermedia o acesso a um objeto de memoria."""

	def __init__(self, mem, alloc):
		"""Recebe uma memoria mem e uma funcao alloc que implementa o algoritmo de alocacao."""
		self.mem = mem
		self.alloc = alloc
		self.alloclist = LinkedList()

	def create(self, id, size):
		"""Aloca memoria de tamanho size para um processo de com o id fornecido."""
		pos,base = self.alloc(self, size)
		self.alloclist.add(pos, {'id':id, 'size':size, 'base':base})
		for i in range(size):
			self.mem.writebyte(base+i, id)

	def delete(self, id):
		"""Libera a memoria alocada para o processo id."""
		self.alloclist.remove(self.get_alloc(id, True))

	def read(self, id, pos):
		"""Le uma posicao de memoria do processo com o id fornecido."""
		pos += self.get_alloc(id)['base']
		return self.mem.readbyte(pos)

	def get_alloc(self, id, index=False):
		"""Retorna o indice ou a entrada do processo com o id fornecido na lista de alocacao."""
		for i,x in enumerate(self.alloclist):
			if x['id'] == id:
				return i if index else x
