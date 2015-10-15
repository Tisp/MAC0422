def firstfit(manager, size):
	"""Implementacao do first-fit."""
	for i,x in enumerate(manager.alloclist):
		if x['occupied']==False and x['size']>=size:
			return i

def nextfit(manager, size):
	"""Implementacao do next-fit, guarda a base do ultimo segmento alocado e procura a partir dali."""
	for i,x in enumerate(manager.alloclist):
		if x['base']>nextfit.last and x['occupied']==False and x['size']>=size:
			nextfit.last = x['base']
			return i
	nextfit.last = -1
	return nextfit(manager, size)


nextfit.last = -1
