class ProcessManager:

	def __init__(self, manager):
		self.manager = manager
		self.proclist = [None]*255

	def create(self, size, accesslist):
		uid = self.proclist.index(None)
		self.manager.allocate(uid, size)
		self.proclist[uid] = accesslist

	def delete(self, uid):
		self.manager.delete(uid)
		self.proclist[uid] = None

	def runall(self, time):
		for i,x in enumerate(self.proclist):
			if x is not None:
				if x[0][0] == time:
					self.manager.read(i, x[0][1])
					del x[0]
				if len(x) == 0:
					self.delete(i)

	def size(self):
		ret = 0
		for i in self.proclist:
			if i is not None:
				ret += 1
		return ret
