from prompt import *
import simulator


def main():
	prompt = Prompt()

	carrega = 'tracefile'
	espaco = 1
	substitui = 2
	intervalo = None
	
	ram = '/tmp/ep2.mem'
	swap = '/tmp/ep2.vir'
	pagina = 16

	while True:
		command = prompt.read()

		if command.is_valid():
			if command.argv[0] == 'sai':
				break
			if command.argv[0] == 'carrega':
				carrega = command.argv[1]
			else:
				try:
					comm = command.argv[0]
					param = int(command.argv[1])
					if comm == 'espaco':
						espaco = param
					elif comm == 'substitui':
						substitui = param
					elif comm == 'executa':
						intervalo = param
				except:
					print ("%s\n%s" % (command.get_error(), prompt.help()))
					continue

			""" Executa o simulador """
			if command.argv[0] == 'executa' and isinstance(intervalo, int) and intervalo > 0:
				simulator.main(ram, swap, pagina, carrega, substitui, espaco, intervalo)
		else:
			print ("%s\n%s" % (command.get_error(), prompt.help()))


if __name__ == '__main__':
	main()
