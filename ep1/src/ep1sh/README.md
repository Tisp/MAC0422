O shell ep1sh
O shell, chamado de ep1sh, a ser desenvolvido, deve permitir a invocac ̧a ̃o (execuc ̧a ̃o) dos 2 bina ́rios abaixo com exatamente os argumentos abaixo. Na ̃o ha ́ necessidade de testar o shell para outros programas e nem para os bina ́rios abaixo com outros argumentos:
• /bin/ls -1
• ./ep1 <argumentos do EP1>
O shell tambe ́m precisa ter os 2 comandos abaixo embutidos nele, que devem obrigatoriamente ser implementados usando chamadas de sistema do Linux. Esses comandos devem ser executados sempre com os argumentos abaixo e que devem fazer exatamente o que esses 2 comandos fazem no shell bash:
• cd <novo diret ́orio para mudar>
• pwd
Na ̃o se preocupe em tratar os erros docd. O usua ́rio nunca vai tentar entrar em algum direto ́rio que na ̃o existe.
O shell tem que suportar a listagem de comandos que foram executados previamente, e a edic ̧a ̃o desses comandos para serem executados, por meio das funcionalidades das bibliotecas GNU readline e GNU history. No Debian ambas fazem parte do pacotelibreadline-dev. Mais informac ̧o ̃es podem ser vistas na documentac ̧a ̃o da biblioteca em ftp://ftp.gnu.org/pub/gnu/readline/ . Na ̃o ha ́ necessidade de utilizar outras funcionalidades das bibliotecas ale ́m das requisitadas no in ́ıcio deste para ́ grafo.
O prompt do shell deve conter o direto ́rio atual entre colchetes seguido de um espac ̧o em branco, como no exemplo abaixo que mostra o shell pronto para rodar o comando cd que mudara ́ o direto ́ rio atual para o /tmp:
[/home/mac/] cd /tmp
