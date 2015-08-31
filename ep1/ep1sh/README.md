<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />
### O shell ep1sh


O shell, chamado de ep1sh, a ser desenvolvido, deve permitir a invocação (execução) dos 2 binários abaixo com exatamente os argumentos abaixo. Não há necessidade de testar o shell para outros programas e nem para os binários abaixo com outros argumentos:

* /bin/ls -1
* ./ep1 <argumentos do EP1>

O shell também precisa ter os 2 comandos abaixo embutidos nele, que devem obrigatoriamente ser implementados usando chamadas de sistema do Linux. Esses comandos devem ser executados sempre com os argumentos abaixo e que devem fazer exatamente o que esses 2 comandos fazem no shell bash:

* cd <novo diretório para mudar>
* pwd

Não se preocupe em tratar os erros do cd. O usuário nunca vai tentar entrar em algum diretório que não existe.  
O shell tem que suportar a listagem de comandos que foram executados previamente, e a edição desses comandos para serem executados, por meio das funcionalidades das bibliotecas GNU readline e GNU history. No Debian ambas fazem parte do pacotelibreadline-dev. Mais informaçãoes podem ser vistas na documentação da biblioteca em ftp://ftp.gnu.org/pub/gnu/readline/ . Não há necessidade de utilizar outras funcionalidades das bibliotecas além das requisitadas no início deste parágrafo.  
O prompt do shell deve conter o diretório atual entre colchetes seguido de um espaços em branco, como no exemplo abaixo que mostra o shell pronto para rodar o comando cd que mudaráo diretório atual para o /tmp:
     
     [/home/mac/] cd /tmp

