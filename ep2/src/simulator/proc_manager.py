class ProcessManager:
	"""Classe que cuida da gerencia dos processos."""

	def __init__(self, manager):
		"""Recebe um gerenciador de memoria apenas."""
		self.manager = manager
		self.proclist = [None]*255

	def create(self, endtime, size, accesslist):
		"""Recebe o instante final, o tamanho e a lista de acessos a memoria de um processo e o cria."""
		uid = self.proclist.index(None)
		self.manager.allocate(uid, size)
		self.proclist[uid] = {'endtime':endtime, 'accesslist':accesslist}

	def delete(self, uid):
		"""Deleta um processo."""
		self.manager.delete(uid)
		self.proclist[uid] = None

	def runall(self, time):
		"""Verifica todos os processos, acessando a memoria dos que o pedirem. Recebe o instante de tempo atual como paremetro."""
		for i,x in enumerate(self.proclist):
			if x is not None:
				while len(x['accesslist'])>0 and x['accesslist'][0][0]==time:
					self.manager.read(i, x['accesslist'][0][1])
					del x['accesslist'][0]

	def clean(self, time):
		"""Verifica a lista de processos deletando os que ja tiverem terminado."""
		for i,x in enumerate(self.proclist):
			if x is not None and x['endtime'] <= time:
				self.delete(i)

	def size(self):
		"""Retorna o numero de processos que ainda nao terminaram."""
		ret = 0
		for i in self.proclist:
			if i is not None:
				ret += 1
		return ret
