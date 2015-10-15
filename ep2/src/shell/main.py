from prompt import *
import simulator

prompt = Prompt()

carrega = 'tracefile'
espaco = 1
substitui = 1
intervalo = None 

while True:
    command = prompt.read()

    if command.is_valid():
        if command.argv[0] == 'sair':
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

        """ Executa o simulador """
        if isinstance(intervalo, int) and intervalo > 0:
            simulador.main()
            #Reseta as variaveis para o simulador
            carrega = 'tracefile'
            espaco = 1
            substitui = 1
            intervalo = None
    else:
        print ("%s\n%s" % (command.get_error(), prompt.help()))
